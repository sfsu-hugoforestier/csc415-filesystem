/**************************************************************
* Class:  CSC-415
* Name: DE LA ROSA Adrian
* Student ID: 916835002
* Project: Basic File System
*
* File: fsDirOperations.h
*
* Description:
* Directory operations and neccessary helper functions for directory traversal
*
**************************************************************/

#include "fsDirOperations.h"
#include "vcb.h"
#include "fsParsePath.h"
#include "mfs.h"

fdDir *fs_openDir(const char *name) {
    st_vcb *sVCB = returnVCBRef();
    fdDir *fDir = malloc(sizeof(fdDir));
    //will this work with updated parse path?
    struct st_directory *nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, name);

    // CHECK IF VALUE RETURNED CORRECT OR NOT
    //parsePath();
    if (fDir == NULL) {
        printf("Error while mallocing fDir\n");
        return (NULL);
    }

    //first directory in open at block location with our nDir will be written over
    fDir->directoryStartLocation = nDir->startBlockNb;
    fDir->dirEntryPosition = 0;
    fDir->d_reclen = nDir->nbDir;
    //fDir->fs_diriteminfo = ;
    printf("--------------Directory Found----------------\n");
    return (fDir);
}

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    struct fs_diriteminfo *dirInfo = malloc(sizeof(struct fs_diriteminfo) * dirp->d_reclen);
    //int *dirInfo = *dirp->fs_diriteminfo;
    if (dirInfo == NULL) {
        printf("Error while mallocing dirInfo\n");
        return (NULL);
    }
    if (dirp->directoryStartLocation = NULL){
        dirp->directoryStartLocation = 0;
        return NULL;
    }
    if(dirp->d_reclen > dirp->dirEntryPosition){

    }
    char path[64];
    strcpy(dirInfo->d_name, path);
    
    //dirInfo pointer with lesser value until end when dirp->d_reclen set to 0 again
    //make index with d_reclen
    
    return (dirInfo);
}  

int fs_closedir(fdDir *dirp) {
    return 0;
}
