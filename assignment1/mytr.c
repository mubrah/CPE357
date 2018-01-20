#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFERSIZE 500


void escapeChar(char *string);
void getString(char *inputBuffer);
int stringSearch(char *string, char *query, int *resultIdxs); 
void deleteSubstring(char *string, int length, int start);
void replaceSubstring(char *string, char *replacement, int start);
void delete(char *set);
void translate(char *src, char *dest);


void escapeChar(char *string) {
	int i, j;
	for (i=0; string[i]; i++) {
		if (string[i] == '\\') {
			switch (string[i + 1]) {
				case 't':
					string[i] = '\t';
					break;
				case 'n':
					string[i] = '\n';
					break;
				case '\\':
					string[i] = '\\';
					break;
			}
			for (j=i+1; string[j]; j++) {
				string[j] = string[j+1];
			}
		}
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

int stringSearch(char *string, char *query, int *resultIdxs) {
	int maxSearchMatches = strlen(string) % strlen(query);
	int matchStartIdxs[maxSearchMatches];
	int matchIdx = 0;
	int idx, substrIdx, i;
	 
	// TODO: Update this to something more efficient
	for (idx=0; string[idx]; idx++) {
		if (string[idx] == query[0] && idx < MAXBUFFERSIZE - strlen(query)) {
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
	for (i=0; i<matchIdx; i++) {
		resultIdxs[i] = matchStartIdxs[i];
	}
	return matchIdx;			// Returns the number of matches found
}

void deleteSubstring(char *string, int length, int start) {
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
		int matchStartIdxs[strlen(inputBuffer) % strlen(set)];
		
        getString(inputBuffer); 
		int numMatches = stringSearch(inputBuffer, set, matchStartIdxs);
		for (i=0; i<numMatches; i++) {
			// -strlen(set)*i required to correct for changing input string length since
			// matchStartIdxs continas match start indexss for the original string length. 
			deleteSubstring(inputBuffer, strlen(set), matchStartIdxs[i]-strlen(set)*i);
		}
        printf("%s\n", inputBuffer);
    }
}

void translate(char *src, char *dest) {	
	int lenSrc = strlen(src);
	int lenDest = strlen(dest);
	char fixedDest[lenSrc + 1];
	
	// Update translation dest if len(set1) > len(set2)
	// (ie) set1: abcd	set2: def	=> set2: deff
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
		int idx, substrIdx, i;
		int matchStartIdxs[strlen(inputBuffer) % strlen(src)];
		
		getString(inputBuffer);
		int numMatches = stringSearch(inputBuffer, src, matchStartIdxs);
		for (i=0; i<numMatches; i++) {
			replaceSubstring(inputBuffer, dest, matchStartIdxs[i]);
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
        escapeChar(argv[2]);
        delete(argv[2]);
    } else {
		escapeChar(argv[1]);
		escapeChar(argv[2]);
		translate(argv[1], argv[2]);
    }


    return 0;
}
