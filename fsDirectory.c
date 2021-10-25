/**************************************************************
* Class:  CSC-415-0# - Fall 2021
* Names:
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: 
*
* Description:
*
**************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "fsDirectory.h"


//280 bytes total
st_dir newRoot = malloc(numDirectoryEntry);

int makeRoot(){
    
    int totalSizeNeeded = directorySize * numDirectoryEntry;
    int totalBlocks = totalSizeNeeded/BLOCK_SIZE;
    int remBlocks = totalSizeNeeded%BLOCK_SIZE;
    if(rem > 0){
        totalBlocks++;
    } 
    printf("BLOCKS - %d",totalBlocks);

    newRoot->name[255] = ".";
    newRoot->id = 1;

    newRoot->parentId = 0;


    newRoot->location;
    newRoot->numFilesInside;

    return 0;
}


