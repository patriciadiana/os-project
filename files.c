#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>

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

void print_menu_file()
{
    printf("A) regular file:\n");
    printf("-n: file name\n");
    printf("-d: file size\n");
    printf("-h: number of hard links\n");
    printf("-m: time of last modification\n");
    printf("-a: access rights\n");
    printf("-l: create a symbolic link, give: link name\n");
}

void handle_regular_file(char *filename)
{
    char option[10];
    print_menu_file();
    printf("choose an option:\n");
    scanf("%s", option);
    struct stat filestat;
    for (int i = 1; i < strlen(option); i++)
    {
        switch (option[i])
        {
        case 'n':
        {
            printf("file name: %s\n", getName(filename));
            break;
        }
        case 'd':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("error: unable to stat %s\n", filename);
                break;
            }
            printf("file size: %lld bytes\n", (long long)filestat.st_size);
            break;
        }
        case 'h':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("error: unable to stat %s\n", filename);
                break;
            }
            printf("number of hard links: %ld\n", (long)filestat.st_nlink);
            break;
        }
        case 'm':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("error: unable to stat %s\n", filename);
                break;
            }
            printf("time of last modification: %s", ctime(&filestat.st_mtime));
            break;
        }
        case 'a':
        {
            if (lstat(filename, &filestat) < 0)
            {
                printf("error: unable to stat %s\n", filename);
                break;
            }
            printf("access rights:\n");
            printf("user:\n");
            printf((filestat.st_mode & S_IRUSR) ? "read-yes\n" : "read-yes\n");
            printf((filestat.st_mode & S_IWUSR) ? "write-yes\n" : "write-no\n");
            printf((filestat.st_mode & S_IXUSR) ? "exec-yes\n" : "exec-no\n");
            printf("group:\n");
            printf((filestat.st_mode & S_IRGRP) ? "read-yes\n" : "read-yes\n");
            printf((filestat.st_mode & S_IWGRP) ? "write-yes\n" : "write-no\n");
            printf((filestat.st_mode & S_IXGRP) ? "exec-yes\n" : "exec-no\n");
            printf("others:\n");
            printf((filestat.st_mode & S_IROTH) ? "read-yes\n" : "read-yes\n");
            printf((filestat.st_mode & S_IWOTH) ? "write-yes\n" : "write-no\n");
            printf((filestat.st_mode & S_IXOTH) ? "exec-yes\n" : "exec-no\n");
            printf("\n");
            break;
        }
        case 'l':
        {
            char linkname[20];
            printf("enter link name: ");
            scanf("%s", linkname);
            if (symlink(filename, linkname) < 0)
            {
                printf("error: unable to create symbolic link %s\n", linkname);
                break;
            }
            printf("symbolic link %s created for file %s\n", linkname, filename);
            break;
        }
        default:
        {
            printf("invalid menu option. try again\n");
            break;
        }
        }
    }
}

void print_menu_link()
{
    printf("B) symbolic link:\n");
    printf("-n: link name\n");
    printf("-l: delete link\n");
    printf("-d: size of the link\n");
    printf("-t: size of the target\n");
    printf("-a: access rights\n");
}

void handle_symbolic_link(char *linkname)
{
    char option[10];
    print_menu_link();
    printf("choose an option:\n");
    scanf("%s", option);
    struct stat linkstat;
    for (int i = 1; i < strlen(option); i++)
    {
        switch (option[i])
        {
        case 'n':
        {
            printf("file name: %s\n", getName(linkname));
            break;
        }
        case 'l':
        {
            if (unlink(linkname) < 0)
            {
                printf("error: unable to delete symbolic link %s\n", linkname);
                break;
            }
            printf("symbolic link %s deleted\n", linkname);
            break;
        }
        case 'd':
        {
            if (lstat(linkname, &linkstat) < 0)
            {
                printf("error: unable to stat %s\n", linkname);
                break;
            }
            printf("file size: %lld bytes\n", (long long)linkstat.st_size);
            break;
        }
        case 't':
        {
            struct stat targetstat;
            if (stat(linkname, &targetstat) < 0)
            {
                printf("error: unable to stat target of %s\n", linkname);
                break;
            }
            printf("size of target file: %lld bytes\n", (long long)targetstat.st_size);
            break;
        }
        case 'a':
        {
            if (lstat(linkname, &linkstat) < 0)
            {
                printf("error: unable to stat %s\n", linkname);
                break;
            }
            printf("access rights:\n");
            printf("user:\n");
            printf((linkstat.st_mode & S_IRUSR) ? "read-yes\n" : "read-yes\n");
            printf((linkstat.st_mode & S_IWUSR) ? "write-yes\n" : "write-no\n");
            printf((linkstat.st_mode & S_IXUSR) ? "exec-yes\n" : "exec-no\n");
            printf("group:\n");
            printf((linkstat.st_mode & S_IRGRP) ? "read-yes\n" : "read-yes\n");
            printf((linkstat.st_mode & S_IWGRP) ? "write-yes\n" : "write-no\n");
            printf((linkstat.st_mode & S_IXGRP) ? "exec-yes\n" : "exec-no\n");
            printf("others:\n");
            printf((linkstat.st_mode & S_IROTH) ? "read-yes\n" : "read-yes\n");
            printf((linkstat.st_mode & S_IWOTH) ? "write-yes\n" : "write-no\n");
            printf((linkstat.st_mode & S_IXOTH) ? "exec-yes\n" : "exec-no\n");
            printf("\n");
            break;
        }
        default:
        {
            printf("invalid menu option. try again\n");
            break;
        }
        }
    }
}

void print_menu_directory()
{
    printf("C) directory:\n");
    printf("-n: name\n");
    printf("-d: size\n");
    printf("-a: access rights\n");
    printf("-c: total number of files with the c. extension\n");
}

void handle_directory(char *directorypath)
{
    char option[10];
    print_menu_directory();
    printf("choose an option:\n");
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
                printf("file name: %s\n", getName(directorypath));
                break;
            }
            case 'd':
            {
                if (lstat(directorypath, &filestat) < 0)
                {
                    printf("error: unable to stat %s\n", directorypath);
                    break;
                }
                printf("file size: %lld bytes\n", (long long)filestat.st_size);
                break;   
            }
            case 'a':
            {
                if (lstat(directorypath, &filestat) < 0)
                {
                    printf("error: unable to stat %s\n", directorypath);
                    break;
                }
                printf("access rights:\n");
                printf("user:\n");
                printf((filestat.st_mode & S_IRUSR) ? "read-yes\n" : "read-yes\n");
                printf((filestat.st_mode & S_IWUSR) ? "write-yes\n" : "write-no\n");
                printf((filestat.st_mode & S_IXUSR) ? "exec-yes\n" : "exec-no\n");
                printf("group:\n");
                printf((filestat.st_mode & S_IRGRP) ? "read-yes\n" : "read-yes\n");
                printf((filestat.st_mode & S_IWGRP) ? "write-yes\n" : "write-no\n");
                printf((filestat.st_mode & S_IXGRP) ? "exec-yes\n" : "exec-no\n");
                printf("others:\n");
                printf((filestat.st_mode & S_IROTH) ? "read-yes\n" : "read-yes\n");
                printf((filestat.st_mode & S_IWOTH) ? "write-yes\n" : "write-no\n");
                printf((filestat.st_mode & S_IXOTH) ? "exec-yes\n" : "exec-no\n");
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
                    perror("couldn't open directory");
                    return;
                }
                printf("number of files with .c extension: %d\n", cFiles);
                closedir(dir);
                break;
            }
            default:
            {
                printf("invalid menu option. try again\n");
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct stat itemstat;
    for (int i = 1; i < argc; i++)
    {
        if (lstat(argv[i], &itemstat) < 0)
        {
            printf("error: unable to stat %s\n", argv[i]);
            continue;
        }
        if (S_ISREG(itemstat.st_mode))
        {
            printf("%s %s\n", getName(argv[i]), "regular file");
            handle_regular_file(argv[i]);
        }
        else if (S_ISLNK(itemstat.st_mode))
        {
            printf("%s %s\n", getName(argv[i]), "symbolic link");
            handle_symbolic_link(argv[i]);
        }
        else if (S_ISDIR(itemstat.st_mode))
        {
            printf("%s %s\n", getName(argv[i]), "directory");
            handle_directory(argv[i]);
        }
        else
        {
            printf("unknown file type: %s\n", argv[i]);
        }
    }
    return 0;
}