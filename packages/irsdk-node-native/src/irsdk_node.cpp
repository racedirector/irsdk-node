#include "./irsdk_node.h"
#include "../lib/yaml_parser.h"

// ---------------------------
// Constrcutors
// ---------------------------

/**
 * Singleton constructor???
 */
Napi::Object irsdkNode::Init(Napi::Env env, Napi::Object exports)
{
  printf("Hello from irsdkNode::Init\n");
  Napi::Function func = DefineClass(env, "irsdkNode", {InstanceAccessor<&irsdkNode::GetEnableLogging, &irsdkNode::SetEnableLogging>("enableLogging"), InstanceMethod("startSDK", &irsdkNode::StartSdk), InstanceMethod("stopSDK", &irsdkNode::StopSdk), InstanceMethod("waitForData", &irsdkNode::WaitForData), InstanceMethod("getSessionData", &irsdkNode::GetSessionData), InstanceMethod("getTelemetryData", &irsdkNode::GetTelemetryData), InstanceMethod("getTelemetryVariable", &irsdkNode::GetTelemetryVar), InstanceMethod("broadcast", &irsdkNode::BroadcastMessage), InstanceMethod("isRunning", &irsdkNode::IsRunning)});

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("irsdkNode", func);
  return exports;
}

/**
 * Instance constructor
 */
irsdkNode::irsdkNode(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<irsdkNode>(info), m_data(NULL), m_nData(0), m_statusID(0), m_lastSessionCt(-1), m_lastTick(-1), _loggingEnabled(false)
{
  // If logging enabled, log that we're initializing the class
  if (_loggingEnabled)
    printf("Initializing cpp class instance...\n");
}

// ---------------------------
// Properties
// ---------------------------

/**
 * Returns the current value of the logging enabled property
 */
Napi::Value irsdkNode::GetEnableLogging(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), this->_loggingEnabled);
}

/**
 * Sets the logging enabled property. Always logs a message of the next state.
 */
void irsdkNode::SetEnableLogging(const Napi::CallbackInfo &info, const Napi::Value &value)
{
  Napi::Boolean enable;
  if (info.Length() <= 0 || !info[0].IsBoolean())
  {
    enable = Napi::Boolean::New(info.Env(), false);
  }
  else
  {
    enable = info[0].As<Napi::Boolean>();
  }

  this->_loggingEnabled = enable;
}

// ---------------------------
// Methods
// ---------------------------

/**
 * Starts the SDK.
 *
 * If an SDK instance is not running, this method will start it and return
 * the result of `irsdk_startup()`. If an SDK instance is already running,
 * this method will return `true`.
 */
Napi::Value irsdkNode::StartSdk(const Napi::CallbackInfo &info)
{
  if (!irsdk_isConnected())
  {
    bool result = irsdk_startup();
    if (_loggingEnabled)
    {
      printf("Startup result: %i\n", result);
    }

    return Napi::Boolean::New(info.Env(), result);
  }

  return Napi::Boolean::New(info.Env(), true);
}

/**
 * Stops the SDK
 */
Napi::Value irsdkNode::StopSdk(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), false);
}

Napi::Value irsdkNode::WaitForData(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), false);
}

Napi::Value irsdkNode::GetSessionData(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), "");
}

Napi::Value irsdkNode::GetTelemetryData(const Napi::CallbackInfo &info)
{
  return Napi::String::New(info.Env(), "");
}

Napi::Value irsdkNode::GetTelemetryVar(const Napi::CallbackInfo &info)
{
  return Napi::Object::New(info.Env());
}

/**
 * Broadcasts a message
 */
Napi::Value irsdkNode::BroadcastMessage(const Napi::CallbackInfo &info)
{
  return Napi::Boolean::New(info.Env(), false);
}

/**
 * Returns whether the SDK is running
 */
Napi::Value irsdkNode::IsRunning(const Napi::CallbackInfo &info)
{
  bool sdkConnected = irsdk_isConnected();
  // TODO: Check if we have data
  return Napi::Boolean::New(info.Env(), sdkConnected);
}