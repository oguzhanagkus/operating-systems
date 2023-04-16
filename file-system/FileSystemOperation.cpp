#include "Helper.h"
#include "FileSystem.h"

int main(int argc, char **argv) {
    char *diskFile;
    char *operation;

    if (argc < 3 or argc > 5) {
        exitWithError("Check your arguments.");
    }

    diskFile = argv[1];
    operation = argv[2];

    // Loading file system and calling related functions
    FileSystem fileSystem(diskFile);
    if (fileSystem.load()) {
        log("File system loaded successfully.");
        if (strcmp(operation, "dir") == 0) {
            fileSystem.listDirectory(argv[3]);
        } else if (strcmp(operation, "mkdir") == 0) {
            fileSystem.makeDirectory(argv[3]);
        } else if (strcmp(operation, "rmdir") == 0) {
            fileSystem.removeDirectory(argv[3]);
        } else if (strcmp(operation, "dumpe2fs") == 0) {
            fileSystem.dumpe2fs();
        } else if (strcmp(operation, "write") == 0) {
            fileSystem.writeFile(argv[3], argv[4]);
        } else if (strcmp(operation, "read") == 0) {
            fileSystem.readFile(argv[3], argv[4]);
        } else if (strcmp(operation, "del") == 0) {
            fileSystem.removeFile(argv[3]);
        }
    } else {
        exitWithError("File system cannot be loaded.", errno);
    }

    return 0;
}
