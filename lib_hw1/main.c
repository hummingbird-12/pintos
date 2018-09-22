#include <stdio.h>
#include <string.h>
#include "main.h"

#define COMMAND_MAX_SIZE 100
#define PARA_SIZE 5
void command_input(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
void command_init(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
int command_process(char command[], char ds[], char para[][COMMAND_MAX_SIZE]);
int main(){  

  char command[COMMAND_MAX_SIZE];
  char para[PARA_SIZE][COMMAND_MAX_SIZE];
  char ds[COMMAND_MAX_SIZE];
  

  while(1){
    command_init(command,ds,para);
    command_input(command, ds, para);
    printf("%s/%s/%s\n",command,ds,para[0]);
    
    if( command_process(command, ds, para) == QUIT) break;
  }

  
}

int command_process(char command[], char ds[], char para[][COMMAND_MAX_SIZE]){

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
