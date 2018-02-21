#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

int traverseDir(char **path, int depth, int maxDepth);

int traverseDir(char **path, int depth, int maxDepth) {
    DIR *dir;
    struct dirent *ep = NULL;
    struct stat fileStat;
    struct stat rootCheckCurrent;
    struct stat rootCheckUp;
    char *dirName = NULL;
    int ret;

    if (depth == maxDepth) {
        fprintf(stderr, "path too long");
        exit(1);
    }

    if (path[0] != NULL) {
        chdir("../");
    }
    dir = opendir("../");
    stat("./", &fileStat);

    if (dir == NULL) {
        perror("mypwd");
        return -1;
    } else {
        stat("./", &rootCheckCurrent);
        stat("../", &rootCheckUp);
        if (rootCheckCurrent.st_ino == rootCheckUp.st_ino) {
            closedir(dir);
            return depth;
        } else {
            while ((ep = readdir(dir))) {
                if (fileStat.st_ino == ep->d_ino) {
                    dirName = malloc(sizeof(*dirName) * strlen(ep->d_name));
                    strcpy(dirName, ep->d_name);
                    path[depth] = dirName;
                    ret = traverseDir(path, depth + 1, maxDepth);
                }
            }
        }
        closedir(dir);
        return ret ? ret : depth;
    }
}

int main(int argc, char **argv) {
    char **path = NULL;
    int depth, maxPath, i;
    
    maxPath = PATH_MAX ? PATH_MAX : 2048;
    path = calloc(maxPath, sizeof(*path));
    depth = traverseDir(path, 0, maxPath);

    printf("/");
    for (i = depth - 1; i >= 0; i--) {
        if (i > 0) {
            printf("%s/", path[i]);    
        } else {
            printf("%s\n", path[i]);
        }
        free(path[i]);
    }
    free(path);
    return 0;
}
