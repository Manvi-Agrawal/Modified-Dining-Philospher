#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PHIL 5
#define LEFT (phil_id + NUM_PHIL-1) % NUM_PHIL
#define RIGHT (phil_id + 1) % NUM_PHIL

pthread_mutex_t forks[NUM_PHIL];
pthread_mutex_t bowls[2];

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

        // At a time one from group G1(P0, P2, P4) and group G2(P1, P3) can eat
        // So, allow G1 to access bowl B1 if available
        // And allow G2 to access bowl B2 if available
        // Note: This bowl logic should be executed after forks are taken
        // Since fork mutex ordering is creating this condition
        if (phil_id%2 == 0){
            // Group G1 will try accessing B1
            pthread_mutex_lock(&bowls[0]);
            printf("Philosopher %d picked up bowl 0\n",phil_id);
        } else {
            // Group G2 will try accessing B1
            pthread_mutex_lock(&bowls[1]);
            printf("Philosopher %d picked up bowl 1\n",phil_id);
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
        // At a time one from group G1(P0, P2, P4) and group G2(P1, P3) can eat
        // So, allow G1 to access bowl B1 if available
        // And allow G2 to access bowl B2 if available
        // Note: This bowl logic should be executed after forks are realeased
        // Since fork mutex ordering is creating this condition for bowl
        if (phil_id%2 == 0){
            // Group G1 will try accessing B1
            pthread_mutex_unlock(&bowls[0]);
            printf("Philosopher %d has put down bowl 0\n",phil_id);
        } else {
            // Group G2 will try accessing B1
            pthread_mutex_unlock(&bowls[1]);
            printf("Philosopher %d has put down bowl 1\n",phil_id);
        }

    }
}

int main(){
    for (int i=0; i<NUM_PHIL; i++){
        pthread_mutex_init(&forks[i],NULL);
    }
    for (int i=0; i<2; i++){
        pthread_mutex_init(&bowls[i],NULL);
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

    for (int i=0; i<2; i++){
        pthread_mutex_destroy(&bowls[i]);
    }

    return 0;
}