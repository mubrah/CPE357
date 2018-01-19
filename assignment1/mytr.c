#include <stdio.h>
#include <string.h>

#define MAXBUFFERSIZE 500

void delete(char set[]);
void getString(char *inputBuffer);
void deleteSubstring(char *string, int start, int length);


void deleteSubstring(char *string, int start, int length) {
    int i = start;
    for (i; string[i+length]; i++) {
        string[i] = string[i+length];
    }
    for (i; string[i]; i++) {
        string[i] = '\0';
    }
} 


void getString(char *inputBuffer) {
    int thisChar;
    int idx = 0;

    while (thisChar != (int) '\n') {
        thisChar = getchar();
        inputBuffer[idx] = (char) thisChar;
        idx++;
    }
    inputBuffer[idx-1] = '\0';
}


void delete(char set[]) {

    while (1) {
        char inputBuffer[MAXBUFFERSIZE];
        getString(inputBuffer); 

        // Check for EOF and return 0 if so

        // Primitive substring search
        // TODO: Update this to something more efficient
        for (int idx=0; inputBuffer[idx]; idx++) {
            if (inputBuffer[idx] == set[0] && idx < MAXBUFFERSIZE - strlen(set)) {
                // Check that substring has a match starting at current index
                int fullMatch = 1;
                for (int substrIdx=0; set[substrIdx]; substrIdx++) {
                    if (inputBuffer[idx + substrIdx] != set[substrIdx]) {
                      fullMatch = 0;
                      break;
                    }
                }
                if (fullMatch) {
                    deleteSubstring(inputBuffer, idx, strlen(set));
                }
            } 
        }
        printf("%s\n", inputBuffer);
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
    if (strcmp(argv[1], "-d") == 0) {
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
