#include "main.h"

int main(int argc, char **argv) {
    int numWords = 10;
    int filenameIdxs[argc-1];
    int fnIdx = 0;
    int idx;

    
    for (idx=1; idx < argc; idx++) {
        if (argv[idx][0] == '-') {
            switch (argv[idx][1]) {
                case 'n':
                    if (isdigit(argv[idx + 1])) {
                        numWords = *(argv[idx + 1]);
                        idx++;
                    } else {
                        fprintf(stderr, "fw: Integer not passed to '-n' switch");
                        return 1;
                    }
                    break;
                default:
                    fprintf(stderr, "fw: Invalid options given");
                    return 1;
                    break;
            }
        } else {
            filenameIdxs[fnIdx] = idx;
            fnIdx++;
        }
    }

    struct hashTable *table = makeHashTable(); 
    FILE *file = NULL;
    for (idx = 0; idx <= fnIdx; idx++) {
        file = fopen(argv[filenameIdxs[idx]], "r");
        countWords(file, table);
        fclose(file);
    }




    int i;
    for (i=0; i < table->capacity; i++) {
        if (table->buckets[i] != NULL) {
            struct chain *_buckets = table->buckets[i];
            int j = 0;
            while (_buckets != NULL) {
                printf("[%i][%i]: %s | %i\n", i, j, _buckets->word->string, _buckets->word->count);
                _buckets = _buckets->next;
                j++;
            }
        }
    }

    destroyHashTable(table);


    return 0;
}
