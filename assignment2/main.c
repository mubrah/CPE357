#include "main.h"

int main(int argc, char **argv) {
    int numWords = 10;
    struct hashTable *table = makeHashTable(); 
    // TODO: Parse CLI. Right now just reading 1 file passed in



    FILE *file = fopen(argv[1], "r");
    char *stringBuffer;
    int bufferLength = 256;
    int bufferSize = 0;

    if ((stringBuffer = (char *)malloc(bufferLength * sizeof(*stringBuffer))) == NULL) {
        // Do somehing
    }

    while (1) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        } else {
            if (bufferSize == bufferLength) {
                bufferLength *= 2;
                if ((stringBuffer = (char *)realloc(stringBuffer-bufferSize, bufferLength))
                        == NULL) {
                    // Do something
                }
                stringBuffer += bufferSize;
            }

            if (isalpha(c) || isdigit(c)) {
                *stringBuffer = c;
                stringBuffer++;
                bufferSize++;
            } else {
                *stringBuffer = '\0';
                stringBuffer -= bufferSize;
                struct wordCount *word = buildWord(stringBuffer);
                insertHashTable(table, word);
                
                int i;
                for (i = 0; stringBuffer[i]; i++) {
                    stringBuffer[i] = '\0';
                }
                bufferSize = 0;
            } 
        }
    }
    free(stringBuffer);
    fclose(file);
    
    return 0;
}
