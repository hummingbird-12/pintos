#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#define COMMAND_MAX_SIZE 100
#define PARA_SIZE 5
#define LIST_NUM 17
#define HASH_NUM 8
#define BITMAP_NUM 13
#define ETC_NUM 7


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

char str_cmd_etc[][20]={
  "create", "delete", "dumpdata", "quit"
};

char str_ds[][20]={
  "list", "hash", "bitmap"
};


enum _CMD_LIST{
  LIST_INSERT, LIST_SPICE, LIST_PUSH_FROM, LIST_PUSH_BACK,
  LIST_REMOVE, LIST_POP_FRONT, LIST_POP_BACK, LIST_FRONT, LIST_BACK,
  LIST_SIZE, LIST_EMPTY,
  LIST_REVERSE, LIST_SORT, LIST_INSERT_ORDERED, LIST_UNIQUE,
  LIST_MAX, LIST_MIN
};

enum _CMD_HASH{
  HASH_INSERT,HASH_REPLACE, HASH_FIND, HASH_DELETE,
  HASH_CLEAR, HASH_SIZE, HASH_EMPTY, HASH_APPLY
};

enum _CMD_BITMAP{
  BITMAP_SIZE, BITMAP_SET, BITMAP_MARK, BITMAP_RESET, BITMAP_FLIP, BITMAP_TEST,
  BITMAP_SET_ALL, BITMAP_SET_MULTIPLE, BITMAP_COUNT, BITMAP_CONTAINS,
  BITMAP_ANY, BITMAP_NONE, BITMAP_ALL
};

enum _CMD_ETC{
  CREATE=0,DELETE=1,DUMPDATA,QUIT
};

enum _DS{
  LIST=0,HASH,BITMAP
};




void command_input(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
void command_init(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
int command_process(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
void etc_process(int cmd, char ds[], char para[][COMMAND_MAX_SIZE]);



