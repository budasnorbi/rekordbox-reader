#include <uv.h>
#include <node.h>
#include <node_buffer.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
#include <variant>

using namespace v8;

struct ChangesStruct
{
    std::vector<unsigned char> data;
};

typedef ChangesStruct *(*DLLFunction1)();

std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

struct Work
{
    uv_work_t request;
    Persistent<Function> callback;
    uv_async_t async;
    std::vector<std::variant<double, uint32_t, unsigned char>> *data;
};

#define DOUBLE_SIZE ((int)sizeof(double))

const int LITTLE_ENDIAN = 1234;
const int BIG_ENDIAN = 4321;

double ToDouble(std::vector<unsigned char>::const_iterator begin, std::vector<unsigned char>::const_iterator end, int byteOrder = LITTLE_ENDIAN)
{
    double retVal = 0;
    unsigned char *p = (unsigned char *)&retVal;

    if (byteOrder == BIG_ENDIAN)
    {
        for (int32_t i = 0; i < DOUBLE_SIZE; i++)
        {
            p[i] = *(end - (i + 1));
        }
    }

    if (byteOrder == LITTLE_ENDIAN)
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
            arr->Set(isolate->GetCurrentContext(), i, Number::New(isolate, val));
        }
        else if (std::holds_alternative<uint32_t>((*work->data)[i]))
        {
            uint32_t val = std::get<uint32_t>((*work->data)[i]);
            arr->Set(isolate->GetCurrentContext(), i, Integer::NewFromUnsigned(isolate, val));
        }
        else if (std::holds_alternative<unsigned char>((*work->data)[i]))
        {
            unsigned char val = std::get<unsigned char>((*work->data)[i]);
            arr->Set(isolate->GetCurrentContext(), i, Boolean::New(isolate, (bool)val));
        }
    }

    Local<Value> argv[1] = {arr};

    callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 1, argv);
    delete work->data;
}

void WorkAsync(uv_work_t *req)
{
    std::string filePath = "node_modules/rekordbox-reader/reader.dll";
    std::filesystem::path absolutePath = std::filesystem::absolute(filePath);

    std::wstring wstrAbsolutePath = absolutePath.wstring();

    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstrAbsolutePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string strAbsolutePath(requiredSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstrAbsolutePath.c_str(), -1, strAbsolutePath.data(), requiredSize, nullptr, nullptr);

    Work *work = static_cast<Work *>(req->data);

    ChangesStruct *_mystruct = new ChangesStruct();

    while (true)
    {
        // strAbsolutePath.c_str()
        HMODULE dllHandle = LoadLibrary("./reader.dll");
        if (dllHandle == NULL)
        {
            // Handle error
            printf("%s", "dll error");
            return;
        }

        DLLFunction1 getData = (DLLFunction1)GetProcAddress(dllHandle, "getData");

        if (getData == NULL)
        {
            // Handle error
            printf("%s", "getData error");
            FreeLibrary(dllHandle);
        }

        ChangesStruct *mystruct = getData();

        if (_mystruct->data != mystruct->data)
        {
            _mystruct->data = mystruct->data;
            std::vector<std::variant<double, uint32_t, unsigned char>> *data = new std::vector<std::variant<double, uint32_t, unsigned char>>();

            data->push_back(ToDouble(_mystruct->data.begin(), _mystruct->data.begin() + 8));
            data->push_back(ToDouble(_mystruct->data.begin() + 8, _mystruct->data.begin() + 16));
            data->push_back(ToDouble(_mystruct->data.begin() + 16, _mystruct->data.begin() + 24));
            data->push_back(ToDouble(_mystruct->data.begin() + 24, _mystruct->data.begin() + 32));
            data->push_back(ToDouble(_mystruct->data.begin() + 32, _mystruct->data.begin() + 40));
            data->push_back(ToUint32(_mystruct->data.begin() + 40, _mystruct->data.begin() + 44));
            data->push_back(_mystruct->data[45]);
            data->push_back(ToDouble(_mystruct->data.begin() + 45, _mystruct->data.begin() + 53));
            data->push_back(ToDouble(_mystruct->data.begin() + 53, _mystruct->data.begin() + 60));

            data->push_back(ToDouble(_mystruct->data.begin() + 61, _mystruct->data.begin() + 69));
            data->push_back(ToDouble(_mystruct->data.begin() + 69, _mystruct->data.begin() + 77));
            data->push_back(ToDouble(_mystruct->data.begin() + 77, _mystruct->data.begin() + 85));
            data->push_back(ToDouble(_mystruct->data.begin() + 85, _mystruct->data.begin() + 93));
            data->push_back(ToDouble(_mystruct->data.begin() + 93, _mystruct->data.begin() + 101));
            data->push_back(ToUint32(_mystruct->data.begin() + 101, _mystruct->data.begin() + 105));
            data->push_back(_mystruct->data[106]);
            data->push_back(ToDouble(_mystruct->data.begin() + 106, _mystruct->data.begin() + 114));
            data->push_back(ToDouble(_mystruct->data.begin() + 114, _mystruct->data.begin() + 122));

            data->push_back(ToDouble(_mystruct->data.begin() + 122, _mystruct->data.begin() + 130));
            work->data = data;
            uv_async_send(&work->async);
        }

        FreeLibrary(dllHandle);
        delete mystruct;
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
    bool condition = args[1]->BooleanValue(isolate);

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