/**************************************************************
* Class:  CSC-415
* Name: DE LA ROSA Adrian
* Student ID: 916835002
* Project: Basic File System
*
* File: fsDirOperations.h
*
* Description:
* Directory operations
*
**************************************************************/
#ifndef PARSE_PATH_H_
#define PARSE_PATH_H_

#include "fsDirectory.h"
#include "fsLow.h"
#include <stdio.h>
#include <string.h>
#include "mfs.h"
#include "vcb.h"

fdDir *fs_opendir(const char *name);
struct fs_diriteminfo *fs_readdir(fdDir *dirp);
int fs_closedir(fdDir *dirp);

#endif
