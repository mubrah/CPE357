#include "mytar.h"
#include "archive.h"
#include "extract.h"

int main(int argc, char **argv) {
    FILE *archive;

    archive = fopen("test.tar", "wb");
    writeHeader(argv[1], archive);
    archiveData(argv[1], archive);
    fclose(archive);
    archive = fopen("test.tar", "rb");
    readHeader(argv[2], archive);
    extractData(argv[2], archive);


    return 0;
}
