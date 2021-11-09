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

fdDir *fs_openDir(const char *name) {
    st_vcb *sVCB = returnVCBRef();
    struct st_directory *nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, name);

    // CHECK IF VALUE RETURNED CORRECT OR NOT
    //fdDir *fDir = malloc();

}
