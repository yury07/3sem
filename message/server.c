#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sem.h>
#include <unistd.h>

#define RECV_TYPE 228
#define N 2

/*
 * Ок. Засчитано.
 * На семинаре я задам пару вопросов по работе данной программы.
 */

int main()
{
    struct mymsgbuf
    {
        long mtype;
        double x;
        double y;
        long pid;
    } mybuf;
    
    struct sembuf mybuffer;
    int msqid;
    int semid;
    char pathname[] = "client.c";
    key_t key;
    size_t size = sizeof(mybuf) - sizeof(long);
    
    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }
    
    mybuffer.sem_op  = N;
    mybuffer.sem_flg = 0;
    mybuffer.sem_num = 0;
    
    if (semop(semid , &mybuffer , 1) < 0) {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
    
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    
    while(1){
        
        if ((msgrcv(msqid, (struct msgbuf *)&mybuf, size, RECV_TYPE, 0)) < 0)
        {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        pid_t pid;
        pid = fork();
        if (pid == 0)
        {
            mybuffer.sem_op = -1;
            mybuffer.sem_flg = 0;
            mybuffer.sem_num = 0;
            
            if (semop(semid , &mybuffer , 1) < 0) {
                printf("Can`t wait for condition\n");
                exit(-1);
            }
            
            mybuf.x = mybuf.x * mybuf.y;
            mybuf.mtype = mybuf.pid;
            sleep(7);
            
            if (msgsnd(msqid, (struct msgbuf*)&mybuf, size, 0) < 0)
            {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
                exit(-1);
            }
            
            mybuffer.sem_op =  1;
            mybuffer.sem_flg = 0;
            mybuffer.sem_num = 0;
            
            if (semop(semid , &mybuffer , 1) < 0) {
                printf("Can`t wait for condition\n");
                exit(-1);
            }
            
            return 1;
        }
    }
    
    return 0;
}