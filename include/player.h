#ifndef PLAYER_H
#define PLAYER_H

#include "room.h"
#include "utils.h"

struct player {
    int health;
    union {
        struct room *cur_roomp;
        struct location *cur_room;
    } room; // TODO rename

    /* TODO struct item *inventory */
};

#endif
