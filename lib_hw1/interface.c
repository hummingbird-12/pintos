#include "interface.h"
#include "bitmap.h"
#include "debug.h"
#include "hash.h"
#include "limits.h"
#include "list.h"
#include "round.h"

void *listFunc[] = {
    &list_init,
    &list_insert, &list_splice, &list_push_front, &list_push_back,
    &list_remove, &list_pop_front, &list_pop_back, &list_front, &list_back,
    &list_size, &list_empty,
    &list_reverse, &list_sort, &list_insert_ordered, &list_unique,
    &list_max, &list_min,
    &list_swap, &list_shuffle
};

int main() {
    char str[INPUT_SIZE];

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
        listCommand(input);
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

void listCommand(char* input) {
}
