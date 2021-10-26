#include "fsDirectory.h"
#include <stdio.h>
#include <string.h>
#include "fsLow.h"

int initializeDirectory(struct st_directory **head, char *name, int isDirectory, struct st_block **hBlocks, int parentId) {
    struct st_directory *nDirectory = malloc(sizeof(struct st_directory));
    time_t t = time(NULL);
    int nbBlocksWrote = 0;
    int locationBlock = addNewBlockToEnd(hBlocks, 1);
    struct st_directory *lDirectory = *head;

    if (nDirectory == NULL || locationBlock == -1) {
        printf("Error while mallocing new directory\n");
        return (-1);
    }

    // Loop until the end of our linked list, in order to add a new node at the end
    while (lDirectory->next != NULL) {
        lDirectory = lDirectory->next;
    }
    // Fill the struct with the values given as arguments
    strncpy(nDirectory->name, name, 64);
    nDirectory->location = locationBlock;
    nDirectory->id = lDirectory->id + 1;
    nDirectory->parentId = parentId;
    nDirectory->lastAccess = time(&t);
    nDirectory->lastModDate = time(&t);
    nDirectory->creationDate = time(&t);
    nDirectory->isDirectory = isDirectory;
    lDirectory->next = nDirectory;

    // Rewrite the last directory, to change next directory value
    nbBlocksWrote = LBAwrite(lDirectory, 1, lDirectory->location);
    if (nbBlocksWrote != 1) {
        printf("Error while writing\n");
        return (-1);
    }
    // Write to the volume the newly created directory.
    nbBlocksWrote = LBAwrite(nDirectory, 1, locationBlock);
    if (nbBlocksWrote != 1) {
        printf("Error while writing\n");
        return (-1);
    }
    return (0);
}

int initializeFirstDirectory(struct st_directory *sDirectory, struct st_block **hBlocks) {
    time_t t = time(NULL);
    int nbBlocksWrote = 2;
    // If i ask for a new block here, I'm going to skip a block because I already initialized a block with the function initializeFreeSpace.
    // That's why in initializeFreeSpace I specified that the block wasn't free so that I don't have to call for a new block here.
    int locationBlock = 2;

    // Initialize the values for the first directory.
    strcpy(sDirectory->name, "..");
    sDirectory->location = locationBlock;
    sDirectory->id = 1;
    sDirectory->parentId = -1;
    sDirectory->lastAccess = time(&t);
    sDirectory->lastModDate = time(&t);
    sDirectory->creationDate = time(&t);
    sDirectory->next = NULL;
    // Make sure that I write the datas to the volume
    nbBlocksWrote = LBAwrite(sDirectory, 1, locationBlock);
    if (nbBlocksWrote != 1)
        return (-1);
    return (0);
}

struct st_directory *initializeDirectories(struct st_block **hBlocks) {
    struct st_directory *sDirectory = malloc(sizeof(struct st_directory));

    if (sDirectory == NULL) {
        printf("Error while mallocing directory\n");
        return (NULL);
    }
    if (initializeFirstDirectory(sDirectory, hBlocks) != 0) {
        printf("Error while initializing first directory\n");
        return (NULL);
    }
    if (initializeDirectory(&sDirectory, ".", 1, hBlocks, 1) != 0) {
        printf("Error while initializing second directory\n");
        return (NULL);
    }
    return (sDirectory);
}
