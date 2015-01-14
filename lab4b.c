#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int fd[2];

void *read_func()
{
  char read_buff[128];
  sleep(2);
  read( fd[0], read_buff, sizeof(read_buff));
  printf("Start Time from Pipe: %s\n",read_buff);
}

void *write_func()
{
    char write_buff[128];
    time_t sec;
    struct tm *timeinfo;
    time (&sec);
    timeinfo = localtime(&sec);
    strftime(write_buff,128,"%H:%M:%S",timeinfo);

    write(fd[1], write_buff, sizeof(write_buff));
    printf("Start Time: %s\n", write_buff);
}

int main()
{
    pthread_t r_thread, w_thread;
    int result;

    result = pipe(fd);
    if (result != 0) 
    {
        perror("Pipe:");
        exit(1);
    }
    
    if( pthread_create(&r_thread, NULL, read_func, NULL) != 0)
    {
        perror("Creating read thread");
        return EXIT_FAILURE;
    }
    printf("Starting read thread - Success\n");

    if( pthread_create(&w_thread, NULL, write_func, NULL) !=0)
    {
        perror("Creating write thread");
        return EXIT_FAILURE;
    }
    printf("Starting write thread - Success\n");

    pthread_join(r_thread, NULL);
    pthread_join(w_thread, NULL);

    return 0;
}
