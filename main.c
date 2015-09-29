#define DEF_STRING_LENGTH 100
#define DEF_COUNT 10
#define $ //printf("!\n");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct COMMAND{
    char* string;
    char** argarr;
    int time;
    int counter;
};

void Split(char* string, char* delimeters, char*** tokens, int* tokensCount);
int cmp(const void* a, const void* b);

int main() {
    $
    int numberofcomands;
    scanf("%d\n", &numberofcomands);
    struct COMMAND *arr = (struct COMMAND*)calloc(numberofcomands,sizeof(struct COMMAND));
    for (int i = 0; i < numberofcomands; i++) {
        arr[i].string = (char*)calloc(DEF_STRING_LENGTH, sizeof(char)); $
        arr[i].argarr = (char**)calloc(DEF_COUNT, sizeof(char*)); $
        fgets(arr[i].string, DEF_STRING_LENGTH - 1, stdin); $ //printf("%s", arr[i].string);
        Split(arr[i].string, " \n" , &arr[i].argarr, &arr[i].counter); $ //printf("%d", arr[i].counter);
        arr[i].time = atoi(arr[i].argarr[arr[i].counter - 1]); $ //printf("%d\n", arr[i].time);
        arr[i].argarr[arr[i].counter - 1] = NULL; $
        
     }
    qsort((void*)arr, numberofcomands, sizeof(struct COMMAND), cmp); $
    
    sleep(arr[0].time);

    for (int j = 0; j < numberofcomands; j++){
        int n = fork(); $
        if( n == 0){
            execvp(arr[j].argarr[0], arr[j].argarr);
        }
        if(j != numberofcomands - 1)
        {
            sleep(arr[j + 1].time - arr[j].time);
        }
    }
    
    return 0;
}

int cmp(const void* a, const void* b){
    return ((struct COMMAND *)a)->time - ((struct COMMAND *)b)->time;
}



void Split(char* string, char* delimeters, char*** tokens, int* tokensCount)
{
    
    *tokens = (char**)calloc(DEF_COUNT, sizeof(char*));
    size_t allocated = DEF_COUNT;
    *tokensCount = 0;
    char* word;
    for(word = strtok(string, delimeters); word; word = strtok(NULL, delimeters))
    {
        if(*tokensCount + 1 > allocated)
        {
            allocated *= 2;
            *tokens = (char**)realloc(tokens, allocated * sizeof(char*));
        }
        (*tokens)[*tokensCount] = (char*)malloc(strlen(word) * sizeof(char));
        strcpy((*tokens)[*tokensCount], word);
        ++*tokensCount;
    }
}