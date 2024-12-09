var addon = require('bindings')('nativemodule');

var isObsInitialized = addon.ObsInitialized();
if (!isObsInitialized) {

    isObsInitialized = addon.ObsStartup('en-US', '', '');

    if (isObsInitialized) {
        console.log("OBS initialized successfully");
    } else {
        console.log("OBS initialization failed");
    }
}

if (isObsInitialized){

    addon.ObsShutdown();
    console.log("OBS shutdowned successfully");
}

