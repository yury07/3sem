/* не могу найти ошибку в вычисление дисперсии, помогите пожалуйста*/

/*
 * Вы время странно засекали для дисперсии, поэтому казалось, что на её вычисление тратится в 2-3 раза больше времени, чем на
 * вычисление дисперсии.
 * Результат вроде бы верный вычисляется, и вроде даже ускорение видно (с помощью утилиты time замерял)
 * Засчитано.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define N 100000000
#define K 2

struct Data{
    int size;
    double *t;
    double result;
};

void* sum(void*);
void* dispersion(void* p);

int main(){
    
    srand(time(NULL));
    double *arr = (double*)calloc(N, sizeof(double));
    
    for (int i = 0; i < N; i++) {
        //arr[i] = (double)rand() / RAND_MAX*(100) - 50;
        arr[i] = rand() % 2;
    }
    
    struct Data *str = (struct Data*) calloc(K , sizeof(struct Data));
    
    for (int j = 0 ; j < K; j++) {
        str[j].t = arr + j * N / K;
        str[j].size = N / K;
        str[j].result = 0;
    }
    
    pthread_t thread_id[K];
    unsigned int cl1 = clock();
    for (int i = 0; i < K; i++) {
        int result = pthread_create( &thread_id[i], (pthread_attr_t*) NULL, sum, (void*)&str[i]);
        if (result > 0) {
            printf("Error");
            exit(0);
        }
    }
    
    double av = 0;
    for (int i = 0; i < K; i++) {
        pthread_join(thread_id[i], NULL);
        printf("* no %d, start %p, size %d, sum %lg, aver %f\n", i, str[i].t, str[i].size, str[i].result, str[i].result / str[i].size);
        av += str[i].result;
    }
    
    av /= N;
    printf("Averange: %4.4lg\n", av);
    printf ( "Averange time: %ld\n", clock() - cl1);
    
    for (int j = 0 ; j < K; j++) {
        str[j].result = 0;
    }
    
    unsigned int dispersionStartTime = clock();
    
    for (int i = 0; i < K; i++) {
        int result = pthread_create( &(thread_id[i]), (pthread_attr_t*) NULL, dispersion, (void*)&str[i]);
        if (result > 0){
            printf("Error");
            exit(0);
        }
    }
    double dis = 0;
    
    for (int i = 0; i < K; i++) {
        pthread_join(thread_id[i], NULL);
        dis += str[i].result;
    }
    
    dis = dis / N - av * av;
    printf("Dispersion: %4.4lg\n", dis);
    printf("Dispersion time: %ld\n",clock() - dispersionStartTime);
    
    free(arr);
    free(str);
    
    return 0;
}

void* sum(void* p){
    double* start = ((struct Data *)p) ->t;
    double result = 0;
    int size = ((struct Data *)p) ->size;
    
    for( int i = 0; i < size; i++){
        result += start[i];
    }
    
    ((struct Data*)p)->result = result;
    return NULL;
}

void* dispersion(void* p){
    double* start = ((struct Data *)p) ->t;
    double result = 0;
    int size = ((struct Data *)p) ->size;
    
    for( int i = 0; i < size; i++){
        result += start[i] * start[i];
    }
    
    ((struct Data*)p)->result = result;
    return NULL;
}












