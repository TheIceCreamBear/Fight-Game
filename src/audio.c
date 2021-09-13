#include "audio.h"
#include "logging.h"

// inits the audio system, returns 1 on succsess and 0 on failure of any kind
int audioInit(void) {
    ALCboolean enumerateAll = alcIsExtensionPresent(NULL, "ALC_enumerate_all_EXT");
    if (enumerateAll) {
        fprintf(stderr, "OpenAL Info: \n%s\n", alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
    } else {
        fprintf(stderr, "numeration extensinon not found\n");
    }
    // alcOpenDevice

    return 1;
}