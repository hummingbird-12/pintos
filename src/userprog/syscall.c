#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "userprog/syscall.h"
static void syscall_handler (struct intr_frame *);
static int get_user(const uint8_t *uaddr);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //esp가 가르키고있는거가 NUMBER
 //hex_dump(f->esp, f->esp, 1000,1);
  switch(*(uint32_t*)(f->esp)){
    
    case SYS_WRITE:
      sys_write((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;

    case SYS_READ:
      sys_read((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;
    
    case SYS_WAIT:
      sys_wait((pid_t)(*(uint32_t*)(f->esp + 4))) ;
      break;

    case SYS_EXEC:
      sys_exec( (char*)(*(uint32_t*)(f->esp + 4)) );
      break;


    case SYS_EXIT:
      sys_exit((int)(*(uint32_t*)(f->esp + 4)));
      break;
 

    case SYS_HALT:
      sys_halt();
      break;
    
  }

  //thread_exit ();
}



static bool validate_addr (const void *addr){
  if ( (!is_user_addr(addr)) || get_user((const uint8_t*) addr) == -1)
    return false;
  return true;

}


pid_t sys_exec(const char *file)
{
  return process_execute(file);
}

int sys_wait(pid_t pid){
  return process_wait(pid);
}

void sys_halt (){
  shutdown_power_off();
}

void sys_exit (int status){
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}
int
sys_read(int fd, void *buffer,unsigned size)
{
  if(fd){
    putbuf((char*)buffer,(size_t)size);
    return size;
  }

  return -1;
}
int
sys_write(int fd, const void *buffer,unsigned size)
{
  if(fd){
    putbuf((const char*)buffer,(size_t)size);
    return size;
  }

  return -1;
}

static int 
get_user(const uint8_t *uaddr)
{
  int result;
  asm("movl $1f, %0; movzbl %1, %0; 1:"
      : "=&a" (result) : "m" (*uaddr));
  return result;
}



