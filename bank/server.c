//Compile with -Wformat
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define MAGIC_CONST 1000

int money;
int semid;
void* mythread (void* arg);

int main(int argc, char **argv)
{
    int sockfd;
    key_t key;
    const char pathname[] = "server.c";
    money = 10000;
    struct sembuf mybuf;
    int clilen;
    
    struct sockaddr_in servaddr, cliaddr;
    
    if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }
    
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }
    
    mybuf.sem_op  = 1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
    
    if (semop(semid , &mybuf , 1) < 0) {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
    
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51002);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    if(listen(sockfd, 5) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    
    while(1)
    {
        int* newsockfd = (int*)calloc(1, sizeof(int));
        clilen = sizeof(cliaddr);
        if((*newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        pthread_t thread_id;
        if(pthread_create(&thread_id, (pthread_attr_t*)NULL, mythread, (void*)newsockfd) < 0)
        {
            printf("Can't create a thread!\n");
            exit(-1);
        }
        
    }
    
    
    close(sockfd);
    return 0;
}



void* mythread (void* arg){
    int* newsockfd = (int*)arg;
    char line[MAGIC_CONST];
    int n;
    struct sembuf mybuf;
    sprintf(line, "∩༼˵☯‿☯˵༽つ¤=[]:::::> Our balance is %d¥", money);
    if((n = write(*newsockfd, line, strlen(line) + 1)) < 0)
    {
        perror(NULL);
        close(*newsockfd);
        exit(1);
    }
    
    while((n = read(*newsockfd, line, MAGIC_CONST)) > 0)
    {
        int count = atoi(line);
        
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        
        if (semop(semid , &mybuf , 1) < 0) {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
        if (money + count < 0) {
            sprintf(line, "┏༼ ◉ ╭╮ ◉༽┓ balance: %d¥", money);
        }
        else{
            money += count;
            sprintf(line, "╰( ⁰ ਊ ⁰ )━☆ﾟ.*･｡ﾟ balance: %d¥", money);
        }
        
        mybuf.sem_op =  1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        
        if (semop(semid , &mybuf , 1) < 0) {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
        if((n = write(*newsockfd, line, strlen(line) + 1)) < 0)
        {
            perror(NULL);
            close(*newsockfd);
            exit(1);
        }
    }
    
    return NULL;
}
