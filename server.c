#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * В целом всё нормально. Формартирование хорошее.
 * Работает почти правильно, за исключением небольшого замечения про передачу символа конца строки.
 * Можно попридираться по поводу именования переменных, наличия в коде магических чисел, которые надо выносить в отдельные константы,
 * разбиение кода на отдельные ф-и. Не всё сразу. Засчитано.
 */

#define K 100

struct USER{
    int adr;
    int port;
};

int main()
{
    int sockfd;
    int n, clilen;
    char line[1000];
    struct sockaddr_in servaddr, cliaddr;
    struct USER *usr = (struct USER*) calloc(K , sizeof(struct USER));
    int user_number = 0;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    } else
    {
        printf("Sock fd:%d\n", sockfd);
    }
    
    if (bind(sockfd, (struct sockaddr*) &servaddr,
             sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    while (1)
    {
	/*
	 * Вы передаете переменную clilen по указателю в ф-ю recvfrom для того, чтобы в неё записался некоторый результат, в частности, 
	 * размер принятой структуры. Поэтому задавать самим некоторое значение в эту переменную совсем не обязательно.
	 */
        clilen = sizeof(cliaddr);
        if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        printf("%s\n", line);
        int userIndex = -1;
        for (int i = 0; i < user_number; i++){
            if (cliaddr.sin_addr.s_addr == usr[i].adr && cliaddr.sin_port == usr[i].port) {
                userIndex = i;
            }
        }
        if (userIndex < 0){
            usr[user_number].adr = cliaddr.sin_addr.s_addr;
            usr[user_number].port = cliaddr.sin_port;
            user_number++;
        }
        else
        {
            for (int i = 0; i < user_number; i++) {
                if (i != userIndex){
                    cliaddr.sin_addr.s_addr = usr[i].adr;
                    cliaddr.sin_port = usr[i].port ;
		    /*
		     * Вы не пересылаете символ конца строки из-за чего, если вы отправляете строки "123456", а потом "abc", то
		     * будут приняты "123456" и "abc456", т.е. надо отправлять strlen(line) + 1 символ.
		     */
                    if (sendto(sockfd, line, strlen(line), 0,
                               (struct sockaddr*)&cliaddr, clilen) < 0)
                    {
                        perror(NULL);
                        close(sockfd);
                        exit(1);
                    }
                }
            }
            
        }
     
    }
    return 0;
}