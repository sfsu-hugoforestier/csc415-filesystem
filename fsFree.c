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

int getLocationLastBlock(struct st_block **head) {
    // Loop through the linked list to return the last block location
    struct st_block *lBlock = *head;
    while (lBlock->next != NULL) {
        lBlock = lBlock->next;
    }
    return (lBlock->location);
}

int addNewBlockToEnd(struct st_block **head, int isFree) {
    struct st_block *nBlock = malloc(sizeof(struct st_block));
    int nbBlocksWrote = 0;

    if (nBlock == NULL) {
        printf("Error while mallocing new block\n");
        return (-1);
    }

    // Loop until the end of our linked list, in order to add a new node at
    // the end
    struct st_block *lBlock = *head;
    while (lBlock->next != NULL) {
        lBlock = lBlock->next;
    }
    nBlock->isFree = isFree;
    nBlock->next = NULL;
    nBlock->location = lBlock->location + 2; // Because location + 1 is the directory data
    lBlock->next = nBlock;
    // Make sure that the volume is updated
    nbBlocksWrote = LBAwrite(lBlock, 1, lBlock->location);
    if (nbBlocksWrote != 1) {
        printf("Error while writing\n");
        return (-1);
    }

    nbBlocksWrote = LBAwrite(nBlock, 1, nBlock->location);
    if (nbBlocksWrote != 1) {
        printf("Error while writing\n");
        return (-1);
    }
    return (nBlock->location + 1); // So the function calling the function write just after the block info
}

int freeBlock(struct st_block **head, int location) {
    struct st_block *lBlock = *head;
    int nbBlocksWrote = 0;

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
    // The free the block and write it to the volume
    lBlock->isFree = 0;
    nbBlocksWrote = LBAwrite(lBlock, 1, lBlock->location);
    if (nbBlocksWrote != 1) {
        printf("Error while writing\n");
        return (-1);
    }
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
