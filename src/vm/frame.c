#include <hash.h>
#include <debug.h>

#include "threads/synch.h"
#include "vm/frame.h"

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

void init_frame_table (void) {
    lock_init(&frame_table.frame_mutex);
    hash_init(&frame_table.frame_hash, frame_table_hash_func, frame_table_less_func, NULL);
}

