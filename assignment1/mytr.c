#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFERSIZE 500

void getString(char *inputBuffer);
void stringSearch(char *string, char *query, int *resultIdxs); 
void deleteSubstring(char *string, int start, int length);
void replaceSubstring(char *string, char *replacement, int start);
void delete(char *set);
void translate(char *src, char *dest);


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

void stringSearch(char *string, char *query, int *resultIdxs) {
	int maxSearchMatches = strlen(string) % strlen(query);
	int matchStartIdxs[maxSearchMatches];
	int matchIdx = 0;
	int idx, substrIdx, i;
	 
	// Primitive substring search
	// TODO: Update this to something more efficient
	for (idx=0; string[idx]; idx++) {
		if (string[idx] == query[0] && idx < MAXBUFFERSIZE - strlen(query)) {
			// Check that substring has a match starting at current index
			int fullMatch = 1;
			for (substrIdx=0; query[substrIdx]; substrIdx++) {
				if (string[idx + substrIdx] != query[substrIdx]) {
				  fullMatch = 0;
				  break;
				}
			}
			if (fullMatch) {
				matchStartIdxs[matchIdx] = idx;
				matchIdx++;
			}
		}
	}
	for (i=0; i<maxSearchMatches; i++) {
		resultIdxs[i] = matchStartIdxs[i];
	}
}

void deleteSubstring(char *string, int start, int length) {
    int i = start;
    for (i; string[i+length]; i++) {
        string[i] = string[i+length];
    }
    for (i; string[i]; i++) {
        string[i] = '\0';
    }
}

void replaceSubstring(char *string, char *replacement, int start) {
	int idx;
	for (idx=0; replacement[idx]; idx++) {
		string[start + idx] = replacement[idx];
	}
}

void delete(char *set) {

    while (1) {
        char inputBuffer[MAXBUFFERSIZE];
        int idx, substrIdx, i;
		int matchStartIdxs[strlen(inputBuffer) * strlen(set)];
        getString(inputBuffer); 
		stringSearch(inputBuffer, set, matchStartIdxs);
		
        // Check for EOF and return 0 if so
		
		for (i=0; matchStartIdxs[i]; i++) {
			deleteSubstring(inputBuffer, matchStartIdxs[i], strlen(set));
		}

	
        printf("%s\n", inputBuffer);
    }
}


void translate(char *src, char *dest) {	
	int lenSrc = strlen(src);
	int lenDest = strlen(dest);
	char fixedDest[lenSrc + 1];
	
	// Update destination translation to use 
    if (lenSrc > lenDest) {
		
		strcpy(fixedDest, dest);
		fixedDest[lenSrc] = '\0';
		for (int i=lenDest; i<lenSrc; i++) {
			fixedDest[i] = dest[lenDest - 1];
		}
		strncpy(dest, fixedDest, lenSrc);
		dest[lenSrc] = '\0';
	}
	
	while (1) {
		char inputBuffer[MAXBUFFERSIZE];
		int idx, substrIdx;
		getString(inputBuffer);
		
        // Check for EOF and return 0 if so

        // Primitive substring search
        // TODO: Update this to something more efficient
        for (idx=0; inputBuffer[idx]; idx++) {
            if (inputBuffer[idx] == src[0] && idx < MAXBUFFERSIZE - strlen(src)) {
                // Check that substring has a match starting at current index
                int fullMatch = 1;
                for (substrIdx=0; src[substrIdx]; substrIdx++) {
                    if (inputBuffer[idx + substrIdx] != src [substrIdx]) {
                      fullMatch = 0;
                      break;
                    }
                }
                if (fullMatch) {
					replaceSubstring(inputBuffer, dest, idx);
                }
            } 
        }
        printf("%s\n", inputBuffer);
	}
}


int main(int argc, char **argv) {

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
        translate(argv[1], argv[2]);
    }

    return 0;
}
