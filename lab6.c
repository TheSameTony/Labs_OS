#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <fcntl.h>

struct thread_data
{
	int shmid;
	pthread_mutex_t *mutex;
};

void *thread_func(void *arg)
{
	time_t Time;
	char *shm;
	struct thread_data *data;

	data = (struct thread_data *)arg;
	pthread_mutex_lock(data->mutex);
	sleep(2);

	shm = (char *)shmat(data->shmid, NULL, 0);
	if(shm == (char *)(-1))
	{
		perror("shmat");
		exit(1);
	}

	time(&Time);
	sprintf(shm, ctime(&Time));
	printf("Thread time: %s", shm);
	pthread_mutex_unlock(data->mutex);

}

int main()
{
    int shmid;
	char *shm;
	pthread_t thread;
	time_t Time;

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	if(pthread_mutex_init(&mutex, NULL) != 0)
	{
		perror("pthread_mutex_init");
		exit(1);
	}

	shmid = shmget(IPC_PRIVATE, sizeof(time_t), IPC_CREAT | 0666);
	if(shmid == -1)
	{
		perror("shmget");
		exit(1);
	}

	shm = (char *)shmat(shmid, NULL, 0);
	if(shm == (char *)(-1))
	{
		perror("shmat");
		exit(1);
	}

	struct thread_data data;
	data.mutex = &mutex;
	data.shmid = shmid;

	if(pthread_create(&thread, NULL, thread_func, &data) == 0)
	{
		time(&Time);

		sleep(1);

		while(pthread_mutex_trylock(&mutex) != 0)
		{
			printf("Waiting...\n");
			sleep(1);
		}

		printf("Main thread time: %s", ctime(&Time));
		printf("Thread time from Sh. Mem.: %s\n", shm);
		shmdt(shm);
	}
	else 
	{
		perror("pthread_create");
		exit(1);
	}


	return 0;
}
