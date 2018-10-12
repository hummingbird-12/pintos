#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;

void syscall_init (void);

void sys_halt();
pid_t sys_exec(const char* file);
int sys_read(int fd, void *buffer, unsigned size);
int sys_write(int fd, const void *buffer, unsigned size);
void sys_exit(int status);
int sys_wait(pid_t pid);

#endif /* userprog/syscall.h */
