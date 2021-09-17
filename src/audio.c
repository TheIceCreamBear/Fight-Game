#include "audio.h"
#include "logging.h"
#include "utils.h"

#include <string.h>

#ifdef DEBUG
#define AUDIO_DEBUG
#endif

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

// cleans up all AL and ALC obejects
void cleanUpAllAL(void) {
    // clean up buffers and sources
    cleanUpBuffers();
    cleanUpSources();

    // set no context
    alcMakeContextCurrent(NULL);

    // close context and destroy device
    alcDestroyContext(context);
    alcCloseDevice(device);
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
        // make sure the source isnt playing
        alSourceStop(srcllHead->src);

        // delete the source
        alDeleteSources(1, &srcllHead->src);

        // move pointer, free srclostpointer, update srclostpointer
        srcllHead = srcllHead->next;
        free(srclostpointer);
        srclostpointer = srcllHead;
    }
}

// creates a new AL source and adds it to srcll
ALuint createSource(void) { // TODO roll off customization
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

// creates a new AL buffer for the given audio data struct
ALuint createBufferFromData(AudioFormat* data) {
    // create the buffer
    ALuint buffer = createBuffer();

    ALenum format = 0;
    if (data->channels == 1) {
        if (data->bitsPerSample == 8) {
            format = AL_FORMAT_MONO8;
        } else if (data->bitsPerSample == 16) {
            format = AL_FORMAT_MONO16;
        } else {
            // ERROR
            return 0;
        }
    } else if (data->channels == 2) {
        if (data->bitsPerSample == 8) {
            format = AL_FORMAT_STEREO8;
        } else if (data->bitsPerSample == 16) {
            format = AL_FORMAT_STEREO16;
        } else {
            // ERROR
            return 0;
        }
    } else {
        // ERROR
        return 0;
    }

    // put data in buffer
    alBufferData(buffer, format, data->data, data->dataSizeBytes, data->sampleRate);

    return buffer;
}

// reads in the file described by the given string and returns the audio format of it
AudioFormat* readWavAudioData(char* file) {
    // create space for format data
    AudioFormat* format = malloc(sizeof(AudioFormat));
    if (format == NULL) {
        fprintf(stderr, FATAL "Could not create space for audio format: %s\n" ANSI_RESET, file);
        return NULL;
    }

    // open the file
    FILE* f = fopen(file, "r");
    if (f == NULL) {
        fprintf(stderr, FATAL "Could not open audio file %s\n" ANSI_RESET, file);
        free(format);
        return NULL;
    }

    // this is here so we dont need to have a \0 as part of the cbuf, so priting is pretty
    char stackManipulation = '\0';
    // setup data we will need
    char cbuf4[4];
    unsigned char buf4[4];
    unsigned char buf2[2];
    int read = 0;
    int overallSize = 0;
    int fmtLength = 0;

    // read headder
    read = fread(cbuf4, sizeof(cbuf4), 1, f);
    if (read != 1 || strincmp(cbuf4, "riff", 4) != 0) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nMissing riff %s\n" ANSI_RESET, file, cbuf4);
        free(format);
        fclose(f);
        return NULL;
    }
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(1-4): %s\n" ANSI_RESET, cbuf4);
    #endif

    // read overall size
    read = fread(buf4, sizeof(buf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nSize incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    overallSize = buf4[0] | (buf4[1] << 8) | (buf4[2] << 16) | (buf4[3] << 24);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(5-8): %d\n" ANSI_RESET, overallSize);
    #endif

    // read wave file part
    read = fread(cbuf4, sizeof(buf4), 1, f);
    if (read != 1 || strincmp(cbuf4, "wave", 4) != 0) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nMissing wave %s\n" ANSI_RESET, file, cbuf4);
        free(format);
        fclose(f);
        return NULL;
    }
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(9-12): %s\n" ANSI_RESET, cbuf4);
    #endif

    // read fmt\0 
    read = fread(cbuf4, sizeof(cbuf4), 1, f);
    if (read != 1 || strincmp(cbuf4, "fmt", 3) != 0) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nMissing fmt %s" ANSI_RESET, file, cbuf4);
        free(format);
        fclose(f);
        return NULL;
    }
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(13-16): %s\n" ANSI_RESET, cbuf4);
    #endif

    // read format header size
    read = fread(buf4, sizeof(buf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nSize incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    fmtLength = buf4[0] | (buf4[1] << 8) | (buf4[2] << 16) | (buf4[3] << 24);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(17-20): %d\n" ANSI_RESET, fmtLength);
    #endif

    // read format type
    read = fread(buf2, sizeof(buf2), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nFormat type incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->formatType = buf2[0] | (buf2[1] << 8);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(21-22): %d\n" ANSI_RESET, format->formatType);
    #endif

    // read channels
    read = fread(buf2, sizeof(buf2), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nChannels incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->channels = buf2[0] | (buf2[1] << 8);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(23-24): %d\n" ANSI_RESET, format->channels);
    #endif

    // read sample rate
    read = fread(buf4, sizeof(buf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nSample rate incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->sampleRate = buf4[0] | (buf4[1] << 8) | (buf4[2] << 16) | (buf4[3] << 24);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(25-28): %d\n" ANSI_RESET, format->sampleRate);
    #endif

    // read byte rate
    read = fread(buf4, sizeof(buf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nbyte rate incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->byteRate = buf4[0] | (buf4[1] << 8) | (buf4[2] << 16) | (buf4[3] << 24);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(29-32): %d\n" ANSI_RESET, format->byteRate);
    #endif

    // read block align
    read = fread(buf2, sizeof(buf2), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nblock align incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->blockAllign = buf2[0] | (buf2[1] << 8);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(33-34): %d\n" ANSI_RESET, format->blockAllign);
    #endif

    // read bits per sample
    read = fread(buf2, sizeof(buf2), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nbits per sample incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->bitsPerSample = buf2[0] | (buf2[1] << 8);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(35-36): %d\n" ANSI_RESET, format->bitsPerSample);
    #endif

    // read data or flir
    read = fread(cbuf4, sizeof(cbuf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\nbits per sample incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(37-40): %s\n" ANSI_RESET, cbuf4);
    #endif

    // read data size in samples
    read = fread(buf4, sizeof(buf4), 1, f);
    if (read != 1) {
        fprintf(stderr, FATAL "Incorrect file format for %s\ndata size incorrect\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }
    format->dataSizeBytes = buf4[0] | (buf4[1] << 8) | (buf4[2] << 16) | (buf4[3] << 24);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(41-44): %d\n" ANSI_RESET, format->dataSizeBytes);
    #endif
    
    // calculate data size in terms of samples
    format->dataSizeSamples = (8 * format->dataSizeBytes) / (format->channels * format->bitsPerSample);
    // debug
    #ifdef AUDIO_DEBUG
        fprintf(stdout, WARN "(size samples): %d\n" ANSI_RESET, format->dataSizeSamples);
    #endif


    // THIS IS WHERE THE DATA ACTUALLY GETS READ IN
    if (format->formatType != 1) {
        fprintf(stderr, FATAL "Unsupported storage type for audio file %s\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }

    // alocate space for data
    format->data = malloc(format->dataSizeBytes);
    if (format->data == NULL) {
        fprintf(stderr, FATAL "Buffer allocation failed for audio file %s\n" ANSI_RESET, file);
        free(format);
        fclose(f);
        return NULL;
    }

    int sampleSize = (format->bitsPerSample * format->channels) / 8;
    if (sampleSize % format->channels != 0) {
        fprintf(stderr, FATAL "Audio file %s headder invalid\n" ANSI_RESET, file);
        freeAudioFormat(format);
        fclose(f);
        return NULL;
    }

    char sampleBuffer[sampleSize];
    int bytesPerChannel = (sampleSize) / format->channels;
    int dataOffset = 0;
    for (int i = 0; i < format->dataSizeSamples; i++) {
        // read the sample in
        read = fread(sampleBuffer, sizeof(sampleBuffer), 1, f);

        // if nothing was read in
        if (read != 1) {
            fprintf(stderr, FATAL "Audio file %s data sample %d/%d invalid\n" ANSI_RESET, file, i, format->dataSizeSamples);
            freeAudioFormat(format);
            fclose(f);
            return NULL;
        }

        // reverse endianess of the sample
        for (int j = 0; j < format->channels; j++) {
            for (int k = bytesPerChannel - 1; k >= 0; k--) {
                format->data[dataOffset++] = sampleBuffer[j * bytesPerChannel + k];

                // check bounds (before the ++ ran) to not make anything go wack
                if (dataOffset - 1 >= format->dataSizeBytes) {
                    fprintf(stderr, FATAL "Audio file %s data sample %d/%d exceeds dataSizeBytes: %d > %d\n" ANSI_RESET, file, i, format->dataSizeSamples, dataOffset, format->dataSizeBytes);
                    freeAudioFormat(format);
                    fclose(f);
                    return NULL;
                }
            }
        }
    }

    // if audio debug, print out the samples
    #ifdef AUDIO_DEBUG
    for (int i = 0; i < format->dataSizeSamples; i++) {
        int baseDataIndex = i * format->blockAllign;
        for (int j = 0; j < format->channels; j++) {
            long sample = 0;
            for (int k = 0; k < bytesPerChannel; k++) {
                char byte = format->data[baseDataIndex + j * bytesPerChannel + k] & 0x00ff;
                if (bytesPerChannel == 1) {
                    // bytes in 8 bit wav files are unsigned, so switch this to signed
                    byte -= 128;
                }
                int shift = powi(8, bytesPerChannel - 1 - k);
                sample |= byte << (shift == 1 ? 0 : shift);
            }
            fprintf(stderr, WARN "Sample %d: channel %d: %ld\n", i + 1, j + 1, sample);
        }
    }
    #endif

    return format;
}

// function to clean up an AudioFormat struct
void freeAudioFormat(AudioFormat* format) {
    free(format->data);
    free(format);
}

// = = = = = = = = = = end buffers and buffer loading = = = = = = = = = =
