#include "fsDirectory.h"
#include <stdio.h>
#include <string.h>
#include "fsLow.h"
#include "vcb.h"

struct st_directory *initDefaultDir(struct st_directory *sDir, int iBlock, int sizeMallocDir) {
    time_t t = time(NULL);

    sDir[0].isFree = FALSE;
    sDir[0].startBlockNb = iBlock;
    sDir[0].creationDate = time(&t);
    sDir[0].lastModDate = time(&t);
    sDir[0].id = 1;
    sDir[0].parentId = 0;
    strcpy(sDir[0].name, ".");
    sDir[0].isDirectory = TRUE;
    sDir[0].sizeDirectory = sizeMallocDir;

    sDir[1].isFree = FALSE;
    sDir[1].startBlockNb = iBlock;
    sDir[1].creationDate = time(&t);
    sDir[1].lastModDate = time(&t);
    sDir[1].id = 0;
    sDir[1].parentId = -1;
    strcpy(sDir[1].name, "..");
    sDir[1].isDirectory = TRUE;
    sDir[1].sizeDirectory = sizeMallocDir;
    return (sDir);
}

int initializeDirectories(st_vcb *rVCB, int blockSize, int numberOfBlocks) {
    struct st_directory *sDir;
    unsigned int end = 0;
    unsigned int iBlock = 0;
    unsigned int nbDir = DIRECTORY_ENTRIES;
    unsigned int sizeMallocDir = sizeof(struct st_directory) * DIRECTORY_ENTRIES;
    unsigned int nbBlocks = (sizeMallocDir / blockSize) + 1;
    unsigned int maxSize = nbBlocks * blockSize;

    while (sizeMallocDir < maxSize && end != 1) {
        if (sizeMallocDir + sizeof(struct st_directory) < maxSize) {
            sizeMallocDir+=(sizeof(struct st_directory));
            nbDir+=1;
        } else {
            end = 1;
        }
    }
    sDir = malloc(sizeMallocDir);
    if (sDir == NULL) {
        printf("Error while allocating the memory for sDir\n");
        return (-1);
    }
    for (int i = 0; i != nbDir; i++)
        sDir[i].isFree = TRUE;
    iBlock = getFreeSpace(rVCB, nbBlocks, blockSize, numberOfBlocks);
    sDir = initDefaultDir(sDir, iBlock, sizeMallocDir);
    LBAwrite(sDir, nbBlocks, iBlock);
    return (iBlock);
}

