#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

void traverse(int depth) {
    char currentDir[2 + 3 * depth];
    char upDir[3 + 3 * depth];
    DIR *dir;
    struct dirent *ep = NULL;
    struct stat fileStat;
    int i;

    /* Set up traversal path based on depth */
    currentDir[0] = '.';
    currentDir[1] = '/';

    upDir[0] = '.';
    upDir[1] = '.';
    upDir[2] = '/';

    for (i = 0; i < depth; i++) {
        currentDir[2 + 3*i + 0] = '.';
        currentDir[2 + 3*i + 1] = '.';
        currentDir[2 + 3*i + 2] = '/';

        upDir[3 + 3*i + 0] = '.';
        upDir[3 + 3*i + 1] = '.';
        upDir[3 + 3*i + 2] = '/';
    }

    /* Open current directory file at specified depth above this level */
    dir = opendir(upDir);
    stat(currentDir, &fileStat);


    if (dir == NULL) {
        printf("Cannot open directory: %s\n", upDir);
        return 0;
    } else {
        while (ep = readdir(dir)) {
            if (fileStat.st_ino == ep->d_ino) {
                printf("%s\n", ep->d_name);
                traverse(depth+1);
            }
        }
        closedir(dir);
    }
    return 0;
}

int main(int argc, char **argv) {
    char *pathName = malloc(sizeof(*pathName) * 256);
    traverse(0);
}
