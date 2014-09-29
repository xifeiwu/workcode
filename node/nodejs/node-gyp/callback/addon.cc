#include <node.h>

using namespace v8;

Handle<Value> RunCallback(const Arguments& args) {
  HandleScope scope;

  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Local<Value>::New(String::New("hello world")) };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);

  return scope.Close(Undefined());
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(addon, Init)



// #include <node.h>

// using namespace v8;

// void RunCallback(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = Isolate::GetCurrent();
//   HandleScope scope(isolate);

//   Local<Function> cb = Local<Function>::Cast(args[0]);
//   const unsigned argc = 1;
//   Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };
//   cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
// }

// void Init(Handle<Object> exports, Handle<Object> module) {
//   NODE_SET_METHOD(module, "exports", RunCallback);
// }

// NODE_MODULE(addon, Init)