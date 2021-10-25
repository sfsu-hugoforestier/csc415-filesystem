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

#include "vcb.h"
#include <stddef.h>

struct st_block {
    int location;
    int isFree;
    struct st_block *next;
};

struct st_block *initializeFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks);
void addNewBlockToEnd(struct st_block **head, int isFree, int location);
int freeBlock(struct st_block **head, int location);

#endif
