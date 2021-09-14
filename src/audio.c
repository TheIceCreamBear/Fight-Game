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
// = = = = = = = = = = sources = = = = = = = = = = 

// helper linked list
struct srcll {
    ALuint src;
    struct srcll* next;
};

struct srcll* srcllHead = NULL;
struct srcll* srcllTail = NULL;

// adds the given src to the srcll for global cleanup later. returns 1 on success and 0 on failure
int addSrc(ALuint src) {
    struct srcll* srclli = malloc(sizeof(struct srcll));
    if (srclli == NULL) {
        return 0;
    }

    // add source to node
    srclli->src = src;
    srclli->next = NULL;

    // empty list
    if (srcllHead == NULL) {
        srcllHead = srclli;
        srcllTail = srcllHead;
        return 1;
    }

    // non empty list
    srcllTail->next = srclli;
    srcllTail = srcllTail->next;
    return 1;
}

// cleans up all memory used by AL sources
void cleanUpSources(void) {
    struct srcll* srclostpointer = srcllHead;
    while (srcllHead != NULL) {
        // delete the source
        alDeleteSources(1, &srcllHead->src);

        // move pointer, free srclostpointer, update srclostpointer
        srcllHead = srcllHead->next;
        free(srclostpointer);
        srclostpointer = srcllHead;
    }
}

// creates a new AL source and adds it to srcll
ALuint creteSource(void) { // TODO roll off customization
    // generate buffer
    ALuint src;
    alGenSources(1, &src);

    // add to srcll
    addSrc(src);

    return src;
}

// = = = = = = = = = = end sources = = = = = = = = = =
// = = = = = = = = = = buffers and buffer loading = = = = = = = = = =

// helper linked list
struct bufll {
    ALuint buf;
    struct bufll* next;
};

struct bufll* bufllHead = NULL;
struct bufll* bufllTail = NULL;

// adds the given buf to the bufll for global cleanup later. returns 1 on success and 0 on failure
int addBuf(ALuint buf) {
    struct bufll* buflli = malloc(sizeof(struct bufll));
    if (buflli == NULL) {
        return 0;
    }

    // add buffer to node
    buflli->buf = buf;
    buflli->next = NULL;

    // empty list
    if (bufllHead == NULL) {
        bufllHead = buflli;
        bufllTail = bufllHead;
        return 1;
    }

    // non empty list
    bufllTail->next = buflli;
    bufllTail = bufllTail->next;
    return 1;
}

// cleans up all memory used by AL buffers
void cleanUpBuffers(void) {
    struct bufll* buflostpointer = bufllHead;
    while (bufllHead != NULL) {
        // delete the buffer
        alDeleteBuffers(1, &bufllHead->buf);

        // move pointer, free buflostpointer, update buflostpointer
        bufllHead = bufllHead->next;
        free(buflostpointer);
        buflostpointer = bufllHead;
    }
}

// creates a new AL buffer and adds it bufll
ALuint createBuffer(void) { // TODO buffer data
    // generate buffer
    ALuint buf;
    alGenBuffers(1, &buf);
    // add to list
    addBuf(buf);
    
    return buf;
}



// = = = = = = = = = = end buffers and buffer loading = = = = = = = = = =
