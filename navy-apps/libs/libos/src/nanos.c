#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include "syscall.h"
#include<stdio.h>

#if defined(__ISA_X86__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  int ret = -1;
  asm volatile("int $0x80": "=a"(ret): "a"(type), "b"(a0), "c"(a1), "d"(a2));
  return ret;
}
#elif defined(__ISA_AM_NATIVE__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  intptr_t ret = 0;
  asm volatile("call *0x100000": "=a"(ret): "a"(type), "S"(a0), "d"(a1), "c"(a2));
  return ret;
}
#else
#error _syscall_ is not implemented
#endif

void _exit(int status) {
  _syscall_(SYS_exit, status, 0, 0);
  while (1);
}

int _open(const char *path, int flags, mode_t mode) {
  return _syscall_(SYS_open,(intptr_t)path,flags,mode);
    //_exit(SYS_open);
}

int _write(int fd, void *buf, size_t count){
  return _syscall_(SYS_write,fd,(intptr_t)buf,count);
  // _exit(SYS_write);
}


extern char _end;
void * program_break=&_end;
void * old_pb;


void *_sbrk(intptr_t increment){
    old_pb=program_break;
    intptr_t now=(intptr_t) program_break+increment;
    program_break=(void *) now;//actually we let user to alter the stack
     if(_syscall_(SYS_brk,now,0,0)==0)
         return old_pb;
     else
         return (void *)-1;//pa3.2 should not reach here
}

int _read(int fd, void *buf, size_t count) {
    return _syscall_(SYS_read,fd,(intptr_t)buf,count);
    // _exit(SYS_read);
}

int _close(int fd) {
 return _syscall_(SYS_close,fd,0,0);
    //_exit(SYS_close);
}

off_t _lseek(int fd, off_t offset, int whence) {
 return _syscall_(SYS_lseek,fd,offset,whence);
    //_exit(SYS_lseek);
}

int _execve(const char *fname, char * const argv[], char *const envp[]) {
  _exit(SYS_execve);
  return 0;
}

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

int _fstat(int fd, struct stat *buf) {
  return 0;
}

int _kill(int pid, int sig) {
  _exit(-SYS_kill);
  return -1;
}

pid_t _getpid() {
  _exit(-SYS_getpid);
  return 1;
}
