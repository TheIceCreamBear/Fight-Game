#include "roomtype.h"

#include "utils.h"

const char *rtype_str(enum __room_type rtype) {
    switch (rtype) {
        case START:
            return "START";
            break;
        case HALLWAY:
            return "HALLWAY";
            break;
        case DUNGEON:
            return "DUNGEON";
            break;
        case CHEST:
            return "CHEST";
            break;
        case MYSTIC:
            return "MYSTIC";
            break;
        case NON_EXISTENT:
            return "NON_EXISTENT";
            break;
        default:
            return "INVALID_ROOM_TYPE";
    }
}

