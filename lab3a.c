#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void getTime(struct tm *timeinfo);

int main()
{
 time_t sec;
 struct tm *timeinfo;
 time (&sec);
 timeinfo = localtime(&sec);
 
 
 int fd[2];
 int p = pipe(fd);
 char buff[128];
 strftime(buff,128,"%H:%M:%S",timeinfo);
 
 if( p==(-1))
 {
   perror("Error");
   exit(-1);
 }
 else 
{
  switch(fork())
 {
    case -1:
       perror("Error");
       exit(-1);
    case 0:
        write(fd[0],buff, sizeof(buff));
        printf("Start Time: %s\n", buff);
        exit(0);
    default:
        sleep (5);
        time (&sec);
        timeinfo = localtime(&sec);
        strftime(buff,128,"%H:%M:%S",timeinfo);
        read(fd[1],buff, sizeof(buff));
        printf("End Time: %s",buff);
  }
 
}
return 0;
}
