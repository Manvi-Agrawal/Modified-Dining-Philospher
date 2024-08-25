#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5

#define THINKING 2
#define HUNGRY 1
#define EATING 0

#define BOWL_AVAILABLE 1
#define BOWL_OCCUPIED 0

#define LEFT (phil_id + 4) % N
#define RIGHT (phil_id + 1) % N

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

int bowl[2] = {BOWL_AVAILABLE, BOWL_AVAILABLE}; // 2 bowls
int phil_bowl[N] = {-1, -1, -1, -1, -1};


sem_t mutex; // To prevent philosphers from taking and putting forks at the same time
sem_t philState[N];

void think(void* id) {
    int phil_id = *((int*) id);

    printf("Philosopher %d started thinking\n",phil_id );
	sleep(3);
    printf("Philosopher %d stopped thinking\n",phil_id );
}

void eat(void* id) {
    int phil_id = *((int*) id);

    printf("Philosopher %d started eating\n",phil_id );
	sleep(2);
    printf("Philosopher %d finished eating\n",phil_id );

}

void test(int phil_id)
{
	if (state[phil_id] == HUNGRY
		&& state[LEFT] != EATING
		&& state[RIGHT] != EATING) {
		// state that eating
		state[phil_id] = EATING;

		sleep(2);

		printf("Philosopher %d takes fork %d and %d\n", phil_id, LEFT, RIGHT);

        if(bowl[0] == BOWL_AVAILABLE) {
		    printf("Philosopher %d takes bowl 0\n", phil_id);
            bowl[0] = BOWL_OCCUPIED;
            phil_bowl[phil_id] = 0;
        } else {
		    printf("Philosopher %d takes bowl 1\n", phil_id);
            bowl[1] = BOWL_OCCUPIED;
            phil_bowl[phil_id] = 1;
        }


		// sem_post(&philState[phil_id]) has no effectduring takefork
		// used to wake up hungry philosophers during putfork
		sem_post(&philState[phil_id]);
	}
}

// take up chopsticks
void take_fork(void* id)
{
    int phil_id = *((int*) id);


	sem_wait(&mutex);

	// state that hungry
	state[phil_id] = HUNGRY;

	printf("Philosopher %d is Hungry\n", phil_id);

	// eat if neighbours are not eating
	test(phil_id);

	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&philState[phil_id]);

	sleep(1);
}

// put down chopsticks
void put_fork(void* id)
{
    int phil_id = *((int*) id);

	sem_wait(&mutex);

	// state that thinking
	state[phil_id] = THINKING;

	printf("Philosopher %d putting fork %d and %d down\n", phil_id, LEFT, RIGHT);

	printf("Philosopher %d puts down bowl %d\n", phil_id, phil_bowl[phil_id]);
    bowl[phil_bowl[phil_id]] = BOWL_AVAILABLE;
    phil_bowl[phil_id] = -1;


	test(LEFT);
	test(RIGHT);

	sem_post(&mutex);
}

void* philosopher(void* num)
{

	while (1) {
        think(num);

		take_fork(num);

        eat(num);

		put_fork(num);
	}
}

int main()
{

	int i;
	pthread_t thread_id[N];

	// initialize the semaphores
	sem_init(&mutex, 0, 1);

	for (i = 0; i < N; i++)

		sem_init(&philState[i], 0, 0);

	for (i = 0; i < N; i++) {

		// create philosopher processes
		pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);

	}

	for (i = 0; i < N; i++)

		pthread_join(thread_id[i], NULL);
}
