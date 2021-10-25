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
#include <stdio.h>
#include <stdlib.h>

void addNewBlockToEnd(struct st_block **head, int isFree, int location) {
    struct st_block *nBlock = malloc(sizeof(struct st_block));

    if (nBlock == NULL) {
        printf("Error while mallocing new block\n");
        return;
    }

    struct st_block *lBlock = *head;
    // Loop until the end of our linked list, in order to add a new node at
    // the end
    while (lBlock->next != NULL) {
        lBlock = lBlock->next;
    }
    nBlock->isFree = 1;
    nBlock->next = NULL;
    nBlock->location = location;
    lBlock->next = nBlock;
}

int freeBlock(struct st_block **head, int location) {
    struct st_block *lBlock = *head;
    // Make sure that if we stop by checking if next != NULL. In case Location
    // is unknown
    while (lBlock->next != NULL || lBlock->location != location) {
        lBlock = lBlock->next;
    }
    // Return an error if we can't free the block
    if (lBlock->next == NULL) {
        printf("Error while trying to free the block. Location out of range\n");
        return (-1);
    }
    // The block is now free
    lBlock->isFree = 0;
    return (0);
}

struct st_block *initializeFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks) {
    struct st_block *sBlock = malloc(sizeof(struct st_block));
    int nbBlocksWrote = 0;

    // We initialize our block function, and write it to block 1
    if (sBlock == NULL) {
        printf("Error while mallocing first block\n");
        return (NULL);
    }
    sBlock->isFree = 1;
    sBlock->location = 1;
    sBlock->next = NULL;
    nbBlocksWrote = LBAwrite(sBlock, 1, 1);
    if (nbBlocksWrote == -1) {
        printf("Error while writing\n");
        return (NULL);
    }
    return (sBlock);
}
