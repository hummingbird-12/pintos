#include "interface.h"
//#include "bitmap.h"
//#include "debug.h"
//#include "hash.h"
//#include "limits.h"
#include "list.h"
//#include "round.h"

void *listFunc[] = {
    &list_init, NULL,
    &list_insert, &list_splice, &list_push_front, &list_push_back,
    &list_remove, &list_pop_front, &list_pop_back, &list_front, &list_back,
    &list_size, &list_empty,
    &list_reverse, &list_sort, &list_insert_ordered, &list_unique,
    &list_max, &list_min,
    &list_swap, &list_shuffle
};
struct LIST_ARRAY {
    struct list* listLink;
    char listName[INPUT_SIZE];
} listArray[MAX_LIST];
int listCount;

int main() {
    char str[INPUT_SIZE];

    initializer();
    
    do {
        memset(str, '\0', INPUT_SIZE);
        fgets(str, sizeof(str), stdin);
    } while(!inputParser(str));
    
    return 0;
}

void initializer() {
    int i;

    listCount = 0;
    for(i = 0; i < MAX_LIST; i++) {
        listArray[i].listLink = NULL;
        memset(listArray[i].listName, INPUT_SIZE, '\0');
    }
}

int findTargetIndex(CMD_TYPE type, char *name) {
    int index;
    switch(type) {
        case LIST:
            for(index = 0; index < MAX_LIST; index++)
                if(listArray[index].listLink && !strcmp(listArray[index].listName, name))
                    return index;
            break;
        case HASHTABLE:
            break;
        case BITMAP:
            break;
        default:
            break;
    }
    return -1;
}

bool inputParser(char* input) {
    char delim[] = " \n";
    char tok[20][INPUT_SIZE] = { '\0' };
    int i = 0;
    bool createFlag = false, quitFlag = false;

    assert(input != NULL);

    strcpy(tok[i], strtok(input, delim));
    while(tok[i][0]) {
        char *tmp = strtok(NULL, delim);
        i++;
        if(tmp)
            strcpy(tok[i], tmp);
    }

    if(!strcmp(tok[0], "create")) {
        assert(tok[1][0] != '\0' && tok[2][0] != '\0');
        createFlag = true;
    }

    if(!strncmp(tok[0], "list_", 5) || !strcmp(tok[1], "list"))
        listCommand(tok, createFlag);
    /*
       if(!strcmp(cmd, "hash"))
       return HASHTABLE;
       if(!strcmp(cmd, "bitmap"))
       return BITMAP;
       */
    if(!strcmp(tok[0], "dumpdata") && tok[1][0] != '\0')
        dataDumper(tok[1]);
    if(!strcmp(tok[0], "delete"))
        dataDestroyer(tok[1]);
    if(!strcmp(tok[0], "quit"))
        quitFlag = true;
    return quitFlag;
}

void dataDumper(char* name) {
    CMD_TYPE type = LIST;
    int index;
    struct list_elem *it;

    while((index = findTargetIndex(type, name)) == -1) {
        if(type < BITMAP)
            type++;
        else
            return;
    }
    
    switch(type) {
        case LIST:
            for(it = list_begin(listArray[index].listLink);
                    it != list_end(listArray[index].listLink);
                    it = list_next(it)) {
                printf("%d ", list_entry(it, LIST_ITEM, elem)->data);
            }
            if(!list_empty(listArray[index].listLink))
                puts("");
            break;
        case HASHTABLE:
        case BITMAP:
            break;
        default:
            break;
    }
}

void dataDestroyer(char* name) {
    CMD_TYPE type = LIST;
    int index;
    struct list_elem *it1, *it2;

    while((index = findTargetIndex(type, name)) == -1) {
        if(type < BITMAP)
            type++;
        else
            return;
    }
    
    switch(type) {
        case LIST:
            for(it1 = list_begin(listArray[index].listLink);
                    it1 != list_end(listArray[index].listLink);) {
                it2 = list_next(it1);
                free(list_entry(it1, LIST_ITEM, elem));
                it1 = it2;
            }
            listArray[index].listLink = NULL;
            listCount--;
            memset(listArray[index].listName, INPUT_SIZE, '\0');
            break;
        case HASHTABLE:
        case BITMAP:
            break;
        default:
            break;
    }
}

void listCommand(char tok[][INPUT_SIZE], bool createFlag) {
    char funcList[][INPUT_SIZE] = {
        "create", "destroy",
        "insert", "splice", "push_front", "push_back",
        "remove", "pop_front", "pop_back", "front", "back",
        "size", "empty",
        "reverse", "sort", "insert_ordered", "unique",
        "max", "min",
        "swap", "shuffle"
    };
    char funcName[INPUT_SIZE] = { '\0' };
    LIST_FUNC funcNum;
    int index, i;

    LIST_ITEM *listItem = NULL;
    struct list_elem *elem0, *elem1, *elem2, *elem3;

    if(createFlag) {
        funcNum = L_CREATE;
        assert(tok[2][0] != '\0');
        assert(tok[3][0] == '\0');
        assert(listCount >= 0 && listCount <= 10);

        for(index = 0; listArray[index].listLink && index < MAX_LIST; index++);

        strcpy(listArray[index].listName, tok[2]);
        listArray[index].listLink = (struct list*) malloc(sizeof(struct list));
        listCount++;
    }
    else {
        strcpy(funcName, tok[0] + 5);
        index = findTargetIndex(LIST, tok[1]);
        for(funcNum = L_INSERT; strcmp(funcName, funcList[funcNum]) && funcNum <= L_SHUFFLE; funcNum++);
    }

    assert(index != -1);
    assert(index < MAX_LIST);
    struct list* targetList = listArray[index].listLink;
    struct list* targetList2;

    switch(funcNum) {
        // return type is void
        case L_CREATE:
        case L_REVERSE:
        case L_SHUFFLE:
            ((void(*)(struct list*)) listFunc[funcNum]) (targetList);
            break;
        case L_INSERT:
            listItem = (LIST_ITEM*) malloc(sizeof(LIST_ITEM));
            listItem->data = strtol(tok[3], NULL, 10);

            elem1 = listSearchByIndex(targetList, strtol(tok[2], NULL, 10));
            if(!elem1)
                elem1 = list_end(targetList);

            ((void(*)(struct list_elem*, struct list_elem*)) listFunc[funcNum])(elem1, &(listItem->elem));
            break;
        case L_SPLICE:
            elem1 = listSearchByIndex(targetList, strtol(tok[2], NULL, 10));
            elem2 = listSearchByIndex(targetList, strtol(tok[3], NULL, 10));
            elem3 = listSearchByIndex(targetList, strtol(tok[4], NULL, 10));

            assert(elem1 != NULL && elem2 != NULL && elem3 != NULL);

            ((void(*)(struct list_elem*, struct list_elem*, struct list_elem*)) listFunc[funcNum]) (elem1, elem2, elem3);
            break;
        case L_PUSH_FRONT:
        case L_PUSH_BACK:
            listItem = (LIST_ITEM*) malloc(sizeof(LIST_ITEM));
            listItem->data = strtol(tok[2], NULL, 10);

            ((void(*)(struct list*, struct list_elem*)) listFunc[funcNum]) (targetList, &(listItem->elem));
            break;
        case L_SORT:
            ((void(*)(struct list*, list_less_func*, void*)) listFunc[funcNum]) (targetList, (list_less_func*) elem_compare, NULL);
            break;
        case L_INSERT_ORDERED:
            listItem = (LIST_ITEM*) malloc(sizeof(LIST_ITEM));
            listItem->data = strtol(tok[3], NULL, 10);

            ((void(*)(struct list*, struct list_elem*, list_less_func*, void*)) listFunc[funcNum]) (targetList, &(listItem->elem), (list_less_func*) elem_compare, NULL);
            break;
        case L_UNIQUE:
            if(tok[2][0] == '\0')
                targetList2 = NULL;
            else
                targetList2 = listArray[findTargetIndex(LIST, tok[2])].listLink;
            
            ((void(*)(struct list*, struct list*, list_less_func*, void*)) listFunc[funcNum]) (targetList, targetList2, (list_less_func*) elem_compare, NULL);
            break;
        case L_SWAP:
            elem1 = listSearchByIndex(targetList, strtol(tok[2], NULL, 10));
            elem2 = listSearchByIndex(targetList, strtol(tok[3], NULL, 10));

            assert(elem1 != NULL && elem2 != NULL);

            ((void(*)(struct list_elem*, struct list_elem*)) listFunc[funcNum]) (elem1, elem2);
            break;
        // return type is struct list_elem*
        case L_REMOVE:
        case L_POP_FRONT:
        case L_POP_BACK:
        case L_FRONT:
        case L_BACK:
        case L_MAX:
        case L_MIN:
            break;
        // return type is size_t
        case L_SIZE:
            break;
        // return type is bool
        case L_EMPTY:
            break;
        default:
            errorDump("Unkown list command");
            break;
    }
}

struct list_elem* listSearchByData(struct list* list, int data) {
    struct list_elem *it;
    for(it = list_begin(list); it != list_end(list); it = list_next(it))
        if(data == list_entry(it, LIST_ITEM, elem)->data)
            return it;
    return NULL;
}

struct list_elem* listSearchByIndex(struct list* list, int index) {
    struct list_elem *it;
    int i;

    for(it = list_begin(list), i = 0; it != list_end(list); it = list_next(it), i++)
        if(i == index)
            return it;
    return NULL;
}
