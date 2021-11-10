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

#include "fsDirOperations.h"
#include "vcb.h"
#include "fsParsePath.h"

fdDir* fs_openDir(const char *name) {
    //vcb malloc'd by this point\
    //bogus code, working through compiler faults
    st_vcb *sVCB = returnVCBRef();
    struct st_directory *nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, name);
    int dir = opendir(name);
    if (dir == NULL){
        printf("No such directory\n");
        return NULL;
    }
    // CHECK IF VALUE RETURNED CORRECT OR NOT
    //fdDir *fDir = malloc();
    printf("--------------Directory Found----------------\n");
    return 0;
}
