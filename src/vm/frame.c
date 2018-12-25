#include <debug.h>

#include "vm/frame.h"
#include "threads/thread.h"
#include "threads/malloc.h"

static FRAME_TABLE frame_table;
/* use hash_bytes() function to hash *UPAGE* */
static unsigned frame_table_hash_func(const struct hash_elem *elem, void *aux UNUSED){
  void *page_addr = hash_entry(elem, FRAME_ENTRY, f_elem)->upage;
  return hash_bytes(&page_addr, sizeof(page_addr));
}

static bool frame_table_less_func(const struct hash_elem *elemA, const struct hash_elem *elemB, void *aux UNUSED){
  return
    hash_entry(elemA, FRAME_ENTRY, f_elem)->upage < hash_entry(elemB, FRAME_ENTRY, f_elem)->upage;
}

void frame_table_init(void){
  lock_init(&frame_table.frame_mutex);
  hash_init(&frame_table.frame_hash, frame_table_hash_func, frame_table_less_func, NULL);
}


void *frame_alloc(enum palloc_flags flags, void *upage){
  void *uframe;
  FRAME_ENTRY *fentry;

  lock_acquire(&frame_table.frame_mutex);
  
  // check if swapping is needed
  if(!(uframe = palloc_get_page(flags))){
    /* swapping to be implemented ...  */
    lock_release(&frame_table.frame_mutex);
    return NULL;
  }

  if(!(fentry = (FRAME_ENTRY*) malloc(sizeof(FRAME_ENTRY)) )){
    lock_release(&frame_table.frame_mutex);
    return NULL;
  }

  //add entry to frame table
  hash_insert(&frame_table.frame_hash, &fentry->f_elem);

  fentry->holder = thread_current();
  fentry->uframe = uframe;
  fentry->upage = upage;

  lock_release(&frame_table.frame_mutex);
  return uframe;


}
