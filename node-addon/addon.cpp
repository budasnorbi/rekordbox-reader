#include <uv.h>
#include <node.h>
#include <node_buffer.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>

using namespace v8;

struct MyStruct
{
    double d1ChannelFader = 0.0;
    double d1CfxKnob = 0.0;
    double d1LowFilter = 0.0;
    double d1MidFilter = 0.0;
    double d1HighFilter = 0.0;
    uint32_t d1SongId = 0;
    int d1IsPlaying = -1;
    double d1CurrentTime = 0.0;
    double d1CalculatedTempo = 0.0;

    double d2ChannelFader = 0.0;
    double d2CfxKnob = 0.0;
    double d2LowFilter = 0.0;
    double d2MidFilter = 0.0;
    double d2HighFilter = 0.0;
    uint32_t d2SongId = 0;
    int d2IsPlaying = -1;
    double d2CurrentTime = 0.0;
    double d2CalculatedTempo = 0.0;
    double crossfader = 0.0;
};

typedef MyStruct *(*DLLFunction1)();

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
    MyStruct *data;
};

void AsyncCb(uv_async_t *handle)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Work *work = static_cast<Work *>(handle->data);

    Local<Function> callback = Local<Function>::New(isolate, work->callback);

    Local<Object> obj = Object::New(isolate);
    int updatedValues = 0;

    if (work->data->d1ChannelFader)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1ChannelFader").ToLocalChecked(), Number::New(isolate, work->data->d1ChannelFader));
        updatedValues++;
    }

    if (work->data->d1CfxKnob)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1CfxKnob").ToLocalChecked(), Number::New(isolate, work->data->d1CfxKnob));
        updatedValues++;
    }

    if (work->data->d1LowFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1LowFilter").ToLocalChecked(), Number::New(isolate, work->data->d1LowFilter));
        updatedValues++;
    }

    if (work->data->d1MidFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1MidFilter").ToLocalChecked(), Number::New(isolate, work->data->d1MidFilter));
        updatedValues++;
    }

    if (work->data->d1HighFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1HighFilter").ToLocalChecked(), Number::New(isolate, work->data->d1HighFilter));
        updatedValues++;
    }

    if (work->data->d1SongId)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1SongId").ToLocalChecked(), Number::New(isolate, work->data->d1SongId));
        updatedValues++;
    }

    if (work->data->d1IsPlaying != -1)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1IsPlaying").ToLocalChecked(), Boolean::New(isolate, (bool)work->data->d1IsPlaying));
        updatedValues++;
    }

    if (work->data->d1CurrentTime)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1CurrentTime").ToLocalChecked(), Number::New(isolate, work->data->d1CurrentTime));
        updatedValues++;
    }

    if (work->data->d1CalculatedTempo)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1CalculatedTempo").ToLocalChecked(), Number::New(isolate, work->data->d1CalculatedTempo));
        updatedValues++;
    }

    if (work->data->d2ChannelFader)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2ChannelFader").ToLocalChecked(), Number::New(isolate, work->data->d2ChannelFader));
        updatedValues++;
    }

    if (work->data->d2CfxKnob)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2CfxKnob").ToLocalChecked(), Number::New(isolate, work->data->d2CfxKnob));
        updatedValues++;
    }

    if (work->data->d2LowFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2LowFilter").ToLocalChecked(), Number::New(isolate, work->data->d2LowFilter));
        updatedValues++;
    }

    if (work->data->d2MidFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2MidFilter").ToLocalChecked(), Number::New(isolate, work->data->d2MidFilter));
        updatedValues++;
    }

    if (work->data->d2HighFilter)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2HighFilter").ToLocalChecked(), Number::New(isolate, work->data->d2HighFilter));
        updatedValues++;
    }

    if (work->data->d2SongId)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2SongId").ToLocalChecked(), Number::New(isolate, work->data->d2SongId));
        updatedValues++;
    }

    if (work->data->d2IsPlaying != -1)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2IsPlaying").ToLocalChecked(), Boolean::New(isolate, (bool)work->data->d2IsPlaying));
        updatedValues++;
    }

    if (work->data->d2CurrentTime)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2CurrentTime").ToLocalChecked(), Number::New(isolate, work->data->d2CurrentTime));
        updatedValues++;
    }

    if (work->data->d2CalculatedTempo)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2CalculatedTempo").ToLocalChecked(), Number::New(isolate, work->data->d2CalculatedTempo));
        updatedValues++;
    }

    if (work->data->crossfader)
    {
        obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "crossfader").ToLocalChecked(), Number::New(isolate, work->data->crossfader));
        updatedValues++;
    }

    if (updatedValues != 0)
    {
        Local<Value> argv[1] = {obj};
        callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 1, argv);
    }

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

    MyStruct *_mystruct = new MyStruct();
    int updatedValues = 0;

    while (true)
    {
        HMODULE dllHandle = LoadLibrary(strAbsolutePath.c_str());
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

        MyStruct *mystruct = getData();
        MyStruct *data = new MyStruct();

        if (mystruct->d1ChannelFader != _mystruct->d1ChannelFader)
        {
            double d1ChannelFader = mystruct->d1ChannelFader;
            _mystruct->d1ChannelFader = d1ChannelFader;
            data->d1ChannelFader = d1ChannelFader;

            updatedValues++;
        }

        if (mystruct->d1CfxKnob != _mystruct->d1CfxKnob)
        {
            double d1CfxKnob = mystruct->d1CfxKnob;
            _mystruct->d1CfxKnob = d1CfxKnob;
            data->d1CfxKnob = d1CfxKnob;
            updatedValues++;
        }

        if (mystruct->d1LowFilter != _mystruct->d1LowFilter)
        {
            double d1LowFilter = mystruct->d1LowFilter;

            _mystruct->d1LowFilter = d1LowFilter;
            data->d1LowFilter = d1LowFilter;
            updatedValues++;
        }

        if (mystruct->d1MidFilter != _mystruct->d1MidFilter)
        {
            double d1MidFilter = mystruct->d1MidFilter;

            _mystruct->d1MidFilter = d1MidFilter;
            data->d1MidFilter = d1MidFilter;
            updatedValues++;
        }

        if (mystruct->d1HighFilter != _mystruct->d1HighFilter)
        {
            double d1HighFilter = mystruct->d1HighFilter;

            _mystruct->d1HighFilter = d1HighFilter;
            data->d1HighFilter = d1HighFilter;
            updatedValues++;
        }

        if (mystruct->d1SongId != _mystruct->d1SongId)
        {
            uint32_t d1SongId = mystruct->d1SongId;
            _mystruct->d1SongId = d1SongId;
            data->d1SongId = d1SongId;
            updatedValues++;
        }

        if (mystruct->d1IsPlaying != _mystruct->d1IsPlaying)
        {
            int d1IsPlaying = mystruct->d1IsPlaying;

            _mystruct->d1IsPlaying = d1IsPlaying;
            data->d1IsPlaying = d1IsPlaying;
            updatedValues++;
        }

        if (mystruct->d1CurrentTime != _mystruct->d1CurrentTime)
        {
            double d1CurrentTime = mystruct->d1CurrentTime;

            _mystruct->d1CurrentTime = d1CurrentTime;
            data->d1CurrentTime = d1CurrentTime;
            updatedValues++;
        }

        if (mystruct->d1CalculatedTempo != _mystruct->d1CalculatedTempo)
        {
            double d1CalculatedTempo = mystruct->d1CalculatedTempo;

            _mystruct->d1CalculatedTempo = d1CalculatedTempo;
            data->d1CalculatedTempo = d1CalculatedTempo;
            updatedValues++;
        }

        if (mystruct->d2ChannelFader != _mystruct->d2ChannelFader)
        {
            double d2ChannelFader = mystruct->d2ChannelFader;
            _mystruct->d2ChannelFader = d2ChannelFader;
            data->d2ChannelFader = d2ChannelFader;

            updatedValues++;
        }

        if (mystruct->d2CfxKnob != _mystruct->d2CfxKnob)
        {
            double d2CfxKnob = mystruct->d2CfxKnob;
            _mystruct->d2CfxKnob = d2CfxKnob;
            data->d2CfxKnob = d2CfxKnob;
            updatedValues++;
        }

        if (mystruct->d2LowFilter != _mystruct->d2LowFilter)
        {
            double d2LowFilter = mystruct->d2LowFilter;

            _mystruct->d2LowFilter = d2LowFilter;
            data->d2LowFilter = d2LowFilter;
            updatedValues++;
        }

        if (mystruct->d2MidFilter != _mystruct->d2MidFilter)
        {
            double d2MidFilter = mystruct->d2MidFilter;

            _mystruct->d2MidFilter = d2MidFilter;
            data->d2MidFilter = d2MidFilter;
            updatedValues++;
        }

        if (mystruct->d2HighFilter != _mystruct->d2HighFilter)
        {
            double d2HighFilter = mystruct->d2HighFilter;

            _mystruct->d2HighFilter = d2HighFilter;
            data->d2HighFilter = d2HighFilter;
            updatedValues++;
        }

        if (mystruct->d2SongId != _mystruct->d2SongId)
        {
            uint32_t d2SongId = mystruct->d2SongId;
            _mystruct->d2SongId = d2SongId;
            data->d2SongId = d2SongId;
            updatedValues++;
        }

        if (mystruct->d2IsPlaying != _mystruct->d2IsPlaying)
        {
            int d2IsPlaying = mystruct->d2IsPlaying;

            _mystruct->d2IsPlaying = d2IsPlaying;
            data->d2IsPlaying = d2IsPlaying;
            updatedValues++;
        }

        if (mystruct->d2CurrentTime != _mystruct->d2CurrentTime)
        {
            double d2CurrentTime = mystruct->d2CurrentTime;

            _mystruct->d2CurrentTime = d2CurrentTime;
            data->d2CurrentTime = d2CurrentTime;
            updatedValues++;
        }

        if (mystruct->d2CalculatedTempo != _mystruct->d2CalculatedTempo)
        {
            double d2CalculatedTempo = mystruct->d2CalculatedTempo;

            _mystruct->d2CalculatedTempo = d2CalculatedTempo;
            data->d2CalculatedTempo = d2CalculatedTempo;
            updatedValues++;
        }

        if (mystruct->crossfader != _mystruct->crossfader)
        {
            double crossfader = mystruct->crossfader;

            _mystruct->crossfader = crossfader;
            data->crossfader = crossfader;
            updatedValues++;
        }

        if (updatedValues != 0)
        {
            updatedValues = 0;
            work->data = data;
            uv_async_send(&work->async);
        }
        else
        {
            delete data;
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