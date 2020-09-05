#ifndef INTERACTABLE_H
#define INTERACTABLE_H

struct interactable {
    void (*iteract)(void *);
};

#endif
