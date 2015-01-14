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
	pthread_rwlock_t *rwlock;
};

void *thread_func(void *arg)
{
	time_t Time;
	char *shm;
	struct thread_data *data;

	data = (struct thread_data *)arg;
	pthread_rwlock_wrlock(data->rwlock);
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
	pthread_rwlock_unlock(data->rwlock);

}

int main()
{
    int shmid;
	char *shm;
	pthread_t thread;
	time_t Time, time_after;

	pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
	if(pthread_rwlock_init(&lock, NULL) != 0)
	{
		perror("pthread_rwlock_init");
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
	data.shmid = shmid;
	data.rwlock = &lock;

	if(pthread_create(&thread, NULL, thread_func, &data) == 0)
	{
		time(&Time);

		sleep(1);

		if(pthread_rwlock_rdlock(&lock) == 0)
		{
			time(&time_after);
			double time_spent = difftime(time_after, Time);
			printf("%.f seconds waited\n", time_spent);
			printf("Main thread time: %s", ctime(&Time));
			printf("Thread time from Sh. Mem.: %s\n", shm);
		}
		else 
		{
			perror("pthread_rwlock_rdlock");
			exit(1);
		}

		shmdt(shm);
	}
	else 
	{
		perror("pthread_create");
		exit(1);
	}


	return 0;
}
