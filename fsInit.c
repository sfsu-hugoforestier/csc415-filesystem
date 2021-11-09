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
#include "fsDirectory.h"

#include "fsParsePath.h"

st_vcb *sVCB;

void initializeVCB(st_vcb *sVCB, int numberOfBlocks, int blockSize) {
    sVCB->blockSize = blockSize;
    sVCB->signature = PART_SIGNATURE;
    sVCB->freeBlockSize = numberOfBlocks;
    sVCB->numberOfBlocks = numberOfBlocks;
}

st_vcb *formatVolume(int blockSize, int numberOfBlocks) {
    int nbBlocksWrote = 0;
    st_vcb *rVCB = malloc(blockSize);

    // Make sure that our vcb structure is correctly filled
    initializeVCB(rVCB, numberOfBlocks, blockSize);
    rVCB->indexFreeSpace = initializeFreeSpace(blockSize, numberOfBlocks);
    if (rVCB->indexFreeSpace == -1)
        return (NULL);
    rVCB->startDirectory = initializeDirectories(rVCB, blockSize, numberOfBlocks);
    if (rVCB->startDirectory == -1)
        return (NULL);
    // Write the vcb to block 0
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

    // Try to fetch an existing volume by reading block 0.
    uint64_t rvRead = LBAread(sVCB, 1, 0);
    if (rvRead == -1) {
        printf("Error while reading\n");
        return (NULL);
    }
    // Here we find out if we need to create a new volume or not
    if (sVCB->signature == PART_SIGNATURE) {
        printf("Not formatting\n");
        readFreeSpace(sVCB, blockSize, numberOfBlocks);
        return (sVCB);
    } else {
        free(sVCB);
        printf("Formatting...\n");
        printf("Test\n");
        fflush(NULL);
        return (formatVolume(blockSize, numberOfBlocks));
    }
}

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize) {
    printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    // Check if a volume already exists. If it doesn't we will create a new one
    sVCB = checkIfVolumeExists(numberOfBlocks, blockSize);

    if (sVCB == NULL)
        return (-1);
    return (0);
}

st_vcb* returnVCBRef(){
    return sVCB;
}


void exitFileSystem () {
    printf ("System exiting\n");
}
