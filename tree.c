#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/*
 * Хорошо. Засчитано.
 * 1. называйте переменные со строчной буквы везде (Name, First)->(name, first)
 * 2. Объявляйте соответствующие константы вместо магических чисел: #define MAX_STRING_SIZE 100
 * 3. давайте более содержательные название переменным нежели просто "a".
 * 
 * Дома у вас больше времени нежели на семинаре, поэтому старайтесь следить из за этим тоже.
 * Вы уже нормально форматируете код. Если не будете ставить лишние пустые строки, то будет совсем здорово.
 */

void search(int a, char* Name, char* First, char* result);

int main(int argc, char *argv[], char *envp[]) {
    char str[100] = "";
    
    search(atoi(argv[2]), argv[3], argv[1], str);
    printf("%s", str);
   
    return 0;
}

void search(int a, char* Name, char* First, char* result){
    
    if (a < 0) {
        return;
    }
   // printf("scanning %s\n", First);
    DIR* dir = opendir(First);
    struct dirent* entity;
    struct stat st;
    while ((entity = readdir(dir)) != NULL){
        char str[100];
        strcpy(str, First);
        strcat(str, "/");
        strcat(str, entity->d_name);
        stat(str, &st);
        if(S_ISREG(st.st_mode)){
            if (strcmp(entity->d_name, Name) == 0) {
                strcpy(result, str);
            }
            return;
        }
        if(S_ISDIR(st.st_mode) && (strcmp(entity->d_name, ".") != 0) && (strcmp(entity->d_name, "..") != 0)){
            search(a - 1, Name, str, result);
        }
        
    }
    

}