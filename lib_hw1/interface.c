#include "interface.h"
//#include "bitmap.h"
//#include "debug.h"
//#include "hash.h"
//#include "limits.h"
#include "list.h"
//#include "round.h"

void *listFunc[] = {
    &list_init,
    &list_insert, &list_splice, &list_push_front, &list_push_back,
    &list_remove, &list_pop_front, &list_pop_back, &list_front, &list_back,
    &list_size, &list_empty,
    &list_reverse, &list_sort, &list_insert_ordered, &list_unique,
    &list_max, &list_min,
    &list_swap, &list_shuffle
};
struct list listArray[MAX_LIST];
char listName[MAX_LIST][INPUT_SIZE];
int listCount;

int main() {
    char str[INPUT_SIZE];
    listCount = 0;

    while(true) {
        memset(str, '\0', INPUT_SIZE);
        fgets(str, sizeof(str), stdin);
        inputParser(str);
    }
    return 0;
}

void inputParser(char* input) {
    char delim[] = " _\n";
    char* cmd;
    bool createFlag = false;

    assert(input != NULL);

    cmd = strtok(input, delim);
    assert(cmd != NULL);

    if(!strcmp(cmd, "create")) {
        cmd = strtok(input, NULL);
        assert(cmd != NULL);
        createFlag = true;
    }

    if(!strcmp(cmd, "list"))
        listCommand(strtok(input, NULL), createFlag);
    /*
       if(!strcmp(cmd, "hash"))
       return HASHTABLE;
       if(!strcmp(cmd, "bitmap"))
       return BITMAP;
       if(!strcmp(cmd, "delete") || !strcmp(cmd, "dumpdata") || !strcmp(cmd, "quit"))
       return NONE;
       return ERROR;
       */
}

void listCommand(char* input, bool createFlag) {
    char op[INPUT_SIZE] = { '\0' };
    char name[INPUT_SIZE] = { '\0' };
    char par[5][INPUT_SIZE] = { '\0' };
    LIST_FUNC func;

    assert(input != NULL);
    if(createFlag) {
        func = L_CREATE;
        assert(listCount >= 0 && listCount <= 10);
        strcpy(listName[listCount], input);
        ((void(*)(struct list*)) listFunc[func])(listArray + listCount);
        listCount++;
        return;
    }
}
