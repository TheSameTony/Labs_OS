#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() 
{
   
    DIR *dir;
    struct dirent *dir_entry;
    struct stat stat_buff;
    char dir_path[256];
    char full_path[512];

    printf("Type Directory path: ");
    scanf("%s", dir_path);
    strcpy(full_path, dir_path);

    dir = opendir(dir_path);
    if (dir) 
    {
        while ( (dir_entry = readdir(dir)) != NULL) 
        {

             strcat(full_path, "/");
             strcat(full_path, dir_entry->d_name);

           if( stat( full_path, &stat_buff)  != (-1) )
            {

            printf("File name: %s\n", dir_entry->d_name);
            if(dir_entry->d_type == DT_UNKNOWN)
               printf("\tType: Type is unknown\n");
            if(dir_entry->d_type == DT_REG)
               printf("\tType: Regular file\n");
            if(dir_entry->d_type == DT_DIR)
               printf("\tType: Directory\n");
            if(dir_entry->d_type == DT_FIFO)
               printf("\tType: named pipe, or FIFO\n");
            if(dir_entry->d_type == DT_SOCK)
               printf("\tType: local-domain socket\n");
            if(dir_entry->d_type == DT_CHR)
               printf("\tType: Character device\n");
            if(dir_entry->d_type == DT_BLK)
               printf("\tType: Block device\n");
            if(dir_entry->d_type == DT_LNK)
               printf("\tType: Symbolic link\n");
            printf("\tFile size: %lld bytes\n",(long long) stat_buff.st_size);
            printf("\tInode number: %llu\n", dir_entry->d_ino);
            printf("\tPreferred I/O block size: %ld bytes\n", (long) stat_buff.st_blksize);
            printf("\tLast status change: %s", ctime(&stat_buff.st_ctime));
            printf("\tLast file access: %s", ctime(&stat_buff.st_atime));
            printf("\tLast file modification: %s", ctime(&stat_buff.st_mtime));

            strcpy(full_path, dir_path);
          }
        }
        
       closedir(dir);

    } 
    
    else 
    {
        perror("The following error occurred");
        exit(1);
    }

    return 0;
};
