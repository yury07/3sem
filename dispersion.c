#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define N 100000000
#define K 10

struct Data{
    int size;
    double *t;
    double result;
};

void* average(void*);

int main(){
    
    srand(time(NULL));
    double *arr = (double*)calloc(N, sizeof(double));
    double *aar = (double*)calloc(N, sizeof(double));
    
    for (int i = 0; i < N; i++) {
        // arr[i] = (double)rand() / RAND_MAX*(100) - 50;
        arr[i] = 1;
        /*
         * FIXIT:
         * задаю все элементы равными 1. ожидаю, что среднее будет тоже 1, но программа выдаёт другое.
         * Где-то ошибка.
         * Заодно проверьте, что результат работы программы не зависит от количества используемых нитей.
         */
       // printf("%lg\n", arr[i]);
    }
    struct Data *str = (struct Data*) calloc(K , sizeof(struct Data));
    
    /*
     * FIXIT: 
     * N/K -> N / K
     * 
     */
    for (int j = 0 ; j < K; j++) {
        str[j].t = arr + j * N/K;
        str[j].size = N/K;
        str[j].result = 0;
    }
    
    pthread_t thread_id[K];
    unsigned int cl1 = clock();
    for (int i = 0; i < K; i++) {
        int result = pthread_create( &thread_id[i], (pthread_attr_t*) NULL, average, (void*)&str[i]);
        if (result > 0) {
            printf("Error");
            exit(0);
        }
    }
    double av = 0;
    
    for (int i = 0; i < K; i++) {
        pthread_join(thread_id[i], NULL);
        av += str[i].result;
        av /= N;
    }
    
    printf("Averange: %4.4lg\n", av);
    unsigned int cl2 = -cl1 + clock();
    printf ( "Averange time: %u\n", cl2);
    
    /*
     * FIXIT:
     * Дисперсию нужно посчитать также с использованием K нитей.
     * Вам не нужно вычислять отдельно массив aar.
     * Вы можете находу по аналогии с вычислением суммы элементов находить sum (arr[i] - av) ^ 2.
     * Можно отдельную ф-ю написать для этого, например.
     * 
     * Из-за того, что у вас цикл ниже не распараллелен, дисперсия вычисляется в разы дольше, чем среднее.
     */
    for (int i = 0; i < N; i++) {
        aar[i] = (arr[i] - av) * (arr[i] - av);
    }
    
    for (int j = 0 ; j < K; j++) {
        str[j].t = aar + j * N/K;
        str[j].size = N/K;
        str[j].result = 0;
    }
    
    for (int i = 0; i < K; i++) {
        int result = pthread_create( &thread_id[i], (pthread_attr_t*) NULL, average, (void*)&str[i]);
        if (result > 0){
            printf("Error");
            exit(0);
        }
    }
    double dis = 0;
    
    for (int i = 0; i < K; i++) {
        pthread_join(thread_id[i], NULL);
        dis += str[i].result;
        dis /= N;
    }

    printf("Dispersion: %4.4lg\n", dis);
    printf("Dispersion time: %u\n",clock() - cl2);
    
    free(arr);
    free(aar);
    free(str);
    
    return 0;
}

void* average(void* p){
    double* start = ((struct Data *)p) ->t;
    double result = 0;
    int size = ((struct Data *)p) ->size;
    
    for( int i = 0; i < size; i++){
        result += *(start + i);
    }
    
    ((struct Data*)p)->result = result;
    return NULL;
}














