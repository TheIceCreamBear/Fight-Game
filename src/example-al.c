#include "example-al.h"

void simpleALExample(void) {
    // al distance model
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    // create buffer
    AudioFormat* format = readWavAudioData("res/audio/bounce.wav");
    ALuint buffer = createBufferFromData(format);

    // create source
    ALuint source = createSource();
    alSourcef(source, AL_ROLLOFF_FACTOR, 6.0f);
    alSourcef(source, AL_REFERENCE_DISTANCE, 6.0f);
    alSourcef(source, AL_MAX_DISTANCE, 50.0f);

    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);

    // play
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);
}