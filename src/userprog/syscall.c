#include "userprog/syscall.h"
#include <syscall-nr.h>
#include <stdio.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
static int get_user(const uint8_t *uaddr);
static bool validate_addr (const void *addr);

static void sys_halt(void);
static pid_t sys_exec(const char* file);
static int sys_read(int fd,const void *buffer, unsigned size);
static int sys_write(int fd, const void *buffer, unsigned size);
static void sys_exit(int status);
static int sys_wait(pid_t pid);

static bool check_valid_arg(void* arg[], int argc);

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
  void* arg[] = {NULL,f->esp+4,f->esp+8,f->esp+12,f->esp+16};

  switch(*((int*)f->esp)){
    case SYS_WRITE:
      if(!check_valid_arg(arg,3) ){
        f->eax = 0;
        break;
      }
      f->eax = sys_write((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;

    case SYS_READ:
      if(!check_valid_arg(arg,3) ){
        f->eax = 0;
        break;
      }
      f->eax = sys_read((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;

    case SYS_WAIT:
      if(!check_valid_arg(arg,1) ){
        f->eax = -1;
        break;
      }
      //   hex_dump(f->esp, f->esp, 100,1);
      f->eax = sys_wait((pid_t)(*(uint32_t*)(f->esp + 4))) ;
      break;

    case SYS_EXEC:
      if(!check_valid_arg(arg,1)){
        f->eax = -1;
        break;
      }
      f->eax = sys_exec( (char*)(*(uint32_t*)(f->esp + 4)) );
      break;


    case SYS_EXIT:
      if(!check_valid_arg(arg,1) ) break;
      sys_exit((int)(*(uint32_t*)(f->esp + 4)));
      break;


    case SYS_HALT:
      sys_halt();
      break;

  }

  //thread_exit ();
  //}
}

static bool check_valid_arg( void* arg[], int argc){
  int i;
  for(i=1; i<=argc ; i++)
    if(validate_addr(arg[i])==false){
      fail_exit();
      break;
    }

}

static bool validate_addr (const void *addr){
  if ( (is_kernel_vaddr(addr)) && get_user((const uint8_t*) addr) == -1)
    return false;
  return true;

}
void fail_exit (void){
  printf("%s: exit(%d)\n", thread_current()->name, -1);
  thread_current()->exit_status = -1;
  thread_exit();
}

  static pid_t 
sys_exec(const char *file)
{
  return process_execute(file);
}

static int 
sys_wait(pid_t pid){
  return process_wait(pid);
}

static void 
sys_halt (){
  shutdown_power_off();
}

static void 
sys_exit (int status){
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_current()->exit_status = status;
  thread_exit();
}
  static int
sys_read(int fd, const void *buffer,unsigned size)
{
  if(!(validate_addr(buffer))){
    fail_exit();
    return 0;
  }
  if(fd){
    putbuf((char*)buffer,(size_t)size);
    return size;
  }
  fail_exit();
  return 0;
}
  static int
sys_write(int fd, const void *buffer,unsigned size)
{
  if( !(validate_addr(buffer)) ){
    fail_exit();
    return 0;
  }

  if(fd){
    putbuf((const char*)buffer,(size_t)size);
    return size;
  }

  fail_exit();
  return 0;
}

  static int 
get_user(const uint8_t *uaddr)
{
  int result;
  asm("movl $1f, %0; movzbl %1, %0; 1:"
      : "=&a" (result) : "m" (*uaddr));
  return result;
}



