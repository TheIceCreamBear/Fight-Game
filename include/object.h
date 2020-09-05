#ifndef OBJECT_H
#define OBJECT_H

// XXX - this is in heavy need of renaming to avoid confusion!

struct object {
    void (*init)(void *);
    void (*destroy)(void *);
};

void object_default_init(void *);
void object_default_destroy(void *);

#endif
