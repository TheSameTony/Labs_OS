#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

int fd;

void *read_func(void *arg)
{
  char read_buff[128];
  read( fd, &read_buff, sizeof(read_buff));
  printf("Start Time from FIFO: %s\n",read_buff);
}

void *write_func(void *arg)
{
	char write_buff[128];
    time_t sec;
	struct tm *timeinfo;
	time (&sec);
	timeinfo = localtime(&sec);
    strftime(write_buff,128,"%H:%M:%S",timeinfo);

    write( fd, write_buff, sizeof(write_buff));
    printf("Start Time: %s\n", write_buff);
}

int main()
{
	char path[256] = "FIFO";
    int result, r_id, w_id;

	pthread_t r_thread, w_thread;
	
	
	if( mkfifo(path, 0777) ==0)
	{
	fd = open(path, O_RDWR);

	w_id = 1;
    result = pthread_create(&w_thread, NULL, write_func, &w_id);
    if(result != 0 )
    {
    	perror("Creating write thread");
        return EXIT_FAILURE;
    }
    else
        printf("Creating write thread - Success\n");

    sleep(1);

    r_id = 2;
    result = pthread_create(&r_thread, NULL, read_func, &r_id);
    if(result != 0 )
    {
    	perror("Creating read thread");
        return EXIT_FAILURE;
    }
    else
        printf("Creating read thread - Success\n");

    sleep(1);
	}
	else
	  perror("FIFO error:");

	return 0;
}
