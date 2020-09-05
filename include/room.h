#ifndef ROOM_H
#define ROOM_H

#include "object.h"
#include "interactable.h"
#include "types.h"

struct room {
    struct object obj;
    struct interactable *contents;
    RoomType rtype;
    int difficulty;
};

/* object default function pointers */
void __room_init(void *);
void __room_destroy(void *);

/* a room's interactable default function pointer */
void __room_interact(void *);

#endif
