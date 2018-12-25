#include <debug.h>

#include "vm/frame.h"
#include "threads/thread.h"
#include "threads/malloc.h"

static FRAME_TABLE frame_table;

/* use hash_bytes() function to hash *UPAGE */
static unsigned frame_table_hash_func(const struct hash_elem *elem, void *aux UNUSED) {
    void *page_addr = hash_entry(elem, FRAME_ENTRY, f_elem)->upage;
    return hash_bytes(&page_addr, sizeof(page_addr));
}

static bool frame_table_less_func(const struct hash_elem *elemA, const struct hash_elem *elemB, void *aux UNUSED) {
    return
        hash_entry(elemA, FRAME_ENTRY, f_elem)->upage < 
        hash_entry(elemB, FRAME_ENTRY, f_elem)->upage;
}

/* Initialization for frame table */
void frame_table_init (void) {
    lock_init(&frame_table.frame_mutex);
    hash_init(&frame_table.frame_hash, frame_table_hash_func, frame_table_less_func, NULL);
}

/* Allocate a frame for user page UPAGE */
void *frame_alloc (enum palloc_flags flags, void *upage) {
    void *uframe;
    FRAME_ENTRY *fentry;

    lock_acquire(&frame_table.frame_mutex);

    // check if swapping is needed
    if(!(uframe = palloc_get_page(PAL_USER | flags))) {
        /* SWAPPING TO BE IMPLEMENTED... */
        lock_release(&frame_table.frame_mutex);
        return NULL;
    }

    // create entry for frame table
    if(!(fentry = (FRAME_ENTRY*) malloc(sizeof(FRAME_ENTRY)))) {
        lock_release(&frame_table.frame_mutex);
        return NULL;
    }

    // add entry to frame table
    hash_insert(&frame_table.frame_hash, &fentry->f_elem);

    fentry->holder = thread_current();
    fentry->uframe = uframe;
    fentry->upage = upage;

    lock_release(&frame_table.frame_mutex);
    return uframe;
}

void frame_free (void *page_addr) {
    FRAME_ENTRY *fentry;
    lock_acquire(&frame_table.frame_mutex);

    if(!(fentry = frame_lookup(page_addr)))
        return;

    hash_delete(&frame_table.frame_hash, &fentry->f_elem);
    palloc_free_page(fentry->uframe);
    free(fentry);

    lock_release(&frame_table.frame_mutex);
}

FRAME_ENTRY *frame_lookup(void *page_addr) {
    struct hash_iterator frame_it;

    hash_first(&frame_it, &frame_table.frame_hash);
    while(hash_next(&frame_it)) {
        FRAME_ENTRY *fentry = hash_entry(hash_cur(&frame_it), FRAME_ENTRY, f_elem);
        if(page_addr == fentry->upage)
            return fentry;
    }
    return NULL;
}

