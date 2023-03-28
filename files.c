#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

char* validOptions = "ndhmal";

void print_menu_file()
{
    printf("A) regular file\n");
    printf("-n: file name\n");
    printf("-d: file size\n");
    printf("-h: number of hard links\n");
    printf("-m: time of last modification\n");
    printf("-a: access rights\n");
    printf("-l: create a symbolic link\n");
}

void handle_regular_file(char *filename)
{
    char* option;
    print_menu_file();
    for(int i=1;i<strlen(option);i++)
    {
        if(strchr(validOptions,option[i]))
        {
            

        }
    }
    
}

void print_menu_symbolic()
{
    printf("B) symbolic link\n");
    printf("-n: link name\n");
    printf("-l: delete link\n");
    printf("-d: size of the link\n");
    printf("-t: size of the target\n");
    printf("-a: access rights\n");
}

void handle_symbolic_link(char *filename)
{
    char *option;
    print_menu_symbolic();
}

int main(int argc, char *argv[])
{
    struct stat filestat;
    for (int i = 1; i < argc; i++) 
    {
        if(lstat(argv[i], &filestat) < 0) 
        {
            printf("Error: unable to stat %s\n", argv[i]);
            continue;
        }
        if(S_ISREG(filestat.st_mode)) 
        {
            handle_regular_file(argv[i]);
        } 
        else if(S_ISLNK(filestat.st_mode)) 
        {
            handle_symbolic_link(argv[i]);
        } 
        else if(S_ISDIR(filestat.st_mode))
        {
            printf("Directory: %s\n", argv[i]);
        } 
        else 
        {
            printf("Unknown file type: %s\n", argv[i]);
        }
    }
    return 0;
}
