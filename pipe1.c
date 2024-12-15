/*管道通信实验程序残缺版 */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int pid1,pid2; // 定义两个进程变量
int main( ) 
{
    int fd[2];
    char InPipe[1000]; // 定义读缓冲区
    pipe(fd); // 创建管道
    while((pid1 = fork( )) == -1); // 如果进程 1 创建不成功,则空循环
    if(pid1 == 0) 
    { // 如果子进程 1 创建成功,pid1 为进程号
        lockf(fd[1],1,0); // 锁定管道
        char OutPipe[1000] = "Child process 1 is sending message!";
        write(fd[1], OutPipe, sizeof(OutPipe)-1);
        sleep(1); // 等待读进程读出数据
        lockf(fd[1],0,0); // 解除管道的锁定
        exit(0); // 结束进程 1
    }
    else 
    {
        while((pid2 = fork()) == -1); // 若进程 2 创建不成功,则空循环
        if(pid2 == 0) 
        {
            lockf(fd[1],1,0);
            char OutPipe[1000] = "Child process 2 is sending message!";
            write(fd[1], OutPipe, sizeof(OutPipe)-1);
            sleep(1);
            lockf(fd[1],0,0);
            exit(0);
        }
        else 
        {
            wait(0); // 等待子进程 1 结束
            wait(0); // 等待子进程 2 结束
            close(fd[1]);
            read(fd[0], InPipe, sizeof(InPipe)-1); // 从管道中读出 4000 个字符
            InPipe[999] = 0; // 加字符串结束符
            printf("%s\n",InPipe); // 显示读出的数据
            exit(0); // 父进程结束
        }
    }
    return 0;
}