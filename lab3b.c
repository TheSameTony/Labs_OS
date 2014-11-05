#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


int main()
{

 time_t sec;
 struct tm *timeinfo;
 time (&sec);
 timeinfo = localtime(&sec);

 int fd;
 char path[256] = "FIFO";
 char buff[128];
 char read_buff[128];
 strftime(buff,128,"%H:%M:%S",timeinfo);
 
 mknod(path, 0777, 0);

 fd = open(path, O_RDWR);

  switch(fork())
 {
    case -1:
       perror("Error");
       exit(-1);
    case 0:
        write( fd, buff, sizeof(buff));
        printf("Start Time: %s\n", buff);
        exit(0);
    default:
        sleep (5);
        read( fd, &read_buff, sizeof(read_buff));
        printf("Start Time from FIFO: %s",read_buff);
  }
return 0;
}
