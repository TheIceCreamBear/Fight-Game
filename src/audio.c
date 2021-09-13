#include "audio.h"
#include "logging.h"
#include "utils.h"

// = = = = = = = = = = init = = = = = = = = = =

// the aldevice currently being used
ALCdevice* device; // TODO extern??? (shouldnt need it but maybe)

// the current alcontext tied to the current aldevice
ALCcontext* context;

// flag representing weather or not we are using thread local contexts
// TODO thread local contexts

// inits the audio system, returns 1 on succsess and 0 on failure of any kind
int audioInit(void) {
    // get the system default device
    device = alcOpenDevice(NULL);

    // if there is no device (somehow, i wouldn't put it past apple or something), failure
    if (device == NULL) {
        logs(FATAL "No OpenAL device available\n" ANSI_RESET);
        return 0;
    }

    // create the alcontext for the current device
    context = alcCreateContext(device, NULL);

    // also shouldnt be possible but lets not make something really weird happen
    if (context == NULL) {
        const ALCchar* deviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);
        fprintf(log_output_stream, FATAL "Unable to get context for the current device (%s)\n" ANSI_RESET, deviceName);
        return 0;
    }

    // setup thread local context
    // TODO determine if the above is needed, if not just keep using a process wide context
    if (alIsExtensionPresent("ALC_EXT_thread_local_context")) {
        logs(INFO "The thread local context is available for use\n" ANSI_RESET);
    }

    // make the context we generated current. we should not do this if we are able to use thread local contexts (tho the reason for using them doesnt fully make sense in this context)
    if (!alcMakeContextCurrent(context)) {
        // an error occured
        logALCErrors();
        return 0;
    }

    return 1;
}

// = = = = = = = = = = end init = = = = = = = = = = 
// = = = = = = = = = = helper = = = = = = = = = = 

// function to get and print all errors after a call to an AL function, returns 1 if an error was present
int logALErrors(void) {
    // get the first error
    ALenum error = alGetError();

    // quick check if an error was present
    if (error == AL_NO_ERROR) {
        return 0;
    }

    // loop all errors
    while (error != AL_NO_ERROR) {
        fprintf(log_output_stream, WARN "AL Error occured\n%s" ANSI_RESET, alGetString(error));
    }
    return 1;
}

// function to get and print all errors after a call to an ALC function, returns 1 if an error was present
int logALCErrors(void) {
    // get the first error
    ALCenum error = alcGetError(device);

    // quick check if an error was present
    if (error == ALC_NO_ERROR) {
        return 0;
    }

    // loop all errors
    while (error != ALC_NO_ERROR) {
        fprintf(log_output_stream, WARN "ALC Error occured\n%s" ANSI_RESET, alcGetString(device, error));
    }
    return 1;
}

// = = = = = = = = = = end helper = = = = = = = = = = 