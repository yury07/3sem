#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define DEF_TOKENS_COUNT 10
#define MAX_DELIMETRS_COUNT 200
#define MAX_STRING_LENGTH 1000

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount);

int main()
{
    int tokensCount = 0;
    char** tokens = NULL;
    char *string = (char *) malloc(sizeof(char) * MAX_STRING_LENGTH);
    char *delimiters = (char *) malloc(sizeof(char) * MAX_DELIMETRS_COUNT);
    
    
    gets(string);
    
    gets(delimiters);
    
    
    
    Split(string, delimiters, &tokens, &tokensCount);
    
    for(int i = 0; i < tokensCount; i++)
    {
        printf("%s\n", tokens[i]);
    }
    
    return 0;
}

void Split(char* string, char* delimeters, char*** tokens, int* tokensCount)
{
    
    *tokens = (char**)calloc(DEF_TOKENS_COUNT, sizeof(char*));
    size_t allocated = DEF_TOKENS_COUNT;
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







