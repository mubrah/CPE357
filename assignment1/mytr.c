#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFERSIZE 500


void escapeChar(char *string);
void getString(char *inputBuffer);
int getCharMapIdx(char string, char *query); 
void deleteSubstring(char *string, int length, int start);
void replaceSubstring(char *string, char *replacement, int start);
void delete(char *set);
void translate(char *src, char *dest);


void escapeChar(char *string) {
	int i, j;
	for (i=0; string[i]; i++) {
		if (string[i] == '\\' && string[i - 1] != '\\') {
			switch (string[i + 1]) {
				case 't':
					string[i] = '\t';
                    for (j=i+1; string[j]; j++) {
                        string[j] = string[j+1];
                    }
					break;
				case 'n':
					string[i] = '\n';
                    for (j=i+1; string[j]; j++) {
                        string[j] = string[j+1];
                    }
					break;
				case '\\':
					string[i] = '\\';
					break;
			}
		}
	}
}

void getString(char *inputBuffer) {
    int thisChar;
    int idx = 0;

    while (thisChar != (int)'\n') {
        if (thisChar == EOF) {
            exit(0);
        }
        thisChar = getchar();
        inputBuffer[idx] = (char)thisChar;
        idx++;
    }
    inputBuffer[idx - 1] = '\0';
}

int getCharMapIdx(char checkMatch, char *query) {
	int idx;
	int substrIdx = -1;
	for (idx=0; query[idx]; idx++) {
		if (checkMatch == query[idx]) {
			substrIdx = idx;
		}
	}
	return substrIdx;
}

void deleteSubstring(char *string, int length, int start) {
    int i = start;
    for (i+1; string[i]; i++) {
        string[i] = string[i+length];
    }
    for (i; string[i]; i++) {
        string[i] = '\0';
    }
}

void delete(char *set) {
    while (1) {
        char inputBuffer[MAXBUFFERSIZE];
        int idx;
		
        getString(inputBuffer);
		for (idx=0; inputBuffer[idx]; idx++) {
			int match = getCharMapIdx(inputBuffer[idx], set);
			if (match != -1) {
				deleteSubstring(inputBuffer, 1, idx);
				idx--;
			}
		}
        printf("%s\n", inputBuffer);
    }
}

void translate(char *src, char *dest) {	
	int lenSrc = strlen(src);
	int lenDest = strlen(dest);
	char fixedDest[lenSrc + 1];
	
	/*
	 * Update translation dest if len(set1) > len(set2)
	 * (ie) set1: abcd	set2: def	=> set2: deff
	 */
    if (lenSrc > lenDest) {
	    int i;

		strcpy(fixedDest, dest);
		for (i=lenDest; i<lenSrc; i++) {
			fixedDest[i] = dest[lenDest - 1];
		}
		strncpy(dest, fixedDest, lenSrc);
		dest[lenSrc] = '\0';
	}
	
	while (1) {
		char inputBuffer[MAXBUFFERSIZE];
		int idx;
		
		getString(inputBuffer);
		for (idx=0; inputBuffer[idx]; idx++) {
			int substrMapIdx = getCharMapIdx(inputBuffer[idx], src);
			if (substrMapIdx == -1) {
				continue;
			} else {
				inputBuffer[idx] = dest[substrMapIdx];
			}
		}
        printf("%s\n", inputBuffer);
	}
}


int main(int argc, char **argv) {

    int deleteMode = 0;

    /* Validate input arguments */
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

    /* Main program logic */
    if (strcmp(argv[1], "-d") == 0) {
        deleteMode = 1;
    }

    if (deleteMode) {
        escapeChar(argv[2]);
        delete(argv[2]);
    } else {
		escapeChar(argv[1]);
		escapeChar(argv[2]);
		translate(argv[1], argv[2]);
    }

    return 0;
}
