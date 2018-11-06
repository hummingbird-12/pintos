#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "devices/shutdown.h"
#include "devices/input.h"
#include "threads/vaddr.h"
#include "userprog/process.h"

#include "filesys/filesys.h"

typedef int pid_t;

static void syscall_handler (struct intr_frame *);

static bool validate_address (const void *addr);
static int get_user (const uint8_t *uaddr);
//static bool put_user (uint8_t *uaddr, uint8_t byte);

static void halt (void **argv);
static void exit (void **argv);
static pid_t exec (void **argv);
static int wait (void **argv);
static int create (void **argv);
static int remove (void **argv);
static int open (void **argv);
static int filesize (void **argv);
static int read (void **argv);
static int write (void **argv);
static int seek (void **argv);
static int tell (void **argv);
static int close (void **argv);
static int pibonacci (void **argv);
static int sum_of_four_integers (void **argv);

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
        &create, &remove, &open, &filesize,
        &read, &write, &seek, &tell,
        &close, &pibonacci, &sum_of_four_integers
    };
    /* get arguments for system call,
       address verification will take place later  */
    void *argv[129] = { NULL };
    int i;
    bool valid_address = true;

    for(i = 1; i <= 4; i++)
        argv[i] = (void*) ((uint32_t)(f->esp + i * sizeof(uint32_t)));

    if((valid_address = validate_address(f->esp))) {
        switch(*((int*)f->esp)) {
            case SYS_HALT:                      // Project 1
            case SYS_EXIT:                      // Project 1
            case SYS_SEEK:                      // Project 2
            case SYS_CLOSE:                     // Project 2
                ((void (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
                break;
            case SYS_EXEC:                      // Project 1
                f->eax = ((pid_t (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
                break;
            case SYS_READ:                      // Project 1
            case SYS_WRITE:                     // Project 1
            case SYS_WAIT:                      // Project 1
            case SYS_PIBONACCI:                 // Project 1
            case SYS_SUM_OF_FOUR_INTEGERS:      // Project 1
            case SYS_OPEN:                      // Project 2
            case SYS_FILESIZE:                  // Project 2
                f->eax = ((int (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
                break;
            case SYS_CREATE:                    // Project 2
            case SYS_REMOVE:                    // Project 2
                f->eax = ((bool (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
                break;
            case SYS_TELL:                      // Project 2
                f->eax = ((unsigned (*) (void**)) syscall_ptr[*((int*) f->esp)]) (argv);
                break;
            default:
                break;
        }
    }

    if(!valid_address)
        fail_exit();
}

static bool validate_address (const void *addr) {
    if (is_kernel_vaddr(addr) ||
            get_user((const uint8_t*) addr) == -1)
        return false;
    return true;
}

void fail_exit (void) {
    printf("%s: exit(%d)\n", thread_current()->name, -1);
    thread_current()->exit_status = -1;
    thread_exit ();
}

/* Suggested method in Pintos Manual 3.1.5 */
/* Reads a byte at user virtual address UADDR.
 * UADDR must be below PHYS_BASE.
 * Returns the byte value if successful,
 * -1 if a segfault occured. */
static int get_user (const uint8_t *uaddr) {
    int result;
    asm ("movl $1f, %0; movzbl %1, %0; 1:"
            : "=&a" (result) : "m" (*uaddr));
    return result;
}

/* MAYBE NOT NEEDED */
/* Suggested method in Pintos Manual 3.1.5
 * Writes BYTE to user address UDST.
 * UDST must be below PHYS_BASE.
 * Returns true if successful,
 * false if a segfault occured.
 static bool put_user (uint8_t *udst, uint8_t byte) {
 int error_code;
 asm ("movl $1f, %0; movb %b2, %1; 1:"
 : "=&a" (error_code), "=m" (*udst) : "q" (byte));
 return error_code != -1;
 }
 */

static void halt (void **argv UNUSED) {
    shutdown_power_off();
}

static void exit (void **argv) {
    if(!validate_address(argv[1])) {
        fail_exit();
        return;
    }
    printf("%s: exit(%d)\n", thread_current()->name, *(int*)argv[1]);
    thread_current()->exit_status = *(int*)argv[1];
    thread_exit ();
}

static pid_t exec (void **argv) {
    if(!validate_address((void*)*(uint32_t*) argv[1]) || !validate_address(argv[1])) {
        fail_exit();
        return -1;
    }
    return process_execute(*(const char**)argv[1]);
}

static int wait (void **argv) {
    if(!validate_address(argv[1])) {
        fail_exit();
        return -1;
    }
    return process_wait(*(tid_t*)argv[1]);
}

static bool create(void **argv) {
}

static bool remove(void **argv) {
}

static int open(void **argv) {
    if(!validate_address(argv[1])) {
        fail_exit();
        return -1;
    }
    struct file *openRes = filesys_open(*(const char**) argv[1]);
    return openRes ? openRes : -1;
}

static int filesize(void **argv) {
}

static int read (void **argv) {
    int i;
    if(
            !validate_address((void*)*(uint32_t*) argv[2]) ||
            !(validate_address(argv[1]) && validate_address(argv[2]) && validate_address(argv[3])))
    {
        fail_exit();
        return 0;
    }
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
    if(
            !validate_address((void*)*(uint32_t*) argv[2]) ||
            !(validate_address(argv[1]) && validate_address(argv[2]) && validate_address(argv[3])))
    {
        fail_exit();
        return 0;
    }
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

static void seek (void **argv) {
}

static void tell (void **argv) {
}

static void close (void **argv) {
}

static int pibonacci (void **argv) {
    int fib[47] = { 0 };
    int i;
    if(!validate_address(argv[1])) {
        fail_exit();
        return 0;
    }
    if(*(int*)argv[1] > 46) {
        printf("46-th Fibonacci number is the largest number as signed integer.\n");
        return -1;
    }
    fib[1] = 1;
    for(i = 2; i <= *(int*)argv[1]; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
    return fib[*(int*)argv[1]];
}

static int sum_of_four_integers (void **argv) {
    if(!(validate_address(argv[1]) && validate_address(argv[2]) && validate_address(argv[3]) && validate_address(argv[4]))) {
        fail_exit();
        return 0;
    }
    return *(int*)argv[1] + *(int*)argv[2] + *(int*)argv[3] + *(int*)argv[4];
}
