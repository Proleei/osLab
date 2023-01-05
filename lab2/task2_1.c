#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int main(void) {
        pid_t pid;
        char *message;
        int n;
        pid = fork();
        if (pid < 0) {
                perror("fork failed");
                exit(1);
        }
        if (pid == 0) {
                message = "This is the child\n";
                printf("This is the child, pid = %d\n", getpid());
                n = 1000;
        } else {
                message = "This is the parent\n";
                printf("This is the parent, pid = %d\n", getpid());
                n = 5;
        }
        for(; n > 0; n--) {
                printf(message);
                sleep(1);
        }
        return 0;
 }

 /*parent may be over but child continue running*/
 /*同时，用ps命令显示进程列表，观察指定进程的进程ID和父进程
        ID，和printf输出的这些ID是否一致，并解释   yizhi*/