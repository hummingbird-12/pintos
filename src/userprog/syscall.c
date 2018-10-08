#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "devices/shutdown.h"
#include "devices/input.h"
#include "threads/vaddr.h"
#include "userprog/process.h"

typedef int pid_t;

static void syscall_handler (struct intr_frame *);

static bool validate_address (const void* addr);

static void halt (void **argv);
static void exit (void **argv);
static pid_t exec (void **argv);
static int wait (void **argv);
static int read (void **argv);
static int write (void **argv);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
    /* function pointer to each system call, NULL means not yet implemented  */
    void *syscall_ptr[] = {
        &halt, &exit, &exec, &wait,
        NULL, NULL, NULL, NULL,
        &read, &write, NULL, NULL,
        NULL
    };
    /* get arguments for system call, address verification will take place later  */
    void *argv[129] = { NULL };
    int i;

    for(i = 1; i <= 3; i++)
        argv[i] = (void*) ((uint32_t)(f->esp + i * sizeof(uint32_t)));

    switch(*((int*)f->esp)) {
        case SYS_HALT:
        case SYS_EXIT:
            ((void (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
            break;
        case SYS_EXEC:
            f->eax = ((pid_t (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
            break;
        case SYS_WAIT:
        case SYS_READ:
        case SYS_WRITE:
            f->eax = ((int (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
            break;
        default:
            break;
    }
}

static bool validate_address (const void *addr) {
    if ((uint32_t*)addr == NULL || is_kernel_vaddr(addr))
        return false;
}

static void halt (void **argv UNUSED) {
    shutdown_power_off();
}

static void exit (void **argv) {
    printf("%s: exit(%d)\n", thread_current()->name, *(int*)argv[1]);
    thread_current()->exit_status = *(int*)argv[1];
    thread_exit ();
}

static pid_t exec (void **argv) {
    return process_execute(*(const char**)argv[1]);
}

static int wait (void **argv) {
    return process_wait(*(tid_t*)argv[1]);
}

static int read (void **argv) {
    int i;
    switch(*(int*)argv[1]) {
        case STDIN_FILENO:
            for(i = 0; i < *(int*)argv[3]; i++)
                (*(char**)argv[2])[i] = input_getc();
            return *(unsigned*)argv[3];
            break;
        default:
            break;
    }
    return 0;
}

static int write (void **argv) {
    switch(*(int*)argv[1]) {
        case STDOUT_FILENO:
            putbuf(*(const void**)argv[2], *(unsigned*)argv[3]);
            return *(unsigned*)argv[3];
            break;
        default:
            break;
    }
    return 0;
}
