

// Main workflow function
function ObsWork() {

    //JSON object for initializing video context
    const obsVideoInfo = {
        adapter: 0,                         // Use the default adapter
        graphics_module: "libobs-opengl",   // Graphics module (use OpenGL on macOS)
        base_width : 1920,                  // Base (canvas) width
        base_height : 1080,                 // Base (canvas) height
        output_width : 1920,                // Output width
        output_height : 1080,               // Output height
        fps_num : 60,                       // Frames per second numerator
        fps_den : 1,                        // Frames per second denominator
        graphics_thread : true              // Run graphics on a separate thread
    }

    var result = addon.ObsResetVideo(obsVideoInfo)
    if (result != 0) {
        console.log("Failed to initialize video context\n");
        return ;
    }

    //JSON object for initializing audio context 
    const obsAudioInfo = {
        samples_per_sec : 48000, // Audio sample rate
        speakers : 0 //SPEAKERS_STEREO // Output speaker configuration
    }

    result = addon.ObsResetAudio(obsAudioInfo);
    if (result != 0) {
        console.log("Failed to initialize audio context\n");
        return ;
    }

    //Before use load OBS modules
    result = addon.ObsLoadAllModules()
    if (result != 0) {
        console.log("Failed to load OBS modules\n");
        return ;
    }

    const videoEncoder = addon.ObsVideoEncoderCreate();
    if (!videoEncoder) {
        console.log("Failed to create a video encoder\n");
    }

    const audioEncoder = addon.ObsAudioEncoderCreate();
    if (!audioEncoder) {
        console.log("Failed to create an audio encoder\n");
    }

    const twitchService = addon.ObsServiceCreate();
    if (!twitchService) {
        console.log("Failed to create a Twitch service\n");
    }

    const obsOutput = addon.ObsOutputCreate();
    if (!obsOutput) {
        console.log("Failed to create an OBS output\n");
    }

    // Attach encoders and service to output stream
    addon.ObsOutputSetVideoEncoder(obsOutput, videoEncoder);
    addon.ObsOutputSetAudioEncoder(obsOutput, audioEncoder, 0);
    addon.ObsOutputSetService(obsOutput, twitchService);

    result = addon.ObsOutputStart(obsOutput);
    if (result != 0) {
        console.log("Failed to start Obs outputs\n");
    }

    result = addon.ObsOutputStop(obsOutput);
    if (result != 0) {
        console.log("Failed to stop Obs outputs\n");
    }

    //Free resources
    result = addon.ObsOutputRelease(obsOutput);
    result = addon.ObsServiceRelease(twitchService);
    result = addon.ObsEncoderRelease(videoEncoder);
    result = addon.ObsEncoderRelease(audioEncoder);
}


/////////////////////////////////////////////////////////////////////////

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

//Call main obs workflow
ObsWork();


if (isObsInitialized){

    addon.ObsShutdown();
    console.log("OBS shutdowned successfully");
}

