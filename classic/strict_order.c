#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PHIL 5
#define LEFT (phil_id + NUM_PHIL-1) % NUM_PHIL
#define RIGHT (phil_id + 1) % NUM_PHIL

pthread_mutex_t forks[NUM_PHIL];

pthread_t philosophers[NUM_PHIL];

int ids[NUM_PHIL] = {0,1,2,3,4};

void* philosopher(void* id){
    int phil_id = *((int*) id);
    while (1){
        printf("Philosopher %d started thinking\n",phil_id);
        sleep(1);
        printf("Philosopher %d started thinking\n",phil_id);

        if (phil_id==0){
            pthread_mutex_lock(&forks[RIGHT]);
            printf("Philosopher %d picked up fork %d\n",phil_id, RIGHT);

            pthread_mutex_lock(&forks[LEFT]);
            printf("Philosopher %d picked up fork %d\n",phil_id, LEFT);

        } else {
            pthread_mutex_lock(&forks[LEFT]);
            printf("Philosopher %d picked up fork %d\n",phil_id, LEFT);

            pthread_mutex_lock(&forks[RIGHT]);
            printf("Philosopher %d picked up fork %d\n",phil_id, RIGHT);
        }

        printf("Philosopher %d started eating\n",phil_id);
        sleep(1);
        printf("Philosopher %d finished eating\n",phil_id);


        if (phil_id==0){
            pthread_mutex_unlock(&forks[RIGHT]);
            printf("Philosopher %d has put down fork %d\n",phil_id, RIGHT);

            pthread_mutex_unlock(&forks[LEFT]);
            printf("Philosopher %d has put down fork %d\n",phil_id, LEFT);
        } else {
            pthread_mutex_unlock(&forks[LEFT]);
            printf("Philosopher %d has put down fork %d\n",phil_id, LEFT);

            pthread_mutex_unlock(&forks[RIGHT]);
            printf("Philosopher %d has put down fork %d\n",phil_id, RIGHT);
        }
    }
}

int main(){
    for (int i=0; i<NUM_PHIL; i++){
        pthread_mutex_init(&forks[i],NULL);
    }

    for (int i=0; i<NUM_PHIL; i++){
        pthread_create(&philosophers[i],NULL,philosopher,&ids[i]);
    }

    for (int i=0; i<NUM_PHIL; i++){
        pthread_join(philosophers[i],NULL);
    }

    for (int i=0; i<NUM_PHIL; i++){
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}