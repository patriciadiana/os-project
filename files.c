#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void print_menu_file()
{
    printf("A) regular file\n");
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
            char *name = strrchr(filename, '/');
            if (name == NULL)
            {
                name = filename;
            }
            else
            {
                name++;
            }
            printf("file name: %s\n", name);
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
            printf("access rights: ");
            printf((filestat.st_mode & S_IRUSR) ? "r" : "-");
            printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
            printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
            printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
            printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
            printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
            printf((filestat.st_mode & S_IROTH) ? "r" : "-");
            printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
            printf((filestat.st_mode & S_IXOTH) ? "x" : "-");
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
    printf("B) symbolic link\n");
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
            char *name = strrchr(linkname, '/');
            if (name == NULL)
            {
                name = linkname;
            }
            else
            {
                name++;
            }
            printf("file name: %s\n", name);
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
            printf("access rights: ");
            printf((linkstat.st_mode & S_IRUSR) ? "r" : "-");
            printf((linkstat.st_mode & S_IWUSR) ? "w" : "-");
            printf((linkstat.st_mode & S_IXUSR) ? "x" : "-");
            printf((linkstat.st_mode & S_IRGRP) ? "r" : "-");
            printf((linkstat.st_mode & S_IWGRP) ? "w" : "-");
            printf((linkstat.st_mode & S_IXGRP) ? "x" : "-");
            printf((linkstat.st_mode & S_IROTH) ? "r" : "-");
            printf((linkstat.st_mode & S_IWOTH) ? "w" : "-");
            printf((linkstat.st_mode & S_IXOTH) ? "x" : "-");
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
            handle_regular_file(argv[i]);
        }
        else if (S_ISLNK(itemstat.st_mode))
        {
            handle_symbolic_link(argv[i]);
        }
        else if (S_ISDIR(itemstat.st_mode))
        {
            printf("directory: %s\n", argv[i]);
        }
        else
        {
            printf("unknown file type: %s\n", argv[i]);
        }
    }
    return 0;
}