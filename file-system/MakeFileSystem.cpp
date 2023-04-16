#include "Helper.h"
#include "FileSystem.h"

int main(int argc, char **argv) {
    unsigned short blockSize = 0;
    std::string diskFile;

    // Argument validation
    if (argc != 3) {
        exitWithError("Check your arguments.");
    }
    if (strcmp(argv[1], "1") != 0 and strcmp(argv[1], "2") != 0 and strcmp(argv[1], "4") != 0) {
        exitWithError("Invalid block size: " + std::string(argv[1]) + ". Available sizes: 1, 2, and 4.");
    } else {
        blockSize = atoi(argv[1]);
        diskFile = argv[2];
    }

    // Creating file system
    FileSystem fileSystem(diskFile);
    if (fileSystem.create(16 , blockSize)) {
        log("File system created successfully.");
        fileSystem.dumpe2fs();
    } else {
        exitWithError("File system cannot be created.", errno);
    }

    return 0;
}
