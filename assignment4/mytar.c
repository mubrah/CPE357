#include "mytar.h"
#include "archive.h"
#include "extract.h"

int main(int argc, char **argv) {
    FILE *archive;
    struct tarHeader header = {0};

    archive = fopen("test.tar", "wb");
    writeHeader(argv[1], archive);
    archiveData(argv[1], archive);
    fclose(archive);
    archive = fopen("test.tar", "rb");
    readHeader(argv[2], archive, &header);
    extractData(argv[2], archive, &header);


    return 0;
}
