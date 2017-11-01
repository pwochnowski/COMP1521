// myls.c ... my very own "ls" implementation

#include <stdlib.h>
#include <stdio.h>
#include <bsd/string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXDIRNAME 100
#define MAXFNAME   200
#define MAXNAME    20

char *rwxmode(mode_t, char *);
char *username(uid_t, char *);
char *groupname(gid_t, char *);

int main(int argc, char *argv[])
{
    // string buffers for various names
    char dirname[MAXDIRNAME];
    // char uname[MAXNAME+1]; // UNCOMMENT this line
    // char gname[MAXNAME+1]; // UNCOMMENT this line
    // char mode[MAXNAME+1]; // UNCOMMENT this line

    // collect the directory name, with "." as default
    if (argc < 2)
        strlcpy(dirname, ".", MAXDIRNAME);
    else
        strlcpy(dirname, argv[1], MAXDIRNAME);

    // check that the name really is a directory
    struct stat info;
    if (stat(dirname, &info) < 0)
    { perror(argv[0]); exit(EXIT_FAILURE); }
    if ((info.st_mode & S_IFMT) != S_IFDIR)
    { fprintf(stderr, "%s: Not a directory\n",argv[0]); exit(EXIT_FAILURE); }

    // open the directory to start reading
    DIR *df = opendir(dirname); // UNCOMMENT this line
    struct dirent *dir;

    strcat(dirname, "/");
    int dirLength = strlen(dirname);

    struct stat* buf = malloc(sizeof(struct stat));

    char buf1[11];
    buf1[10] = 0;
    char buf2[MAXDIRNAME];
    while ((dir = readdir(df)) != NULL) {
        char *filename = dir->d_name;
        if (strstr(filename,".") == dir->d_name) continue;
        char *path = malloc(dirLength+1+strlen(filename));
        strcpy(path, dirname);
        strcat(path, filename);

        printf("path: %s\n", path);
        lstat(path, buf);
        printf("%s  %-8.8s %-8.8s %8lld  %s\n",
                rwxmode(buf->st_mode, buf1),
                username(buf->st_uid, buf2),
                groupname(buf->st_gid, buf2),
                (long long)buf->st_size, filename);
    }
    

    // ... TODO ...

    // read directory entries
    // struct dirent *entry; // UNCOMMENT this line
    // ... TODO ...

    // finish up
    // closedir(df); // UNCOMMENT this line
    return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode(mode_t mode, char *str)
{
    char c;
    if (S_ISLNK(mode)) {
        c = 'l';
    } else if (S_ISDIR(mode)) {
        c = 'd';
    } else if (S_ISREG(mode)) {
        c = '-';
    } else {
        c = '?';
    }
    str[0] = c; 
    str++;
    for (int i=0; i<9; i++) {
        str[i] = '-';
    }
    int a[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    for (int i=0; i<9; i++) {
        if ((mode & a[i]) == 0) continue;
        switch (i%3) {
            case (0) :
                str[i] = 'r';
                break;
            case (1) :
                str[i] = 'w';
                break;
            case (2) :
                str[i] = 'x';
                break;
        }
    }
    return str-1;
}

// convert user id to user name
char *username(uid_t uid, char *name)
{
    struct passwd *uinfo = getpwuid(uid);
    if (uinfo == NULL)
        snprintf(name, MAXNAME, "%d?", (int)uid);
    else
        snprintf(name, MAXNAME, "%s", uinfo->pw_name);
    return name;
}

// convert group id to group name
char *groupname(gid_t gid, char *name)
{
    struct group *ginfo = getgrgid(gid);
    if (ginfo == NULL)
        snprintf(name, MAXNAME, "%d?", (int)gid);
    else
        snprintf(name, MAXNAME, "%s", ginfo->gr_name);
    return name;
}
