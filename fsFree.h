/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: fsFree.h
*
* Description:
* File where we create our block structure
*
**************************************************************/
#ifndef FSFREE_H_
#define FSFREE_H_

#define TRUE (1)
#define FALSE (0)

#include "vcb.h"
#include <stddef.h>

int initializeFreeSpace(int blockSize, int numberOfBlocks);
int getFreeSpace(st_vcb *sVCB, int nbBlocks, int blockSize, int numberOfBlocks);
int readFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks);
int freeSpace(int startBlock, int nbBlock);
int readFreeMap(st_vcb *sVCB, int blockSize, int numberOfBlocks);

#endif
