#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

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

  switch(*(uint32_t*)(f->esp)){
    case SYS_EXIT:
      break;

    case SYS_WRITE:
      write((int)(*(uint32_t*)(f->esp + 4)), ((void *)(*(uint32_t*)(f->esp+8))) ,((unsigned)(*(uint32_t*)(f->esp+12))) );
      break;
  }

  //thread_exit ();
}

int
write(int fd, const void *buffer,unsigned size)
{
  if(fd){
    putbuf((const char*)buffer,(size_t)size);
    return size;

  }

  return 0;
}
