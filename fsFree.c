#include <stdio.h>
#include <stdlib.h>
#include "fsFree.h"
#include "fsLow.h"


void addNewBlockToEnd(struct st_block **head, int isFree, int location) {
    struct st_block *nBlock = malloc(sizeof(struct st_block));

    if (nBlock == NULL) {
        printf("Error while mallocing new block\n");
        return;
    }

    struct st_block *lBlock = *head;
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
    while (lBlock->next != NULL || lBlock->location != location) {
        lBlock = lBlock->next;
    }
    if (lBlock->next == NULL) {
        printf("Error while trying to free the block. Location out of range\n");
        return (-1);
    }
    lBlock->isFree = 0;
    return (0);
}

struct st_block *initializeFreeSpace(st_vcb *sVCB, int blockSize, int numberOfBlocks) {
    struct st_block *sBlock = malloc(sizeof(struct st_block));
    int nbBlocksWrote = 0;

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
