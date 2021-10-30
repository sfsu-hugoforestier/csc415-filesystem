/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: fsDirectory.h
*
* Description:
* File with the directory structure and functions types for fsDirectory.c
*
**************************************************************/
#ifndef FSDIRECTORY_H_
#define FSDIRECTORY_H_

#define DIRECTORY_ENTRIES (50)
#define TRUE (1)
#define FALSE (0)

#include <time.h>
#include <stdlib.h>
#include "fsFree.h"

struct st_directory {
    int isFree;
    int startBlockNb;
    int id;
    int parentId;
    int isDirectory;
    int sizeDirectory;
    time_t creationDate;
    time_t lastModDate;
    char name[64];
};

int initializeDirectories(st_vcb *rVCB, int blockSize, int numberOfBlocks);

#endif
