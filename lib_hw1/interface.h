#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LIST 10
#define MAX_HASHTABLE 10
#define MAX_BITMAP 10

#define INPUT_SIZE 101
#define ERROR_CODE -1
#define errorDump(X) printf("[ERROR] %s.\n", X)
#define debugDump(X) printf("[DEBUG] %s.\n", X)

typedef enum {
    ERROR = -1, NONE, LIST, HASHTABLE, BITMAP
} CMD_TYPE;

typedef enum {
    L_ERROR = -1,
    L_CREATE = 0, L_DESTROY,
    L_INSERT, L_SPLICE, L_PUSH_FRONT, L_PUSH_BACK,
    L_REMOVE, L_POP_FRONT, L_POP_BACK, L_FRONT, L_BACK,
    L_SIZE, L_EMPTY,
    L_REVERSE, L_SORT, L_INSERT_ORDERED, L_UNIQUE,
    L_MAX, L_MIN,
    L_SWAP, L_SHUFFLE // new functions
} LIST_FUNC;

typedef enum {
    H_ERROR = -1,
    H_CREATE = 0, H_DESTROY,
    H_INSERT, H_REPLACE, H_FIND, H_DELETE,
    H_CLEAR, H_SIZE, H_EMPTY, H_APPLY,
    H_INT_2 // new function
} HASH_FUNC;

typedef enum {
    B_ERROR = -1,
    B_CREATE = 0, B_DESTROY,
    B_SIZE, B_SET, B_MARK, B_RESET, B_FLIP, B_TEST,
    B_SET_ALL, B_SET_MULTIPLE, B_COUNT, B_CONTAINS,
    B_ANY, B_NONE, B_ALL,
    B_SCAN, B_SCAN_AND_FLIP, B_DUMP,
    B_EXPAND // new function
} BITMAP_FUNC;

void initializer();
int findTargetIndex(CMD_TYPE, char*);
bool inputParser(char*);
void dataDumper(char*);
void dataDestroyer(char*);

void listCommand(char[][INPUT_SIZE], bool);
void hashCommand(char[][INPUT_SIZE], bool);
void bitmapCommand(char[][INPUT_SIZE], bool);
