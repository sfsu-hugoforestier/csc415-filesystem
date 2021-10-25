/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names:
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "vcb.h"
#include "fsFree.h"


void initializeVCB(st_vcb *sVCB, int numberOfBlocks, int blockSize) {
    sVCB->blockSize = blockSize;
    sVCB->signature = PART_SIGNATURE;
    sVCB->freeBlockSize = numberOfBlocks;
    sVCB->numberOfBlocks = numberOfBlocks;
}

st_vcb *formatVolume(int blockSize, int numberOfBlocks) {
    int nbBlocksWrote = 0;
    st_vcb *rVCB = malloc(blockSize);
    initializeVCB(rVCB, numberOfBlocks, blockSize); // TO ADD IN checkIfVolumeExists mby
    rVCB->next = initializeFreeSpace(rVCB, blockSize, numberOfBlocks); // TO ADD IN checkIfVolumeExists mby
    if (rVCB->next == NULL)
        return (NULL);
    // initializeDirectory();
    nbBlocksWrote = LBAwrite(rVCB, 1, 0);
    if (nbBlocksWrote == -1) {
        printf("Error while writing\n");
        return (NULL);
    }
    printf("Formatting ended!\n");
    return (rVCB);
}

st_vcb *checkIfVolumeExists(uint64_t numberOfBlocks, uint64_t blockSize) {
    st_vcb *sVCB = malloc(blockSize);

    if (sVCB == NULL) {
        printf("Error while mallocing sVCB\n");
        return (NULL);
    }

    uint64_t rvRead = LBAread(sVCB, 1, 0);
    if (rvRead == -1) {
        printf("Error while reading\n");
        return (NULL);
    }
    if (sVCB->signature == PART_SIGNATURE) {
        printf("Formatting...\n");
        return (formatVolume(blockSize, numberOfBlocks));

    } else {
        
    }
}

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize) {
    printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    st_vcb *sVCB = checkIfVolumeExists(numberOfBlocks, blockSize);
    int result = makeRoot();
    if (sVCB == NULL) {
        return (-1);
    }
    return (0);
}


void exitFileSystem () {
    printf ("System exiting\n");
}
