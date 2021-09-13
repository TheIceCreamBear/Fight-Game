#ifndef AUDIO_H
#define AUDIO_H

// per https://cmake.org/cmake/help/latest/module/FindOpenAL.html
// this line will include the openal header
#ifdef __APPLE__
#include <OpenAL/OpenAL.h>
#else
#include "al.h"
#include "alc.h"
#endif


// = = = = = = = = = = init = = = = = = = = = = 

// inits the audio system, returns 1 on succsess and 0 on failure of any kind
int init(void);

// = = = = = = = = = = end init = = = = = = = = = = 

#endif