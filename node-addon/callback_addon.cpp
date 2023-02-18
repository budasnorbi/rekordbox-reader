#define NAPI_DISABLE_CPP_EXCEPTIONS
#include <napi.h>
#include <uv.h>
#include <chrono>
#include <thread>

using namespace Napi;

void RunForever(const CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // Create a new ThreadSafeFunction to the JavaScript callback
    Napi::ThreadSafeFunction tsfn = Napi::ThreadSafeFunction::New(
        env,
        info[0].As<Function>(),
        "RunForever",
        0,
        1);

    // Create a new uv_thread_t object to start the worker thread
    auto worker = new uv_thread_t;

    // Start the worker thread and pass the ThreadSafeFunction to it
    uv_thread_create(
        worker, [](void *arg)
        {
    auto tsfn = static_cast<Napi::ThreadSafeFunction*>(arg);

    // Create a while loop that runs forever
    while (true) {
      // Call the JavaScript callback in a thread-safe way
      tsfn->NonBlockingCall(
          [tsfn](Napi::Env env, Napi::Function jsCallback) {
            // Call the JavaScript callback with the Buffer
            jsCallback.Call({Napi::Buffer<char>::New(env, "Hello world!", 12)});
          });

      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } },
        &tsfn);

    // Detach the worker thread to allow it to continue running after the main thread exits
    // #ifdef _WIN32
    //     uv_thread_join(worker);
    // #endif
}

Object Init(Env env, Object exports)
{
    exports.Set("runForever", Function::New(env, RunForever));
    return exports;
}

NODE_API_MODULE(addon, Init)