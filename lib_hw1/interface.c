#include "interface.h"
//#include "bitmap.h"
//#include "debug.h"
#include "hash.h"
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

void *hashFunc[] = {
    &hash_init, NULL,
    &hash_insert, &hash_replace, &hash_find, &hash_delete,
    &hash_clear, &hash_size, &hash_empty, &hash_apply,
    &hash_int_2
};
struct HASH_ARRAY {
    struct hash* hashLink;
    char hashName[INPUT_SIZE];
} hashArray[MAX_HASHTABLE];
int hashCount;

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
        memset(listArray[i].listName, '\0', INPUT_SIZE);
    }

    hashCount = 0;
    for(i = 0; i < MAX_HASHTABLE; i++) {
        hashArray[i].hashLink = NULL;
        memset(hashArray[i].hashName, '\0', INPUT_SIZE);
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
            for(index = 0; index < MAX_HASHTABLE; index++)
                if(hashArray[index].hashLink && !strcmp(hashArray[index].hashName, name))
                    return index;
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

    if(!strncmp(tok[0], "list_", 5) || (createFlag && !strcmp(tok[1], "list")))
        listCommand(tok, createFlag);
    else if(!strncmp(tok[0], "hash_", 5) || (createFlag && !strcmp(tok[1], "hashtable")))
        hashCommand(tok, createFlag);
    else if(!strncmp(tok[0], "bitmap_", 7) || (createFlag && !strcmp(tok[1], "bitmap")))
        debugDump("NOT YET!");
    else if(!strcmp(tok[0], "dumpdata") && tok[1][0] != '\0')
        dataDumper(tok[1]);
    else if(!strcmp(tok[0], "delete"))
        dataDestroyer(tok[1]);
    else if(!strcmp(tok[0], "quit"))
        quitFlag = true;
    else
        errorDump("Unkown interface command");
    return quitFlag;
}

void dataDumper(char* name) {
    CMD_TYPE type = LIST;
    int index;
    struct list_elem *listIt;
    struct hash_iterator hashIt;

    while((index = findTargetIndex(type, name)) == -1) {
        if(type < BITMAP)
            type++;
        else
            return;
    }
    
    switch(type) {
        case LIST:
            for(listIt = list_begin(listArray[index].listLink);
                    listIt != list_end(listArray[index].listLink);
                    listIt = list_next(listIt)) {
                printf("%d ", list_entry(listIt, LIST_ITEM, elem)->data);
            }
            if(!list_empty(listArray[index].listLink))
                puts("");
            break;
        case HASHTABLE:
            hash_first(&hashIt, hashArray[index].hashLink);
            while(hash_next(&hashIt))
                printf("%d ", hash_entry(hash_cur(&hashIt), HASH_ITEM, elem)->data);
            if(!hash_empty(hashArray[index].hashLink))
                puts("");
            break;
        case BITMAP:
            break;
        default:
            break;
    }
}

void dataDestroyer(char* name) {
    CMD_TYPE type = LIST;
    int index;
    struct list_elem *listIt1, *listIt2;
    struct hash_iterator hashIt1, hashIt2;

    while((index = findTargetIndex(type, name)) == -1) {
        if(type < BITMAP)
            type++;
        else
            return;
    }
    
    switch(type) {
        case LIST:
            for(listIt1 = list_begin(listArray[index].listLink);
                    listIt1 != list_end(listArray[index].listLink);) {
                listIt2 = list_next(listIt1);
                free(list_entry(listIt1, LIST_ITEM, elem));
                listIt1 = listIt2;
            }
            free(listArray[index].listLink);
            listArray[index].listLink = NULL;
            listCount--;
            memset(listArray[index].listName, '\0', INPUT_SIZE);
            break;
        case HASHTABLE:
            hash_first(&hashIt1, hashArray[index].hashLink);
            while(1) {
                if(!hash_next(&hashIt1))
                    break;
                hashIt2 = hashIt1;
                free(hash_entry(hash_cur(&hashIt1), HASH_ITEM, elem));
                hashIt1 = hashIt2;
            }
            free(hashArray[index].hashLink);
            hashArray[index].hashLink = NULL;
            hashCount--;
            memset(hashArray[index].hashName, '\0', INPUT_SIZE);
            break;
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
    int index;

    LIST_ITEM *listItem = NULL;
    struct list_elem *elem1, *elem2, *elem3;

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

            targetList2 = listArray[findTargetIndex(LIST, tok[3])].listLink;
            elem2 = listSearchByIndex(targetList2, strtol(tok[4], NULL, 10));
            elem3 = listSearchByIndex(targetList2, strtol(tok[5], NULL, 10));

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
            listItem->data = strtol(tok[2], NULL, 10);

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
            elem1 = listSearchByIndex(targetList, strtol(tok[2], NULL, 10));
            ((struct list_elem*(*)(struct list_elem*)) listFunc[funcNum]) (elem1);
            
            free(list_entry(elem1, LIST_ITEM, elem));
            break;
        case L_POP_FRONT:
        case L_POP_BACK:
            free(list_entry((((struct list_elem*(*)(struct list*)) listFunc[funcNum]) (targetList)), LIST_ITEM, elem));
            break;
        case L_FRONT:
        case L_BACK:
            printf("%d\n", list_entry(((struct list_elem*(*)(struct list*)) listFunc[funcNum]) (targetList), LIST_ITEM, elem)->data);
            break;
        case L_MAX:
        case L_MIN:
            printf("%d\n", list_entry(((struct list_elem*(*)(struct list*, list_less_func*, void*)) listFunc[funcNum]) (targetList, (list_less_func*) elem_compare, NULL), LIST_ITEM, elem)->data);
            break;
        // return type is size_t
        case L_SIZE:
            printf("%zu\n", ((size_t(*)(struct list*)) listFunc[funcNum]) (targetList));
            break;
        // return type is bool
        case L_EMPTY:
            printf("%s\n", (((bool(*)(struct list*)) listFunc[funcNum]) (targetList)) ? "true" : "false");
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

void hashCommand(char tok[][INPUT_SIZE], bool createFlag) {
    char funcList[][INPUT_SIZE] = {
        "create", "destroy",
        "insert", "replace", "find", "delete",
        "clear", "size", "empty", "apply",
        "int_2"
    };
    char funcName[INPUT_SIZE] = { '\0' };
    HASH_FUNC funcNum;
    int index;

    HASH_ITEM *hashItem = NULL;
    struct hash_elem *elem1;

    if(createFlag) {
        funcNum = H_CREATE;
        assert(tok[2][0] != '\0');
        assert(tok[3][0] == '\0');
        assert(hashCount >= 0 && hashCount <= 10);

        for(index = 0; hashArray[index].hashLink && index < MAX_HASHTABLE; index++);

        strcpy(hashArray[index].hashName, tok[2]);
        hashArray[index].hashLink = (struct hash*) malloc(sizeof(struct hash));
        hashCount++;
    }
    else {
        strcpy(funcName, tok[0] + 5);
        index = findTargetIndex(HASHTABLE, tok[1]);
        for(funcNum = H_INSERT; strcmp(funcName, funcList[funcNum]) && funcNum <= H_INT_2; funcNum++);
    }

    assert(index != -1);
    assert(index < MAX_HASHTABLE);
    struct hash* targetHash = hashArray[index].hashLink;

    switch(funcNum) {
        // return type is bool
        case H_CREATE:
            ((bool(*)(struct hash*, hash_hash_func*, hash_less_func*, void*)) hashFunc[funcNum]) (targetHash, (hash_hash_func*) hasher, (hash_less_func*) hash_compare, NULL);
            break;
        case H_EMPTY:
            printf("%s\n", (((bool(*)(struct hash*)) hashFunc[funcNum]) (targetHash)) ? "true" : "false");
            break;
        // return type is struct hash_elem*
        case H_INSERT:
            hashItem = (HASH_ITEM*) malloc(sizeof(HASH_ITEM));
            hashItem->data = strtol(tok[2], NULL, 10);

            ((struct hash_elem*(*)(struct hash*, struct hash_elem*)) hashFunc[funcNum]) (targetHash, &(hashItem->elem));
            break;
        case H_REPLACE:
        case H_DELETE:
            hashItem = (HASH_ITEM*) malloc(sizeof(HASH_ITEM));
            hashItem->data = strtol(tok[2], NULL, 10);

            elem1 = ((struct hash_elem*(*)(struct hash*, struct hash_elem*)) hashFunc[funcNum]) (targetHash, &(hashItem->elem));
            if(elem1)
                free(hash_entry(elem1, HASH_ITEM, elem));
            if(funcNum == H_DELETE)
                free(hashItem);
            break;
        case H_FIND:
            hashItem = (HASH_ITEM*) malloc(sizeof(HASH_ITEM));
            hashItem->data = strtol(tok[2], NULL, 10);

            elem1 = ((struct hash_elem*(*)(struct hash*, struct hash_elem*)) hashFunc[funcNum]) (targetHash, &(hashItem->elem));
            if(elem1)
                printf("%d\n", hashItem->data); //hash_entry(elem1, HASH_ITEM, elem)->data);
            free(hashItem);
            break;
        // return type is void
        case H_CLEAR:
            ((void(*)(struct hash*, hash_action_func*)) hashFunc[funcNum]) (targetHash, (hash_action_func*) hash_destruct);
            break;
        case H_APPLY:
            if(!strcmp(tok[2], "square"))
                ((void(*)(struct hash*, hash_action_func*)) hashFunc[funcNum]) (targetHash, (hash_action_func*) hash_square);
            else if(!strcmp(tok[2], "triple"))
                ((void(*)(struct hash*, hash_action_func*)) hashFunc[funcNum]) (targetHash, (hash_action_func*) hash_triple);
            break;
        // return type is size_t
        case H_SIZE:
            printf("%zu\n", ((size_t(*)(struct hash*)) hashFunc[funcNum]) (targetHash));
            break;
        default:
            errorDump("Unkown hash table command");
            break;
    }
}
