#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

int traverseDir(char **path, int depth, int maxDepth);

int traverseDir(char **path, int depth, int maxDepth) {
    DIR *upDir;
    struct dirent *ep = NULL;
    struct stat dotStat;
    struct stat epStat;
    struct stat rootCheckCurrent;
    struct stat rootCheckUp;
    char *dirName = NULL;
    int ret;

    if (depth == maxDepth) {
        fprintf(stderr, "path too long");
        exit(1);
    }


    upDir = opendir("..");
    stat(".", &dotStat);

    if (upDir == NULL) {
        perror("mypwd");
        return -1;
    } else {
        stat(".", &rootCheckCurrent);
        stat("..", &rootCheckUp);
        if (rootCheckCurrent.st_ino == rootCheckUp.st_ino) {
            closedir(upDir);
            return depth;
        } else {
            chdir("..");
            while ((ep = readdir(upDir))) {
                if (strcmp(ep->d_name, ".") != 0) {
                    stat(ep->d_name, &epStat);
                    if (dotStat.st_ino == epStat.st_ino) {
                        dirName = malloc(sizeof(*dirName) * (strlen(ep->d_name) + 1));
                        strcpy(dirName, ep->d_name);
                        path[depth] = dirName;
                        ret = traverseDir(path, depth + 1, maxDepth);
                        break;
                    } 
                }
            }
        }
        closedir(upDir);
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
