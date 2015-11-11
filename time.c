#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int t;
void my_handler(int nsig);

int main(void){
    signal(SIGALRM, my_handler);
    while(1){
        scanf("%d", &t);
        if(t < 0)
        break;
        alarm(1);
    }
    return 0;
}

void my_handler(int nsig){
    if (nsig == SIGALRM) {
        if (t > 0){
            t--;
            printf("time: %d\a\n", t);
            alarm(1);
        }
        else{
            printf("\a\a\a\a\a\n");
        }
    }
}