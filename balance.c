#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define ARRAY_SIZE 10
#define WORKERS_COUNT 10
#define PAUSE_LENGTH 1

///////
static void *worker_thread(void *v_data)
{
    struct vars *data = v_data;
    unsigned int randseed = data->randseed;
    int *array = data->array;
    pthread_mutex_t *mutex = data->mutex;
    sem_t *data_sem = data->data_sem;
    sem_post(data_sem);  /* tell the main thread we don't need
                            the v_data structure anymore */
    for(;;) {
        int idx1, idx2;
        idx1 = rand_r(&randseed) % ARRAY_SIZE;
        idx2 = rand_r(&randseed) % ARRAY_SIZE;
        if(idx1 == idx2)
            continue;
        pthread_mutex_lock(mutex);
        if(rand_r(&randseed) & 0x10) {
            array[idx1]++;
            array[idx2]--;
        } else {
            array[idx2]--;
            array[idx1]++;
        }
        pthread_mutex_unlock(mutex);
    }
    return NULL;  /* never reached */
}



int _main()
{
    int 			i;
    struct vars tsdata;

    int				the_array[ARRAY_SIZE];
    pthread_t		thr;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    sem_t			tsd_sem;    

    for(i = 0; i < ARRAY_SIZE; i++)
        the_array[i] = 0;

    tsdata.randseed = time(NULL);
    tsdata.array = the_array;
    tsdata.mutex = &mutex;
    tsdata.data_sem = &tsd_sem;
    sem_init(&tsd_sem, 0, 0);

    for(i = 0; i < WORKERS_COUNT; i++) {
        tsdata.randseed++;
        pthread_create(&thr, NULL, worker_thread, &tsdata);
        sem_wait(&tsd_sem);
    }

    for(;;) {
        int sum;
        pthread_mutex_lock(&mutex);
        sum = 0;
        for(i = 0; i < ARRAY_SIZE; i++)
            sum += the_array[i];
        printf("%d ", sum);
        for(i = 0; i < ARRAY_SIZE; i++)
            printf("%c%d", i ? ',' : '(', the_array[i]);
        printf(")\n");
        pthread_mutex_unlock(&mutex);
        sleep(PAUSE_LENGTH);
    }
    return 0;
}


int main(int ac, char **av)
{
	return(_main(ac, av));
}























