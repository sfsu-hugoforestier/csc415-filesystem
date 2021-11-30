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

unsigned char *fMap = NULL;

int readFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks) {
    int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
    int nbWrote = 0;

    fMap = calloc(fsBlockCount, blockSize);
    nbWrote = LBAread(fMap, fsBlockCount, 1);
    if (nbWrote != fsBlockCount) {
        printf("Error while reading the free map\n");
        return (-1);
    }
    return (0);
}

int checkIfFree(int h) {
    return (fMap[h] & (1 << h));
}

int getFreeSpace(st_vcb *sVCB, int nbBlocks, int blockSize, int numberOfBlocks) {
    int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
    int nbWrote = 0;
    int i = 0;
    char orMap[8];
    int foundSpace = FALSE;
    int end = FALSE;

    orMap[0] = 0x80;
    orMap[1] = 0x40;
    orMap[2] = 0x20;
    orMap[3] = 0x10;
    orMap[4] = 0x08;
    orMap[5] = 0x04;
    orMap[6] = 0x02;
    orMap[7] = 0x01;
    for (; i != sVCB->indexFreeSpace && foundSpace != TRUE; i++) {
        for (int h = i; h != i + nbBlocks && end != TRUE; h++) {
            if (checkIfFree(h) == FALSE)
                end = TRUE;
        }
        if (end != TRUE) {
            foundSpace = TRUE;
        }
    }
    //i = sVCB->indexFreeSpace;
    for (int y = i; y != (i + nbBlocks); y++) {
        fMap[y / 8] |= orMap[y % 8];
    }
    sVCB->indexFreeSpace += nbBlocks;
    nbWrote = LBAwrite(fMap, fsBlockCount, 1);
    if (nbWrote != fsBlockCount) {
        printf("Error while writing the fMap to the volume\n");
        return (-1);
    }
    nbWrote = LBAwrite(sVCB, 1, 0);
    if (nbWrote != 1) {
        printf("Error while writing the VCB to the volume\n");
        return (-1);
    }
    return (sVCB->indexFreeSpace - nbBlocks);
}

int freeSpace(int startBlock, int nbBlock) {
    char andMap[8];
    st_vcb *VCBRef = returnVCBRef();
    int fsBlockCount = (((VCBRef->numberOfBlocks + 7) / 8) + (VCBRef->blockSize - 1)) / VCBRef->blockSize;
    int nbWrote = 0;
    andMap[0] = 0x7F;
    andMap[1] = 0xBF;
    andMap[2] = 0xDF;
    andMap[3] = 0xEF;
    andMap[4] = 0xF7;
    andMap[5] = 0xFB;
    andMap[6] = 0xFD;
    andMap[7] = 0xFE;

    for (int i = startBlock; i != startBlock + nbBlock; i++) {
        fMap[i / 8] &= andMap[i % 8];
    }
    nbWrote = LBAwrite(fMap, fsBlockCount, 1);
    if (nbWrote != fsBlockCount) {
        printf("Error while writing the fMap to the volume\n");
        return (-1);
    }
    return (0);
}

int initializeFreeSpace(int blockSize, int numberOfBlocks) {
    int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
    int i = 0;
    int nbWrote = 0;
    char orMap[8];

    orMap[0] = 0x80;
    orMap[1] = 0x40;
    orMap[2] = 0x20;
    orMap[3] = 0x10;
    orMap[4] = 0x08;
    orMap[5] = 0x04;
    orMap[6] = 0x02;
    orMap[7] = 0x01;

    fMap = calloc(fsBlockCount, blockSize);
    for (; i != fsBlockCount; i++) {
      fMap[i / 8] |= orMap[i % 8];
    }
    nbWrote = LBAwrite(fMap, fsBlockCount, 1);
    if (nbWrote != fsBlockCount) {
        printf("Error while writing the fMap to the volume\n");
        return (-1);
    }
    return (i);
}