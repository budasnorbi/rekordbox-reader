#include <uv.h>
#include <node.h>
#include <filesystem>
#include <variant>
#include <mach-o/dyld.h>
#include <iostream>
#include <dlfcn.h>
#include <codecvt>
#include <locale>
#include <string>

using namespace v8;
#define MAX_PATH 4096

struct ChangesStruct
{
    std::vector<unsigned char> data;
};

typedef ChangesStruct *(*DylibFunction1)();

std::string GetCurrentDirectory()
{
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0)
    {
        std::string::size_type pos = std::string(buffer).find_last_of("/");
        return std::string(buffer).substr(0, pos);
    }
    else
    {
        // Handle error
        return "";
    }
}

struct Work
{
    uv_work_t request;
    Persistent<Function> callback;
    uv_async_t async;
    std::vector<std::variant<double, uint32_t>> *data;
};

#define DOUBLE_SIZE ((int)sizeof(double))

const int MY_LITTLE_ENDIAN = 1234;
const int MY_BIG_ENDIAN = 4321;

double ToDouble(std::vector<unsigned char>::const_iterator begin, std::vector<unsigned char>::const_iterator end, int byteOrder = MY_LITTLE_ENDIAN)
{
    double retVal = 0;
    unsigned char *p = (unsigned char *)&retVal;

    if (byteOrder == MY_BIG_ENDIAN)
    {
        for (int32_t i = 0; i < DOUBLE_SIZE; i++)
        {
            p[i] = *(end - (i + 1));
        }
    }

    if (byteOrder == MY_LITTLE_ENDIAN)
    {
        for (int32_t i = 0; i < DOUBLE_SIZE; i++)
        {
            p[i] = *(begin + i);
        }
    }

    return retVal;
}

uint32_t ToUint32(std::vector<unsigned char>::const_iterator begin, std::vector<unsigned char>::const_iterator end)
{
    std::vector<unsigned char> v(begin, end);
    std::reverse(v.begin(), v.end());
    uint32_t result = 0;
    result |= v[0] << 24;
    result |= v[1] << 16;
    result |= v[2] << 8;
    result |= v[3];
    return result;
}

void AsyncCb(uv_async_t *handle)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Work *work = static_cast<Work *>(handle->data);

    Local<Function> callback = Local<Function>::New(isolate, work->callback);
    Local<Array> arr = Array::New(isolate);

    // Add each element of the vector to the array
    for (size_t i = 0; i < work->data->size(); i++)
    {
        if (std::holds_alternative<double>((*work->data)[i]))
        {
            double val = std::get<double>((*work->data)[i]);
            (void)arr->Set(isolate->GetCurrentContext(), i, Number::New(isolate, val));
        }
        else if (std::holds_alternative<uint32_t>((*work->data)[i]))
        {
            uint32_t val = std::get<uint32_t>((*work->data)[i]);
            (void)arr->Set(isolate->GetCurrentContext(), i, Integer::NewFromUnsigned(isolate, val));
        }
    }

    Local<Value> argv[1] = {arr};
    (void)callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 1, argv);
}

void WorkAsync(uv_work_t *req)
{
    std::string filePath = "node_modules/rekordbox-reader/reader.dylib";
    std::filesystem::path absolutePath = std::filesystem::absolute(filePath);

    std::wstring wstrAbsolutePath = absolutePath.wstring();

    // Convert wide string to UTF-8 string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::string strAbsolutePath = conv.to_bytes(wstrAbsolutePath);

    Work *work = static_cast<Work *>(req->data);

    ChangesStruct *_mystruct = new ChangesStruct();
    std::vector<std::variant<double, uint32_t>> *data = new std::vector<std::variant<double, uint32_t>>();

    while (true)
    {
        void *dylibHandle = NULL;
        dylibHandle = dlopen("./reader.dylib", RTLD_LAZY);
        if (dylibHandle == NULL)
        {
            // Handle error
            printf("%s", "dll error");
            return;
        }

        DylibFunction1 getData = reinterpret_cast<DylibFunction1>(dlsym(dylibHandle, "getData"));

        const char *error = NULL;
        error = dlerror();
        if (error)
        {
            printf("%s", "error");
            dlclose(dylibHandle);
            return;
        }

        if (getData == NULL)
        {
            printf("%s", "error");
            dlclose(dylibHandle);
        }

        ChangesStruct *mystruct = getData();

        if (_mystruct->data != mystruct->data)
        {
            _mystruct->data = mystruct->data;
            data->clear();

            data->push_back(ToDouble(_mystruct->data.begin(), _mystruct->data.begin() + 8));
            data->push_back(ToUint32(_mystruct->data.begin() + 8, _mystruct->data.begin() + 12));
            data->push_back(ToDouble(_mystruct->data.begin() + 12, _mystruct->data.begin() + 20));

            data->push_back(ToDouble(_mystruct->data.begin() + 20, _mystruct->data.begin() + 28));
            data->push_back(ToUint32(_mystruct->data.begin() + 28, _mystruct->data.begin() + 32));
            data->push_back(ToDouble(_mystruct->data.begin() + 32, _mystruct->data.begin() + 40));

            work->data = data;
            uv_async_send(&work->async);
        }

        delete mystruct;
        dlclose(dylibHandle);
    }
}

void WorkAsyncComplete(uv_work_t *req, int status)
{
    Work *work = static_cast<Work *>(req->data);
    uv_close(reinterpret_cast<uv_handle_t *>(&work->async), NULL);
    delete work;
}

void trackChanges(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // Parse the arguments: callback function and loop condition.
    Local<Function> callback = Local<Function>::Cast(args[0]);

    // Create a Work object to pass to the worker thread.
    Work *work = new Work();
    work->request.data = work;
    work->callback.Reset(isolate, callback);
    uv_async_init(uv_default_loop(), &work->async, AsyncCb);
    work->async.data = work;

    // Start the worker thread and pass it the Work object.
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);
}

void Init(Local<Object> exports, Local<Object> module)
{
    NODE_SET_METHOD(module, "exports", trackChanges);
}

NODE_MODULE(addon, Init)