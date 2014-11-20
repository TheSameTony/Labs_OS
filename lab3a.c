#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


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
        read(fd[1],buff, sizeof(buff));
        printf("Start Time from pipe: %s",buff);
  }
 
}
return 0;
}
