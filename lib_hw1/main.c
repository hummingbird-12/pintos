#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "main.h"

#define COMMAND_MAX_SIZE 100
#define PARA_SIZE 5
#define LIST_NUM 17
#define HASH_NUM 8
#define BITMAP_NUM 13
#define ETC_NUM 7
#define DS_NUM 3

#define ASSERT(CONDITION) assert(CONDITION)

int main(){  

  char command[COMMAND_MAX_SIZE];
  char para[PARA_SIZE][COMMAND_MAX_SIZE];
  char ds[COMMAND_MAX_SIZE];
  
  dc_head=NULL;
  dc_tail=NULL;


  while(1){
    command_init(command,ds,para);
    command_input(command, ds, para);
  //  printf("test: %s/%s/%s\n",command,ds,para[0]);
    if(command_process(command, ds, para)==-1)  break;
    
    
  }

  
}

void command_init(char command[], char ds[], char para[][COMMAND_MAX_SIZE]){
  int i;
  
  memset(command, '\0', COMMAND_MAX_SIZE);
  memset(ds, '\0', COMMAND_MAX_SIZE);
  for(i=0; i<PARA_SIZE ; i++){
    memset(para[i], '\0', COMMAND_MAX_SIZE);
  }
}

void command_input(char command[], char ds[], char para[][COMMAND_MAX_SIZE]){
  int cnt=0;
  char *token;
  char input[COMMAND_MAX_SIZE];
  
  fgets(input,COMMAND_MAX_SIZE,stdin);
  token = strtok(input, " \n");
  do{
    if(cnt==0) strcpy(command, token);
    else if(cnt==1) strcpy(ds, token);
    else strcpy( para[cnt-2] ,token);
    cnt++;
  }while (token = strtok(NULL, " \n"));  
  
}


int command_process(char command[], char ds[], char para[][COMMAND_MAX_SIZE]){
  int i;
  for(i=0 ; i<ETC_NUM ; i++)
    if( !strcmp(str_cmd_etc[i],command) ){
      if(i==QUIT) return -1;
      else etc_process(i,ds,para);
      return 0;
    }
  for(i=0 ; i<LIST_NUM ; i++)
    if(!strcmp(str_cmd_list[i],command) ){
      list_process(i,ds,para);
      return 0;
    }
 
  ASSERT(1);
}
/////////////////////////////// ETC ///////////////////////////////////
void etc_process(int cmd,char ds[], char para[][COMMAND_MAX_SIZE]){
  switch(cmd){

    case CREATE:
      create(ds,para);
      break;

    case DELETE:
      strcpy(para[0],ds);
      delete(para);
      break;

    case DUMPDATA:      
      strcpy(para[0],ds);
      dumpdata(para);
      break;
      
  }

}

void create(char ds[], char para[][COMMAND_MAX_SIZE]){
  int i=0;
  struct data_collect *dc;
  struct list *list;
  struct hash *hash;

  dc=(struct data_collect*)malloc(sizeof(struct data_collect));


  for(i=0; i<DS_NUM; i++)
    if(!strcmp(str_ds[i],ds)){
      switch(i){


        case LIST:
          list=(struct list*)malloc(sizeof(struct list));
          (dc->data).list = list;
          list_init(list);
          break;


        case HASHTABLE:
          hash=(struct hash*)malloc(sizeof(struct hash));
          (dc->data).hash = hash;
         // hash_init(hash, 0,hash_int_2,hash,NULL);
          break;

        case BITMAP:

          break;
      }
      break;
    }

    //make and link data_collect 
    strcpy(dc->name,para[0]);
    dc->next = NULL;
    dc->data_type = i;
    if(dc_head == NULL){
        dc_head = dc;
        dc_tail = dc;
    }
    else{
        dc_tail->next = dc;
        dc_tail=dc;
    }


}

void delete(char para[][COMMAND_MAX_SIZE]){
  struct data_collect *dc, *bef_dc;
  struct list *list;
  struct list_elem *elem, *del_elem;
  
  dc = find_data_collect(para[0]);
  ASSERT(dc!=NULL);
 
  switch(dc->data_type){
    case LIST:
      for(elem = list_begin(list) ; elem != list_end(list);){
        del_elem = elem;
        if(elem != list_end(list))
          break;
        elem = list_next(elem);
        free(del_elem);
      }
      break;
    case HASHTABLE:

      break;

    case BITMAP:

      break;
  }

  if(dc == dc_head){
    dc_head = dc->next;
    if(dc == dc_tail)
      dc_tail = NULL;
  }
  else{
    for(bef_dc = dc_head; bef_dc->next != dc; bef_dc=bef_dc->next) break;
    bef_dc->next=dc->next;
    if(dc == dc_tail)
      dc_tail=bef_dc;
  }
  
  free(dc);

}

void dumpdata(char para[][COMMAND_MAX_SIZE]){
  //searching data
  struct data_collect *dc;
  struct list *list;
  struct list_elem *elem;
  
  dc = find_data_collect(para[0]);
  ASSERT(dc!=NULL);
  switch(dc->data_type){
    case LIST:
      list=(dc->data).list;
      if(list_empty(list)){
        printf("EMPTY\n");
        return;
      }
      for(elem = list_begin(list) ; elem != list_end(list); elem = list_next(elem)){
        printf("%d ",(list_entry(elem,struct list_item, elem) )->data);
      }
      printf("\n");
      break;

    case HASHTABLE:
      
      break;

    case BITMAP:
      break;


  }
}

struct data_collect* find_data_collect(char name[]){
  struct data_collect *dc;
  for(dc = dc_head ; dc!=NULL; dc=dc->next){
    if(!strcmp(dc->name, name)) return dc;
  }
  return NULL;
}

//////////////////// LIST ////////////////////////////

void list_process(int cmd, char ds[], char para[][COMMAND_MAX_SIZE] ){
  struct data_collect *dc;
  struct list_item *item;
  struct list_elem *elem,*elem1,*elem2,*elem_tmp;
  struct list *list,*list1,*list2;
  int cnt;
  int idx1,idx2,idx3;

  switch(cmd){
    case LIST_INSERT:
      dc = find_data_collect(ds);
      ASSERT(dc != NULL);

      item = (struct list_item*)malloc(sizeof(struct list_item));
      item->data = strtol(para[1],NULL,10);
      cnt = strtol(para[0],NULL,10);
      
      list = (dc->data).list;
      for(elem = list_begin(list) ; elem != list_end(list);){
        if(cnt==0) break; 
        elem = list_next(elem);
        cnt--;
      }

      list_insert(elem,&(item->elem));
      break;


    case LIST_SPLICE:
      dc = find_data_collect(ds);
      ASSERT(dc != NULL);
      list1 = (dc->data).list;
      
      cnt = 0;
      idx1 = strtol(para[0],NULL,10);
      
      dc = find_data_collect(para[1]);
      ASSERT(dc != NULL);
      list2 = (dc->data).list;

      idx2 = strtol(para[2],NULL,10);
      idx3 = strtol(para[3],NULL,10);
     
      for(elem = list_begin(list1) ; elem != list_end(list1) ; ){
        if(cnt == idx1) break;
        elem  = list_next(elem);
        cnt++;
      }

      cnt=0;
      for(elem_tmp = list_begin(list2); elem_tmp!= list_end(list2);){
        if(cnt == idx3){
          elem2 = elem_tmp;
          break;
        }
        if(cnt == idx2) elem1 = elem_tmp;
        
        elem_tmp = list_next(elem_tmp);
        cnt++;
      }
      list_splice(elem, elem1,elem2);
      break;
      
    case LIST_PUSH_FRONT:
      dc = find_data_collect(ds);
      ASSERT(dc != NULL); 
      item = (struct list_item*)malloc(sizeof(struct list_item));
      item->data = strtol(para[0],NULL,10);
      list_push_front((dc->data).list, &(item->elem));
      break;

    case LIST_PUSH_BACK:
      dc = find_data_collect(ds);
      ASSERT(dc != NULL); 
      item = (struct list_item*)malloc(sizeof(struct list_item));
      item->data = strtol(para[0],NULL,10);
      list_push_back((dc->data).list, &(item->elem));
      break;

    case LIST_REMOVE:
      break;
    case LIST_POP_FRONT:
      break;
    case LIST_POP_BACK:
      break;
    case LIST_FRONT:
      break;
    case LIST_BACK:
      break;
    case LIST_SIZE:
      break;
    case LIST_EMPTY:
      break;
    case LIST_REVERSE:
      break;
    case LIST_SORT:
      break;
    case LIST_INSERT_ORDERED:
      break;
    case LIST_UNIQUE:
      break;
    case LIST_MAX:
      break;
    case LIST_MIN:
      break;
  }
}

