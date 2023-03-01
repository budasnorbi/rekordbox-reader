#include <uv.h>
#include <node.h>
#include <node_buffer.h>
#include <chrono> // For std::this_thread::sleep_for
#include <thread>

using namespace v8;

struct MyStruct
{
    double d1ChannelFader;
    double d1CfxKnob;
    double d1LowFilter;
    double d1MidFilter;
    double d1HighFilter;
    double d1TrimKnob;
    double d1Tempo;
    uint32_t d1SongId;
    bool d1IsPlaying;
    double d1CurrentTime;

    double d2ChannelFader;
    double d2CfxKnob;
    double d2LowFilter;
    double d2MidFilter;
    double d2HighFilter;
    double d2TrimKnob;
    double d2Tempo;
    uint32_t d2SongId;
    bool d2IsPlaying;
    double d2CurrentTime;
};

typedef MyStruct *(*DLLFunction1)();

struct Work
{
    uv_work_t request;
    Persistent<Function> callback;
};

void WorkAsync(uv_work_t *req)
{
}

void WorkAsyncComplete(uv_work_t *req, int status)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Work *work = static_cast<Work *>(req->data);

    Local<Function> callback = Local<Function>::New(isolate, work->callback);

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

    MyStruct *_mystruct = new MyStruct();
    int updatedValues = 0;
    while (true)
    {
        MyStruct *mystruct = getData();

        Local<Object> obj = Object::New(isolate);

        if (mystruct->d1ChannelFader != _mystruct->d1ChannelFader)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1ChannelFader").ToLocalChecked(), Number::New(isolate, mystruct->d1ChannelFader));
            _mystruct->d1ChannelFader = mystruct->d1ChannelFader;
            updatedValues++;
        }

        if (mystruct->d1CfxKnob != _mystruct->d1CfxKnob)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1CfxKnob").ToLocalChecked(), Number::New(isolate, mystruct->d1CfxKnob));
            _mystruct->d1CfxKnob = mystruct->d1CfxKnob;
            updatedValues++;
        }

        if (mystruct->d1LowFilter != _mystruct->d1LowFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1LowFilter").ToLocalChecked(), Number::New(isolate, mystruct->d1LowFilter));
            _mystruct->d1LowFilter = mystruct->d1LowFilter;
            updatedValues++;
        }

        if (mystruct->d1MidFilter != _mystruct->d1MidFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1MidFilter").ToLocalChecked(), Number::New(isolate, mystruct->d1MidFilter));
            _mystruct->d1MidFilter = mystruct->d1MidFilter;
            updatedValues++;
        }

        if (mystruct->d1HighFilter != _mystruct->d1HighFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1HighFilter").ToLocalChecked(), Number::New(isolate, mystruct->d1HighFilter));
            _mystruct->d1HighFilter = mystruct->d1HighFilter;
            updatedValues++;
        }

        if (mystruct->d1TrimKnob != _mystruct->d1TrimKnob)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1TrimKnob").ToLocalChecked(), Number::New(isolate, mystruct->d1TrimKnob));
            _mystruct->d1TrimKnob = mystruct->d1TrimKnob;
            updatedValues++;
        }

        if (mystruct->d1Tempo != _mystruct->d1Tempo)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1Tempo").ToLocalChecked(), Number::New(isolate, mystruct->d1Tempo));
            _mystruct->d1Tempo = mystruct->d1Tempo;
            updatedValues++;
        }

        if (mystruct->d1SongId != _mystruct->d1SongId)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1SongId").ToLocalChecked(), Number::New(isolate, mystruct->d1SongId));
            _mystruct->d1SongId = mystruct->d1SongId;
            updatedValues++;
        }

        if (mystruct->d1IsPlaying != _mystruct->d1IsPlaying)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1IsPlaying").ToLocalChecked(), Boolean::New(isolate, mystruct->d1IsPlaying));
            _mystruct->d1IsPlaying = mystruct->d1IsPlaying;
            updatedValues++;
        }

        if (mystruct->d1CurrentTime != _mystruct->d1CurrentTime)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d1CurrentTime").ToLocalChecked(), Number::New(isolate, mystruct->d1CurrentTime));
            _mystruct->d1CurrentTime = mystruct->d1CurrentTime;
            updatedValues++;
        }

        if (mystruct->d2ChannelFader != _mystruct->d2ChannelFader)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2ChannelFader").ToLocalChecked(), Number::New(isolate, mystruct->d2ChannelFader));
            _mystruct->d2ChannelFader = mystruct->d2ChannelFader;
            updatedValues++;
        }

        if (mystruct->d2CfxKnob != _mystruct->d2CfxKnob)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2CfxKnob").ToLocalChecked(), Number::New(isolate, mystruct->d2CfxKnob));
            _mystruct->d2CfxKnob = mystruct->d2CfxKnob;
            updatedValues++;
        }

        if (mystruct->d2LowFilter != _mystruct->d2LowFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2LowFilter").ToLocalChecked(), Number::New(isolate, mystruct->d2LowFilter));
            _mystruct->d2LowFilter = mystruct->d2LowFilter;
            updatedValues++;
        }

        if (mystruct->d2MidFilter != _mystruct->d2MidFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2MidFilter").ToLocalChecked(), Number::New(isolate, mystruct->d2MidFilter));
            _mystruct->d2MidFilter = mystruct->d2MidFilter;
            updatedValues++;
        }

        if (mystruct->d2HighFilter != _mystruct->d2HighFilter)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2HighFilter").ToLocalChecked(), Number::New(isolate, mystruct->d2HighFilter));
            _mystruct->d2HighFilter = mystruct->d2HighFilter;
            updatedValues++;
        }

        if (mystruct->d2TrimKnob != _mystruct->d2TrimKnob)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2TrimKnob").ToLocalChecked(), Number::New(isolate, mystruct->d2TrimKnob));
            _mystruct->d2TrimKnob = mystruct->d2TrimKnob;
            updatedValues++;
        }

        if (mystruct->d2Tempo != _mystruct->d2Tempo)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2Tempo").ToLocalChecked(), Number::New(isolate, mystruct->d2Tempo));
            _mystruct->d2Tempo = mystruct->d2Tempo;
            updatedValues++;
        }

        if (mystruct->d2SongId != _mystruct->d2SongId)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2SongId").ToLocalChecked(), Number::New(isolate, mystruct->d2SongId));
            _mystruct->d2SongId = mystruct->d2SongId;
            updatedValues++;
        }

        if (mystruct->d2IsPlaying != _mystruct->d2IsPlaying)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2IsPlaying").ToLocalChecked(), Boolean::New(isolate, mystruct->d2IsPlaying));
            _mystruct->d2IsPlaying = mystruct->d2IsPlaying;
            updatedValues++;
        }

        if (mystruct->d2CurrentTime != _mystruct->d2CurrentTime)
        {
            obj->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "d2CurrentTime").ToLocalChecked(), Number::New(isolate, mystruct->d2CurrentTime));
            _mystruct->d2CurrentTime = mystruct->d2CurrentTime;
            updatedValues++;
        }

        if (updatedValues != 0)
        {
            Local<Value> argv[1] = {obj};
            callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 1, argv);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            updatedValues = 0;
        }
    }

    FreeLibrary(dllHandle);
    work->callback.Reset();
    delete work;
}

void StartLoop(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // Parse the arguments: callback function and loop condition.
    Local<Function> callback = Local<Function>::Cast(args[0]);
    bool condition = args[1]->BooleanValue(isolate);

    // Create a Work object to pass to the worker thread.
    Work *work = new Work();
    work->request.data = work;
    work->callback.Reset(isolate, callback);

    // Start the worker thread and pass it the Work object.
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);
}

void Init(Local<Object> exports, Local<Object> module)
{
    NODE_SET_METHOD(module, "exports", StartLoop);
}

NODE_MODULE(addon, Init)
