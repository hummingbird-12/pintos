#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H
#define ARG_MAX 128
#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
void parse_cmd(char *cmd);

////// yeddo make //////

//uint32_t parse_filename();
//static void stack_push(void **esp, void *data, size_t size);
//void user_memory_access();

//////////////////////

#endif /* userprog/process.h */
