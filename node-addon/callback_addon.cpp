#include <uv.h>
#include <node.h>
#include <node_buffer.h>
#include <chrono> // For std::this_thread::sleep_for
#include <thread>

using namespace v8;

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

    // The callback function is invoked in the main thread.
    Local<Function> callback = Local<Function>::New(isolate, work->callback);
    // Pass the buffer object to the callback function.
    size_t buffer_length = 4;
    char *buffer_data = "test";
    Local<Value> argv[1] = {node::Buffer::New(isolate, buffer_data, buffer_length).ToLocalChecked()};

    while (true)
    {
        callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 1, argv);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Clean up the persistent reference to the callback function.
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
