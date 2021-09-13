#ifndef ROOM_TYPE_H
#define ROOM_TYPE_H

#define NUM_TYPES 7

enum room_type {
    START, // no enemies, anything
    HALLWAY, // rare enemy, chests
    GREAT_HALL, // big room, couple of chests/enemies or one big boi
    DUNGEON, // high risk/reward. powerful enemies and decent loot
    CHEST, // almost free chests. guaranteed 1 chest. some may be locked
    MYSTIC, // tbd. magical items and creatures?
    NON_EXISTENT // no room here.
};

const char *rtype_str(enum room_type rtype);

#endif
