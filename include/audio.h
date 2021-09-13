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

// = = = = = = = = = = end init = = = = = = = = = = 
// = = = = = = = = = = helper = = = = = = = = = = 

// function to get and print all errors after a call to an AL function, returns 1 if an error was present
int logALErrors(void);

// function to get and print all errors after a call to an ALC function, returns 1 if an error was present
int logALCErrors(void);

// = = = = = = = = = = end helper = = = = = = = = = = 

#endif