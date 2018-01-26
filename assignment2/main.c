#include "main.h"

int main(int argc, char **argv) {
    int numWords = 10;
    int *hashList; 
    // TODO: Parse CLI. Right now just reading 1 file passed in
    getWords(argv[1], hashList);; 
    
    return 0;
}
