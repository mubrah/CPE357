#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_long_line(FILE *file) {
    int bufferLength = 256;
    int bufferIdx = 0;
    char *stringBuffer = (char *)malloc(bufferLength * sizeof(*stringBuffer));
    char *previousLine = (char *)calloc(1, sizeof(*previousLine));

    while (1) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        } else {
            if (bufferIdx == bufferLength) {
                bufferLength *= 2;
                stringBuffer = (char *)realloc(stringBuffer - bufferIdx, bufferLength);
                stringBuffer += bufferIdx;
            }
            if (c == '\n') {
                *stringBuffer = '\0';
                stringBuffer -= bufferIdx;
                if (strcmp(stringBuffer, previousLine)) {
                    printf("%s\n", stringBuffer);
                }
                previousLine = (char *)realloc(previousLine,
                                           bufferIdx+1 * sizeof(*previousLine));
                strcpy(previousLine, stringBuffer);
                bufferIdx = 0;
            } else {
                *stringBuffer = c;
                stringBuffer++;
                bufferIdx++;
            }
        }
    }
    free(stringBuffer);
    free(previousLine);
}

int main(int argc, char **argv) {
    
    FILE *file = fopen(argv[1], "r");
    read_long_line(file); 
    fclose(file);
    return 0;
}   
