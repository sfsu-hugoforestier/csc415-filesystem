/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: fsParsePath.h
*
* Description:
* File with the function declaration of fsParsePath.c
*
**************************************************************/
#ifndef PARSE_PATH_H_
#define PARSE_PATH_H_

#include "fsDirectory.h"
#include "fsLow.h"
#include <stdio.h>
#include <string.h>

struct st_directory *parsePath(int startDirectory, int blockSize, char *path);

#endif
