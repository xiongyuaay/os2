#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int flag = 0;
pid_t pid1 = -1, pid2 = -1;

void inter_handler(int signum) 
{
    // 使用 kill() 发送整数值为 16 和 17 的信号
    if (pid1 > 0) kill(pid1, 16);  // 发送信号 16 给子进程1
    if (pid2 > 0) kill(pid2, 17);  // 发送信号 17 给子进程2
    printf("Parent received signal %d, sending signals 16 and 17 to child processes\n", signum);
}

void child_handler(int signum) 
{
    if(signum == 17)
    {
        flag = 1;
    }
    printf("Child process%d received signal %d, exiting...\n", flag+1, signum);
    exit(0);
}

int main() 
{
    sigset_t parent;
    sigemptyset(&parent);
    sigaddset(&parent, SIGINT); 
    sigaddset(&parent, SIGQUIT);
    // 创建子进程1
    while (pid1 == -1) pid1 = fork();
    if (pid1 > 0) 
    {
        // 创建子进程2
        while (pid2 == -1) pid2 = fork();
        if (pid2 > 0) 
        {
            // 父进程
            signal(SIGINT, inter_handler);  
            signal(SIGQUIT, inter_handler); 
            wait(NULL);
            wait(NULL);

            printf("\nParent process is killed!!\n");
        } 
        else 
        {
            // 子进程2
            sigprocmask(SIG_BLOCK, &parent, NULL);
            signal(17, child_handler); 
            sleep(5); 
            printf("\nChild process 2 is waiting for signal 17...\n");/*  */
        }
    } 
    else 
    {
        // 子进程1
        sigprocmask(SIG_BLOCK, &parent, NULL);
        signal(16, child_handler); 
        sleep(5); 
        printf("\nChild process 1 is waiting for signal 16...\n");
    }
    return 0;
}