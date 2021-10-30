/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: fsFree.c
*
* Description:
* File where we manage our blocks
*
**************************************************************/
#include "fsFree.h"
#include "fsLow.h"
#include "vcb.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char *fMap;

int readFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks) {
    int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
    int nbWrote = 0;


    fMap = malloc(fsBlockCount * blockSize);
    nbWrote = LBAread(fMap, fsBlockCount, sVCB->indexFreeSpace);
    printf("fsBlockCount: %i\n", fsBlockCount);
    if (nbWrote != fsBlockCount) {
        printf("Error while reading the free map\n");
        return (-1);
    }
    return (0);
}

int getFreeSpace(st_vcb *sVCB, int nbBlocks, int blockSize, int numberOfBlocks) {
    int nbWrote = 0;
    int i = 0;
    int foundSpace = FALSE;
    int end = FALSE;

    for (; i != sVCB->indexFreeSpace && foundSpace != TRUE; i++) {
        for (int h = i; h != i + nbBlocks && end != TRUE; h++) {
            if (fMap[h] != 0) {
                end = TRUE;
            }
        }
        if (end != TRUE) {
            foundSpace = TRUE;
        }
    }

    for (int y = i; y != (i + nbBlocks); y++) {
        fMap[y] = 1;
    }
    sVCB->indexFreeSpace += nbBlocks;
    nbWrote = LBAwrite(fMap, 5, 1);
    if (nbWrote != 5) {
        printf("Error while writing the fMap to the volume\n");
        return (-1);
    }
    return (sVCB->indexFreeSpace - nbBlocks);
}

int freeSpace(int startBlock, int nbBlock) {
    for (int i = startBlock; i != startBlock + nbBlock; i++) {
        fMap[i] = 0;
    }
    return (0);
}

int initializeFreeSpace(int blockSize, int numberOfBlocks) {
    int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
    int i = 0;
    int nbWrote = 0;

    fMap = malloc(fsBlockCount * blockSize);
    for (; i != 5; i++) {
      fMap[i] = 1;
    }
    nbWrote = LBAwrite(fMap, 5, 1);
    if (nbWrote != 5) {
        printf("Error while writing the fMap to the volume\n");
        return (-1);
    }
    return (i);
}
