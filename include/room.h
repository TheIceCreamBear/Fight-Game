#ifndef ROOM_H
#define ROOM_H

#include "object.h"
#include "interactable.h"
#include "roomtype.h"
#include "types.h"

#define MAX_ROOM_DIFFICULTY 10

struct room {
    struct object obj;
    struct interactable *contents;

    struct room *up, *down, *left, *right;
    RoomType rtype;
    int difficulty;
};

/* object default function pointers */
void __room_init(void *);
void __room_destroy(void *);

/* a room's interactable default function pointer */
void __room_interact(void *);

#endif
