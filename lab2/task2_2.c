#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int main(void) {
        pid_t pid;
        pid = fork();
        if (pid < 0) {
                perror("fork failed");
                exit(1);
        }
        if (pid == 0) {
                printf("This is the child, pid = %d\n", getpid());
                sleep(5);
                exit(0);
        } else {
                printf("This is the parent, pid = %d\n", getpid());
                int res = wait();
                printf("Child exit ret number : %d\n", res);
        }
        return 0;
 }

 /*parent may be over but child continue running*/
 /*父进程用wait函数。子进程休眠5秒，父进程不休眠。子进程
用exit返回参数。父进程中printf子进程返回的参数*/