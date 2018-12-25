#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <hash.h>
#include "threads/synch.h"
#include "threads/palloc.h"

typedef struct _FRAME_TABLE {
    struct hash frame_hash;     /* hash of frame table entries */
    struct lock frame_mutex;    /* mutex lock for frame table */
} FRAME_TABLE;

typedef struct _FRAME_ENTRY {
    struct hash_elem f_elem;    /* hash_elem for frame table */
    struct thread *holder;      /* thread holding the frame */
    void *uframe;               /* frame address (physical address) */
    void *upage;                /* user page address (virtual address) */
} FRAME_ENTRY;

void frame_table_init(void);
void *frame_alloc(enum palloc_flags flags, void *upage);

#endif /* vm/frame.h */
