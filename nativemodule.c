#include <assert.h>
#include <node_api.h>
#include "3rdparty/src/libobs/obs.h"


//OBS API marshaling

static napi_value ObsInitialized(napi_env env, napi_callback_info info) {

    napi_value result;
    bool obsInitialized = obs_initialized();
    napi_status status = napi_get_boolean(env, obsInitialized, &result);

    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create boolean value");
        return NULL;
    }

    return result;
}

static napi_value ObsStartup(napi_env env, napi_callback_info info) {

    napi_value result;
    size_t argc = 3;
    napi_value args[3];

    // Get the arguments passed to the function
    napi_status status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    if (status != napi_ok) {
      return NULL;
    }

    // Check if exactly 3 arguments are passed
    if (argc != 3) {
        napi_throw_type_error(env, NULL, "Expected 3 arguments");
        return NULL;
    }

    //Marshaling arguments from NodeJS to C++
    char locale[1024], moduleConfigPath[1024], store[1024];
    size_t argumentLength = 0;

    // Get locale argument
    status = napi_get_value_string_utf8(env, args[0], locale, sizeof(locale), &argumentLength);
    if (status != napi_ok)
      return NULL;

    // Get moduleConfigPath argument
    status = napi_get_value_string_utf8(env, args[1], moduleConfigPath, sizeof(moduleConfigPath), &argumentLength);
    if (status != napi_ok)
      return NULL;

    // Get store name argument
    status = napi_get_value_string_utf8(env, args[2], store, sizeof(store), &argumentLength);
    if (status != napi_ok)
      return NULL;


    // TODO: Link with libobsutil for the realization of profiler_name_store_t
    // profiler_name_store_t *storeName = profiler_name_store_create();
    // if (!storeName) {
    //     napi_throw_error(env, NULL, "Memory allocation failed");
    //     return NULL;
    // }
    // profile_store_name(storeName, "%s", store);

    //Finally, call OBS initialization
    bool obsInitialized = obs_startup(locale, moduleConfigPath, NULL);
    //bool obsInitialized = obs_startup("", "", NULL);
    status = napi_get_boolean(env, obsInitialized, &result);

    if (status != napi_ok) {
        //profiler_name_store_free(storeName);
        napi_throw_error(env, NULL, "Failed to create boolean value");
        return NULL;
    }

    //profiler_name_store_free(storeName);

    return result;
}

//API: shutdown OBS
static napi_value ObsShutdown(napi_env env, napi_callback_info info) {

    obs_shutdown();

    return NULL;
}

static napi_value ObsResetVideo(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsResetAudio(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsLoadAllModules(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputCreate(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputRelease(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputSetVideoEncoder(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputSetAudioEncoder(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputSetService(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputStart(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsOutputStop(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsVideoEncoderCreate(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsAudioEncoderCreate(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsEncoderRelease(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsServiceCreate(napi_env env, napi_callback_info info)
{
  return NULL;
}

static napi_value ObsServiceRelease(napi_env env, napi_callback_info info)
{
  return NULL;
}


// -----------------------------------------------------------------------------
// Declare all exports for NodeJs

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc[] = {
    DECLARE_NAPI_METHOD("ObsInitialized", ObsInitialized),
    DECLARE_NAPI_METHOD("ObsStartup", ObsStartup),
    DECLARE_NAPI_METHOD("ObsShutdown", ObsShutdown),

    DECLARE_NAPI_METHOD("ObsResetVideo", ObsResetVideo),
    DECLARE_NAPI_METHOD("ObsResetAudio", ObsResetAudio),
    DECLARE_NAPI_METHOD("ObsLoadAllModules", ObsLoadAllModules),

    DECLARE_NAPI_METHOD("ObsOutputCreate", ObsOutputCreate),
    DECLARE_NAPI_METHOD("ObsOutputRelease", ObsOutputRelease),
    DECLARE_NAPI_METHOD("ObsOutputStart", ObsOutputStart),
    DECLARE_NAPI_METHOD("ObsOutputStop", ObsOutputStop),
    DECLARE_NAPI_METHOD("ObsOutputSetVideoEncoder", ObsOutputSetVideoEncoder),
    DECLARE_NAPI_METHOD("ObsOutputSetAudioEncoder", ObsOutputSetAudioEncoder),
    DECLARE_NAPI_METHOD("ObsOutputSetService", ObsOutputSetService),

    DECLARE_NAPI_METHOD("ObsVideoEncoderCreate", ObsVideoEncoderCreate),
    DECLARE_NAPI_METHOD("ObsAudioEncoderCreate", ObsAudioEncoderCreate),
    DECLARE_NAPI_METHOD("ObsEncoderRelease", ObsEncoderRelease),

    DECLARE_NAPI_METHOD("ObsServiceCreate", ObsServiceCreate),
    DECLARE_NAPI_METHOD("ObsServiceRelease", ObsServiceRelease)
  };

  status = napi_define_properties(env, exports, 18, desc);
  assert(status == napi_ok);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

