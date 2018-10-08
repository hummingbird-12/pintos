#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;

void syscall_init (void);
void halt();
pid_t exec(const char* file);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
void exit(int status);
int wait(pid_t pid);


#endif /* userprog/syscall.h */
