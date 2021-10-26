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

#include <time.h>
#include <stdlib.h>
#include "fsFree.h"

struct st_directory {
    char name[64];
    int location;
    int id;
    int parentId;
    time_t creationDate;
    time_t lastModDate;
    time_t lastAccess;
    int isDirectory;
    struct st_directory *next;
};

struct st_directory *initializeDirectories(struct st_block **hBlocks);

#endif
