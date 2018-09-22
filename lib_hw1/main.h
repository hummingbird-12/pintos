#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"

char str_cmd_list[][20] = {
 "list_insert", "list_spice", "list_push_from", "list_push_back",
 "list_remove", "list_pop_front", "list_pop_back", "list_front", "list_back", 
 "list_size", "list_empty",
 "list_reverse", "list_sort", "list_insert_ordered", "list_unique",
 "list_max", "list_min"
 };

char str_cmd_hash[][20]={
 "hash_insert","hash_replace", "hash_find", "hash_delete",
 "hash_clear", "hash_size", "hash_empty", "hash_apply" 
};

char str_cmd_bitmap[][20]={
 "bitmap_size", "bitmap_set", "bitmap_mark", "bitmap_reset", "bitmap_flip", "bitmap_test",
 "bitmap_set_all", "bitmap_set_multiple", "bitmap_count", "bitmap_contains",
 "bitmap_any", "bitmap_none", "bitmap_all"
};

char str_cmd[][20]={
  "create", "delete", "dumpdata", "quit"
};

char str_ds[][20]={
  "list", "hash", "bitmap"
};

enum COMMAND_TYPE{
  CREATE=0,
  DELETE=1,
  DUMPDATA,
  QUIT
};

enum DATA_STRUCTURE_TYPE{
  LIST=0,
  HASH,
  BITMAP
};




