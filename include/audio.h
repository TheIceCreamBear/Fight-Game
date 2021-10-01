#ifndef AUDIO_H
#define AUDIO_H

#ifdef __APPLE__
// this is seperate because vscode and just making sure to include the macos extra "goodies"
#include <OpenAL/OpenAL.h>
#else
// per https://cmake.org/cmake/help/latest/module/FindOpenAL.html
// this line will include the openal header
#include "al.h"
#include "alc.h"
#endif


// = = = = = = = = = = init = = = = = = = = = = 

// inits the audio system, returns 1 on succsess and 0 on failure of any kind
int audioInit(void);

// cleans up all AL and ALC obejects
void cleanUpAllAL(void);

// = = = = = = = = = = end init = = = = = = = = = = 
// = = = = = = = = = = helper = = = = = = = = = = 

// function to get and print all errors after a call to an AL function, returns 1 if an error was present
int logALErrors(void);

// function to get and print all errors after a call to an ALC function, returns 1 if an error was present
int logALCErrors(void);

// = = = = = = = = = = end helper = = = = = = = = = = 
// = = = = = = = = = = sources = = = = = = = = = = 

// cleans up all memory used by AL sources
void cleanUpSources(void);

// creates a new AL source and adds it to srcll
ALuint createSource(void); // TODO roll off customization

// = = = = = = = = = = end sources = = = = = = = = = =
// = = = = = = = = = = buffers and buffer loading = = = = = = = = = =

// struct to store any and all info about an audio file
typedef struct AudioFormat {
    unsigned int formatType; // 1 - PCM, 3 - IEEE Float
    unsigned int channels; // 1 - mono, 2 - stereo
    unsigned int sampleRate; // number of samples per second
    unsigned int byteRate; // number of bytes per second
    unsigned int blockAllign; // bytes per sample for all channels
    unsigned int bitsPerSample; // bits per sample for one channel
    unsigned int dataSizeSamples; // the size of the data in terms of unique samples
    unsigned int dataSizeBytes; // the size of the data in bytes
    unsigned char* data; 
} AudioFormat;

// cleans up all memory used by AL buffers
void cleanUpBuffers(void);

// creates a new AL buffer and adds it bufll
ALuint createBuffer(void); // TODO buffer data

// creates a new AL buffer for the given audio data struct
ALuint createBufferFromData(AudioFormat* format);

// reads in the file described by the given string and returns the audio format of it
AudioFormat* readWavAudioData(char* file);

// function to clean up an AudioFormat struct
void freeAudioFormat(AudioFormat* format);

// = = = = = = = = = = end buffers and buffer loading = = = = = = = = = =

#endif