#include <stdio.h>
#include <string.h>
#include "main.h"

#define COMMAND_MAX_SIZE 100
#define PARA_SIZE 5
#define LIST_NUM 17
#define HASH_NUM 8
#define BITMAP_NUM 13
#define ETC_NUM 7

int main(){  

  char command[COMMAND_MAX_SIZE];
  char para[PARA_SIZE][COMMAND_MAX_SIZE];
  char ds[COMMAND_MAX_SIZE];
  

  while(1){
    command_init(command,ds,para);
    command_input(command, ds, para);
    printf("test: %s/%s/%s\n",command,ds,para[0]);
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
    }
  return 0;
}

void etc_process(int cmd,char ds[], char para[][COMMAND_MAX_SIZE]){
  switch(cmd){
    case CREATE:
      create();
      break;

  }

}


