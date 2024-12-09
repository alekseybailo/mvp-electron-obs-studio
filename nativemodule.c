#include <assert.h>
#include <node_api.h>
#include "3rdparty/src/libobs/obs.h"


//API: check if OBS initialized
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

//API: initialize OBS
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

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc[] = {
    DECLARE_NAPI_METHOD("ObsInitialized", ObsInitialized),
    DECLARE_NAPI_METHOD("ObsStartup", ObsStartup),
    DECLARE_NAPI_METHOD("ObsShutdown", ObsShutdown),
  };

  status = napi_define_properties(env, exports, 3, desc);
  assert(status == napi_ok);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

