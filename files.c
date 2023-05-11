#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

/*Used to extract the name from a given path.*/
char *getName(char *path)
{
    char *name = strrchr(path, '/');
    if (name == NULL)
    {
        name = path;
    }
    else
    {
        name++;
    }
    return name;
}

/*Regular file menu display.*/
void print_menu_file()
{
    printf("A) Regular file:\n");
    printf("-n: file name\n");
    printf("-d: file size\n");
    printf("-h: number of hard links\n");
    printf("-m: time of last modification\n");
    printf("-a: access rights\n");
    printf("-l: create a symbolic link, give: link name\n");
}

/*Regular file handling.*/
void handle_regular_file(char *filename)
{
    char option[10];
    start_file:
    print_menu_file();
    printf("Choose an option from the menu:\n");
    scanf("%s", option);
    struct stat filestat;
    for (int i = 1; i < strlen(option); i++)
    {
        switch (option[i])
        {
        case 'n':
        {
            printf("File name: %s\n", getName(filename));
            break;
        }
        case 'd':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("Error: unable to stat %s\n", filename);
                break;
            }
            printf("File size: %lld bytes\n", (long long)filestat.st_size);
            break;
        }
        case 'h':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("Error: unable to stat %s\n", filename);
                break;
            }
            printf("Number of hard links: %ld\n", (long)filestat.st_nlink);
            break;
        }
        case 'm':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("Error: unable to stat %s\n", filename);
                break;
            }
            printf("Time of last modification: %s", ctime(&filestat.st_mtime));
            break;
        }
        case 'a':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("Error: unable to stat %s\n", filename);
                break;
            }
            printf("Access rights:\n");
            printf("\n");
            printf("User:\n");
            printf("\n");
            printf((filestat.st_mode & S_IRUSR) ? "Read-yes\n" : "Read-yes\n");
            printf((filestat.st_mode & S_IWUSR) ? "Write-yes\n" : "Write-no\n");
            printf((filestat.st_mode & S_IXUSR) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            printf("Group:\n");
            printf("\n");
            printf((filestat.st_mode & S_IRGRP) ? "Read-yes\n" : "Read-yes\n");
            printf((filestat.st_mode & S_IWGRP) ? "Write-yes\n" : "Rrite-no\n");
            printf((filestat.st_mode & S_IXGRP) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            printf("Others:\n");
            printf("\n");
            printf((filestat.st_mode & S_IROTH) ? "Read-yes\n" : "Read-yes\n");
            printf((filestat.st_mode & S_IWOTH) ? "Write-yes\n" : "Write-no\n");
            printf((filestat.st_mode & S_IXOTH) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            break;
        }
        case 'l':
        {
            char linkname[20];
            printf("Enter link name: ");
            scanf("%s", linkname);
            if (symlink(filename, linkname) < 0)
            {
                printf("Error: unable to create symbolic link %s\n", linkname);
                break;
            }
            printf("Symbolic link %s created for file %s\n", linkname, filename);
            break;
        }
        default:
        {
            printf("Invalid menu option. Try again\n");
            goto start_file;
            break;
        }
        }
    }
}

/*Link menu display.*/
void print_menu_link()
{
    printf("B) Symbolic link:\n");
    printf("-n: link name\n");
    printf("-l: delete symbolic link\n");
    printf("-d: size of the symbolic link\n");
    printf("-t: size of the target file\n");
    printf("-a: access rights\n");
}

/*Link handling.*/
void handle_symbolic_link(char *linkname)
{
    char option[10];
    start_link:
    print_menu_link();
    printf("Choose an option from the menu:\n");
    scanf("%s", option);
    struct stat linkstat;
    for (int i = 1; i < strlen(option); i++)
    {
        switch (option[i])
        {
        case 'n':
        {
            printf("Link name: %s\n", getName(linkname));
            break;
        }
        case 'l':
        {
            if (unlink(linkname) < 0)
            {
                printf("Error: unable to delete symbolic link %s\n", linkname);
                break;
            }
            printf("Symbolic link %s deleted\n", linkname);
            break;
        }
        case 'd':
        {
            if (lstat(linkname, &linkstat) < 0)
            {
                printf("Error: unable to stat %s\n", linkname);
                break;
            }
            printf("Symbolic link size: %lld bytes\n", (long long)linkstat.st_size);
            break;
        }
        case 't':
        {
            struct stat targetstat;
            if (stat(linkname, &targetstat) < 0)
            {
                printf("Error: unable to stat target of %s\n", linkname);
                break;
            }
            printf("Size of target file: %lld bytes\n", (long long)targetstat.st_size);
            break;
        }
        case 'a':
        {
            if (lstat(linkname, &linkstat) < 0)
            {
                printf("Error: unable to stat %s\n", linkname);
                break;
            }
            printf("Access rights:\n");
            printf("\n");
            printf("User:\n");
            printf("\n");
            printf((linkstat.st_mode & S_IRUSR) ? "Read-yes\n" : "Read-yes\n");
            printf((linkstat.st_mode & S_IWUSR) ? "Write-yes\n" : "Write-no\n");
            printf((linkstat.st_mode & S_IXUSR) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            printf("Group:\n");
            printf("\n");
            printf((linkstat.st_mode & S_IRGRP) ? "Read-yes\n" : "Read-yes\n");
            printf((linkstat.st_mode & S_IWGRP) ? "Write-yes\n" : "Write-no\n");
            printf((linkstat.st_mode & S_IXGRP) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            printf("Others:\n");
            printf("\n");
            printf((linkstat.st_mode & S_IROTH) ? "Read-yes\n" : "Read-yes\n");
            printf((linkstat.st_mode & S_IWOTH) ? "Write-yes\n" : "Write-no\n");
            printf((linkstat.st_mode & S_IXOTH) ? "Exec-yes\n" : "Exec-no\n");
            printf("\n");
            break;
        }
        default:
        {
            printf("Invalid menu option. Try again\n");
            goto start_link;
            break;
        }
        }
    }
}

/*Directory menu display.*/
void print_menu_directory()
{
    printf("C) Directory:\n");
    printf("-n: name\n");
    printf("-d: size\n");
    printf("-a: access rights\n");
    printf("-c: total number of files with the .c extension\n");
}

/*Directory handling.*/
void handle_directory(char *directorypath)
{
    char option[10];
    start_directory:
    print_menu_directory();
    printf("Choose an option from the menu:\n");
    scanf("%s", option);
    struct dirent *entry;
    struct stat filestat;
    DIR *dir = opendir(directorypath);
    int cFiles=0;
    for (int i = 1; i < strlen(option); i++)
    {
        switch (option[i])
        {
            case 'n':
            {
                printf("Directory name: %s\n", getName(directorypath));
                break;
            }
            case 'd':
            {
                if (lstat(directorypath, &filestat) < 0)
                {
                    printf("Error: unable to stat %s\n", directorypath);
                    break;
                }
                printf("Directory size: %lld bytes\n", (long long)filestat.st_size);
                break;   
            }
            case 'a':
            {
                if (lstat(directorypath, &filestat) < 0)
                {
                    printf("Error: unable to stat %s\n", directorypath);
                    break;
                }
                printf("Access rights:\n");
                printf("\n");
                printf("User:\n");
                printf("\n");
                printf((filestat.st_mode & S_IRUSR) ? "Read-yes\n" : "Read-yes\n");
                printf((filestat.st_mode & S_IWUSR) ? "Write-yes\n" : "Write-no\n");
                printf((filestat.st_mode & S_IXUSR) ? "Exec-yes\n" : "Exec-no\n");
                printf("\n");
                printf("Group:\n");
                printf("\n");
                printf((filestat.st_mode & S_IRGRP) ? "Read-yes\n" : "Read-yes\n");
                printf((filestat.st_mode & S_IWGRP) ? "Write-yes\n" : "Write-no\n");
                printf((filestat.st_mode & S_IXGRP) ? "Exec-yes\n" : "Exec-no\n");
                printf("\n");
                printf("Others:\n");
                printf("\n");
                printf((filestat.st_mode & S_IROTH) ? "Read-yes\n" : "Read-yes\n");
                printf((filestat.st_mode & S_IWOTH) ? "Write-yes\n" : "Write-no\n");
                printf((filestat.st_mode & S_IXOTH) ? "Exec-yes\n" : "Exec-no\n");
                printf("\n");
                break;
            }
            case 'c':
            {
                if(dir != NULL)
                {
                    while((entry=readdir(dir))!=NULL)
                    {
                        char copy[20];
                        strcpy(copy, entry->d_name);
                        if(copy[strlen(entry->d_name)-1]=='c' && copy[strlen(entry->d_name)-2]=='.')
                        {
                            cFiles++;
                        }
                    }
                }
                else
                {
                    perror("Couldn't open directory");
                    return;
                }
                printf("Number of files with .c extension: %d\n", cFiles);
                closedir(dir);
                break;
            }
            default:
            {
                printf("Invalid menu option. Try again\n");
                goto start_directory;
                break;
            }
        }
    }
}

/*Function for computing the score.*/
int computeScore(int errors, int warnings)
{
    int score;
    if(errors == 0 && warnings == 0)
    {
        score = 10;
    }
    if(errors >= 1)
    {
        score = 1;
    }
    if(errors == 0 && warnings > 10)
    {
        score = 2;
    }
    if(errors == 0 && warnings <= 10)
    {
        score = 2+8*(10-warnings)/10;
    }
    return score;
}

/*Changing link permissions to 'rwxrw----'*/
void change_permission(char *linkname)
{
    char *arguments[] = {"chmod", "-v", "760", linkname, NULL};
    printf("Changing permissions to 'rwxrw----'\n");
    if(execv("/usr/bin/chmod", arguments) == -1) 
    {
        perror("execv");
        exit(EXIT_FAILURE);
    }
}

/*Counting the number of lines in a given file*/
int countLines(char file[])
{
    FILE *fin = fopen(file, "r");
    int count=1;
    char c;
    while((c = fgetc(fin)) != EOF)
    {
        if(c=='\n')
        {
            count++;
        }
    }
    fclose(fin);
    return count;
    
}

int main(int argc, char *argv[])
{
    struct stat itemstat;
    pid_t pid1, pid2;
    int pidCount=0;
    int fd[2];
    char buff[1024];

    for (int i = 1; i < argc; i++)
    { 
        if (lstat(argv[i], &itemstat) < 0)
        {
            printf("Error: unable to stat %s\n", argv[i]);
            break;
        }

        if(pipe(fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(itemstat.st_mode))
        {
            char *name = getName(argv[i]);
            /*If it has the extension .c*/
            if(name[strlen(name)-2] == '.' && name[strlen(name)-1] == 'c')
            {
                if((pid2 = fork()) < 0)
                {
                    printf("Failed to create the second process\n");
                    exit(0);
                }
                pidCount++;
                /*Child process: */
                if(pid2 == 0)
                {
                    /*Closing the reading end*/
                    if(close(fd[0]) == -1)
                    {
                        perror("close");
                        exit(EXIT_FAILURE);
                    }
                    /*Setting the standard output*/
                    if(dup2(fd[1], STDOUT_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    char *arguments[] = {"bash", "errors.sh", argv[i], "redirectError.txt", NULL};
                    printf("This is a .c file, executing script...\n");
                    if(execv("/usr/bin/bash", arguments)==-1)
                    {
                        perror("execv");
                        exit(EXIT_FAILURE);
                    }
                    exit(1);
                }
                /*Parent process: */
                else
                {
                    int errors, warnings, score;
                    /*Closing the writing end*/
                    if(close(fd[1]) == -1)
                    {
                        perror("close");
                        exit(EXIT_FAILURE);
                    }
                    FILE* fout = fopen("grades.txt", "w");
                    ssize_t num_read;
                    while ((num_read = read(fd[0], buff, sizeof(buff))) > 0) {
                        sscanf(buff, "number of errors: %d number of warnings: %d", &errors, &warnings);
                        printf("errors %d, warnings %d\n", errors, warnings);
                        score = computeScore(errors,warnings);
                        fprintf(fout, "%s:%d\n", name, score);
                        printf("The score was printed in the file 'grades.txt'\n");
                        fclose(fout);
                    }
                }
            }
            else
            {
                if((pid2 = fork()) < 0)
                {
                    printf("Failed to create the second process\n");
                    exit(2);
                }
                pidCount++;
                /*Child process: */
                if(pid2 == 0)
                {
                    /*Closing the reading end*/
                    close(fd[0]);
                    printf("The file %s has %d lines\n", name, countLines(argv[i]));
                    exit(3);
                }
            }
        }
        if (S_ISLNK(itemstat.st_mode))
        {
            char *name = getName(argv[i]);
            if((pid2 = fork()) < 0)
            {
                printf("Failed to create the second process\n");
                exit(0);
            }
            pidCount++;
            /*Child process: */
            if(pid2 == 0)
            {
                change_permission(name);
            }
        }

        if (S_ISDIR(itemstat.st_mode))
        {
            char *name = getName(argv[i]);
            if((pid2 = fork()) < 0)
            {
                printf("Failed to create the second process\n");
                exit(0);
            }
            pidCount++;
            /*Child process: */
            if(pid2 == 0)
            {
                char filename[50] = "";
                strcpy(filename, name);
                strcat(filename, "_file.txt");
                char *arguments[] = {"touch", filename, NULL};
                printf("Creating a txt file with the name '<dir_name>_file.txt'\n");
                if(execv("/usr/bin/touch", arguments)==-1)
                {
                    perror("execv");
                    exit(EXIT_FAILURE);
                }
                exit(1);
            }
        }
        
        if((pid1 = fork()) < 0)
        {
            printf("Failed to create the process \n");
            exit(1);
        }
        pidCount++;
        /*Child process: */
        if(pid1 == 0)
        {
            if (S_ISREG(itemstat.st_mode))
            {
                printf("%s %s\n", getName(argv[i]), "Regular file");
                handle_regular_file(argv[i]);
            }
            else if (S_ISLNK(itemstat.st_mode))
            {
                printf("%s %s\n", getName(argv[i]), "Symbolic link");
                handle_symbolic_link(argv[i]);
            }
            else if (S_ISDIR(itemstat.st_mode))
            {
                printf("%s %s\n", getName(argv[i]), "Directory");
                handle_directory(argv[i]);
            }
            else
            {
                printf("Unknown file type: %s\n", argv[i]);
            }
            exit(2);
        }

        for(int i=0; i<pidCount;i++)
        {
            int wstatus;
            pid_t w = wait(&wstatus);
            if (w == -1) 
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(wstatus)) 
            {
                printf("The process with PID %d has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
            }
        }
        pidCount = 0;
    }
    return 0;
}
