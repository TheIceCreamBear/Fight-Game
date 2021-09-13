#ifndef ROOM_H
#define ROOM_H

#include "roomtype.h"

#define ROOM_MAX_DIFFICULTY 10

struct room {
    struct interactable *contents;
    enum room_type rtype;
    struct room *up, *down, *left, *right;
    int difficulty;
};

typedef enum room_type RoomType;
typedef struct room Room;

/* object default function pointers */
void room_init(void *);

/* a room's interactable default function pointer */
void room_interact(void *);

#endif
