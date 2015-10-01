#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 10

/*
 * В конце презентации к семинару 5 есть бонусное задание про бинарное дерево процессов. 
 * Оно ваше.
 */

int main()
{
    int pid, i, status;
    
    for(i = 0; i < N; i++)
    {
        pid = fork();
        
        sleep(2);
        
        if(pid == -1)
        {
            printf("ERROR!\n");
        }
        else if(pid == 0)
        {
            continue;
        }
        break;
    }
    printf("pid = %d, ppid = %d\n" , getpid(), getppid());
    pid = wait(&status);
    printf("Process %d exited with status %d and returned %d\n", pid, status & 255, (status & 65280) >> 8);
    return getpid();
}
