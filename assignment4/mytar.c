#include "mytar.h"
#include "archive.h"
#include "extract.h"

unsigned char getOption(char *options) {
    int c=0, t=0, x=0, f=0, i=0;
    unsigned char ret;
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
            ret = 'c';
        } else if (t > 0) {
            ret = 't';
        } else if (x > 0) { 
            ret = 'x';
        }
    }
    if (f > 0) {
        ret += 'f'; 
    }
    return ret;
}

/* usage: mytar <c | t | x>[v]f tarfile [path [...]] */
int main(int argc, char **argv) {
    FILE *archive;
    char usage[] = "usage: mytar <c | t | x>[v]f tarfile [path [...]]\n";
    unsigned char operation;

    if (argc < 2) {
        fprintf(stderr, usage);
        return 1;
    } else if ((operation = getOption(argv[1])) == '\0') {
        fprintf(stderr, usage);
        return 1;
    } else {
        if ((operation == 'c') || (operation == 't') || (operation == 'x')) {
            fprintf(stderr, "specify a tar file with the 'f' option.\n");
            fprintf(stderr, usage);
            return 1;
        } else if (argc < 3) {
            fprintf(stderr, "option 'f' requires an archive name.\n");
            fprintf(stderr, usage);
            return 1;
        }
        
        if (operation == 'c' + 'f') {
            return createArchive(argc, argv);
        } 
        if (operation == 't' + 'f') {
            return 1;
        }
        if (operation == 'x' + 'f') {
            return extractArchive(argc, argv);
        }
    }
    return 0;
}
