#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>


int fd[2];

void *read_func(void *arg)
{
  char read_buff[128];
  sleep(1);
  read( fd[1], &read_buff, sizeof(read_buff));
  printf("Start Time from Pipe: %s\n",read_buff);
}

void *write_func(void *arg)
{
	char write_buff[128];
    time_t sec;
	struct tm *timeinfo;
	time (&sec);
	timeinfo = localtime(&sec);
    strftime(write_buff,128,"%H:%M:%S",timeinfo);

    write(fd[0], write_buff, sizeof(write_buff));
    printf("Start Time: %s\n", write_buff);
}

int main()
{
    int w_result, r_result;
	pthread_t r_thread, w_thread;
	
    if(pipe(fd) == 0)
    {
        w_result = pthread_create(&w_thread, NULL, write_func, NULL);
            if(w_result != 0 )
            {
                perror("Creating write thread");
                return EXIT_FAILURE;
            }
            else
            {
                printf("Creating write thread - Success\n");
                sleep(1);
            }

       r_result = pthread_create(&r_thread, NULL, read_func, NULL);
            if(r_result != 0 )
            {
                perror("Creating read thread");
                return EXIT_FAILURE;
            }
            else
                printf("Creating read thread - Success\n");

            pthread_join(r_thread, NULL);
            pthread_join(w_thread, NULL);

            sleep(1);
    }
    else
    {
        perror("Pipe error:");
        return EXIT_FAILURE;
    }
    
	return 0;
}
