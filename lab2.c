#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int main()
{
	time_t seconds = time(NULL);
    struct tm *cur_Time = localtime(&seconds);

    pid_t cur_Pid;
    int proc_num = pow(2, cur_Time->tm_hour);
    pid_t pid_num[proc_num];
    int i = 0;

    for(int i; i < proc_num; i++)
    {
       
       switch( cur_Pid = fork())
       {
          case -1:

             perror("An error occured");
             exit(-1);

          case 0:

             printf("Process №%d sin = %lf\n",i, sin(cur_Time->tm_min) );
             exit(0);

          default:

             wait(NULL);
             pid_num[i] = cur_Pid; 
         }

     }
    printf("Parent process sin = %lf\n", sin(cur_Time->tm_min) );
    
	while(i < proc_num)
	{
		waitpid(pid_num[i],NULL,0);
		i++;
	}

	return 0;
}
