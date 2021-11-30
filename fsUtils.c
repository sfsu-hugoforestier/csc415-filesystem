/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: fsUtils.c
*
* Description:
* File with utils functions
*
**************************************************************/
#include "fsUtils.h"
#include <stdio.h>
#include <string.h>

void printDirectory(struct st_directory *dir) {
    for (int i = 0; i != dir[0].nbDir; i++) {
        if (strlen(dir[i].name) != 0)
            printf("%s\n", dir[i].name);
        //printf("%i\n", dir[i].isFree);
        //fflush(NULL);
    }
    fflush(NULL);
}

void printInfoDirectory(struct st_directory *dir) {
    for (int i = 0; i != dir[0].nbDir; i++) {
        if (strlen(dir[i].name) != 0) {
            printf("Name: %s\ncreationName: %li\nlastModDate: %li\nisdirectory: %i\nSizeDirectory: %i\n isFree: %i\nnbDir: %i\nstartBlockNb: %i\n", dir[i].name, dir[i].creationDate,
            dir[i].lastModDate, dir[i].isDirectory, dir[i].sizeDirectory,
            dir[i].isFree, dir[i].nbDir, dir[i].startBlockNb);
        }
    }
    fflush(NULL);
}
