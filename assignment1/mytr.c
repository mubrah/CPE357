#include <stdio.h>
#include <string.h>

#define maxBufferSize 1000

void delete(char set[]);

void delete(char set[]) {
    char inputBuffer[maxBufferSize];
    while (fgets(inputBuffer, maxBufferSize, stdin)) {
        
        // Primitive substring search
        // TODO: Update this to something more efficient
        for (int idx=0; inputBuffer[idx]; idx++) {
            if (inputBuffer[idx] == set[0] && idx < maxBufferSize - strlen(set)) {
                int match = 1;
                for (int subStrIdx=0; set[subStrIdx]; subStrIdx++) {
                    if (inputBuffer[idx + subStrIdx] != set[subStrIdx]) {
                        match = 0;
                        break;
                    }
                }
                if (match) {
                    // Shift all elements in array forward by deleted string length
                    int reIdx;
                    for (reIdx=idx+1; inputBuffer[reIdx]; reIdx++) {
                        int shiftNextIdx = reIdx + strlen(set);
                        if (shiftNextIdx < maxBufferSize) {
                            inputBuffer[reIdx] = inputBuffer[shiftNextIdx];
                        } else {
                            break;
                        }
                    }
                    // Free remaining characters from string
                    for (reIdx; inputBuffer[reIdx]; reIdx++) {
                        inputBuffer[reIdx] = '\0';
                    }
                }
            }
        }
        printf(inputBuffer);
    }
}



int main(int argc, char *argv[]) {

    int deleteMode = 0;

    // Validate input arguments
    if (argc == 1) {
        fprintf(stderr, "mytr: missing operand\n");
        fprintf(stderr, "Try 'mytr --help' for more information.\n");
        return 1;
    }

    if (argc == 2) {
        if (argv[1][0] == '-') {
            fprintf(stderr, "mytr: missing operand\n");
            fprintf(stderr, "Try 'mytr --help' for more information.\n");
        } else {
            fprintf(stderr, "mytr: missing operand after \'%s\'\n", argv[1]);
            fprintf(stderr, "Two strings must be given when translating.\n");
            fprintf(stderr, "Try 'mytr --help' for more information.\n");
        }
        return 1;
    }

    if (argc > 3) {
        fprintf(stderr, "mytr: extra operand \'%s\'\n", argv[2]);
        fprintf(stderr, "Try 'mytr --help' for more information.\n");
        return 1;
    }

    
    // Main program logic
    if (argv[1] == "-d") {
        deleteMode = 1;
    }

    if (deleteMode) {
        delete(argv[2]);
    } else {
        // translate();
        return 255;
    }

    return 0;
}
