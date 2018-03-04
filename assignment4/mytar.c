#include "mytar.h"
#include "writeArchive.h"
#include "readArchive.h"

int getOption(char *options) {
    int c=0, t=0, x=0, f=0, v=0, i=0, ret = 0;
    for (i = 0; i < strlen(options); i++) {
        switch (options[i]) {
            case 'c':
                c++;
                break;
            case 't':
                t++;
                break;
            case 'x':
                x++;
                break;
            case 'f':
                f++;
                break;
            case 'v':
                v++;
                break;
            default:
                fprintf(stderr, "unrecognized option '%c'.\n", options[i]);
                return '\0';
        }
    }
    if (c + t + x > 1) {            /* Multiple options were given */
        fprintf(stderr, "you may only choose one of the 'ctx' options.\n");
        return '\0';
    } else if (c + t + x == 0) {   /* Only f was given */
        fprintf(stderr, "you must choose one of the 'ctx' options.\n");
        return '\0';
    } else {
        if (c > 0) {
            ret = (int)'c';
        } else if (t > 0) {
            ret = (int)'t';
        } else if (x > 0) { 
            ret = (int)'x';
        }
    }
    if (f > 0) {
        ret += (int)'f'; 
    }
    if (v > 0) {
        ret += (int)'v';
    }
    return ret;
}

/* usage: mytar <c | t | x>[v]f tarfile [path [...]] */
int main(int argc, char **argv) {
    char usage[] = "usage: mytar <c | t | x>[v]f tarfile [path [...]]\n";
    int operation;

    if (argc < 2) {
        fprintf(stderr, usage);
        return 1;
    } else if ((operation = getOption(argv[1])) == '\0') {
        fprintf(stderr, usage);
        return 1;
    } else {
        if ((operation == (int)'c') ||
            (operation == (int)'t') ||
            (operation == (int)'x')) {
                fprintf(stderr, "specify a tar file with the 'f' option.\n");
                fprintf(stderr, usage);
            return 1;
        } else if (argc < 3) {
            fprintf(stderr, "option 'f' requires an archive name.\n");
            fprintf(stderr, usage);
            return 1;
        }
        
        switch (operation) {
            case (int)('c' + 'f'):
                return createArchive(argc, argv, 0);
            case (int)('c' + 'f' + 'v'):
                return createArchive(argc, argv, 1);
            case (int)('t' + 'f'):
                return listArchive(argc, argv, 0);
            case (int)('t' + 'f' + 'v'):
                return listArchive(argc, argv, 1);
            case (int)('x' + 'f'):
                return extractArchive(argc, argv, 0);
            case (int)('x' + 'f' + 'v'):
                return extractArchive(argc, argv, 1);
        }
    }
    return 1;
}
