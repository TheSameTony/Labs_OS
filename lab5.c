#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char const *argv[])
{
	int shmid, status;
	char *shm;

	time_t p_time, c_time;

	shmid = shmget(IPC_PRIVATE, sizeof(time_t), IPC_CREAT | IPC_EXCL | 0666);
	if(shmid == -1)
	{
		perror("shmget");
		exit(1);
	}

	shm = (char*)shmat(shmid, NULL, 0);
	if(shm == (char*)(-1))
	{
		perror("shmat");
		exit(1);
	}

	switch( fork() )
	{
		case -1:
			perror("fork");
			exit(1);
		case 0:
			sleep(3);
			time(&c_time);

			shm = (char*)shmat(shmid, NULL, 0);
			if(shm == (char*)(-1))
			{
				perror("shmat");
				exit(1);
			}

			sprintf(shm, ctime(&c_time));
			exit(0);
		default:
			time(&p_time);
			wait(&status);

			printf("Parent process start time: %s\n", ctime(&p_time));
			printf("Child process start time: %s\n", shm);

			shmdt(shm);
	}

	return 0;
}
