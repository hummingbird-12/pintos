#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //esp가 가르키고있는거가 NUMBER
  //halt, exit, exec, wait, read, write
 //hex_dump(f->esp, f->esp, 1000,1);
  switch(*(uint32_t*)(f->esp)){
    case SYS_HALT:
      halt();
      break;

    case SYS_EXIT:
      exit((int)(*(uint32_t*)(f->esp + 4)));
      break;

    case SYS_EXEC:
      exec( (char*)(*(uint32_t*)(f->esp + 4)) );
      break;

    case SYS_WAIT:
      wait((pid_t)(*(uint32_t*)(f->esp + 4))) ;
      break;

    case SYS_READ:
      read((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;

    case SYS_WRITE:
      write((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;

      
  }

  //thread_exit ();
}

pid_t exec(const char *file)
{
  return process_execute(file);
}

int wait(pid_t pid){
  return process_wait(pid);
}

void halt (){
  shutdown_power_off();
}

void exit (int status){
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}
int
read(int fd, void *buffer,unsigned size)
{
  if(fd){
    putbuf((char*)buffer,(size_t)size);
    return size;
  }

  return -1;
}
int
write(int fd, const void *buffer,unsigned size)
{
  if(fd){
    putbuf((const char*)buffer,(size_t)size);
    return size;
  }

  return -1;
}
