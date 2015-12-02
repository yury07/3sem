#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define SEND_TYPE 228

int main()
{
    struct mymsgbuf
    {
        long mtype;
        double x;
        double y;
        long pid;
        
    } mybuf;
    
    int msqid;
    char pathname[] = "client.c";
    key_t key;
    size_t size = sizeof(mybuf) - sizeof(long);
    
    
    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    
    scanf("%lg %lg", &(mybuf.x), &(mybuf.y));
    mybuf.mtype = SEND_TYPE;
    mybuf.pid = getpid();
    
    if (msgsnd(msqid, (struct msgbuf*)&mybuf, size, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    
    if ((msgrcv(msqid, (struct msgbuf *)&mybuf, size, mybuf.pid, 0)) < 0)
    {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("%lg\n", mybuf.x);
    
    return 0;
}