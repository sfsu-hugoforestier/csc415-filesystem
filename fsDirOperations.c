/**************************************************************
* Class:  CSC-415
* Name: DE LA ROSA Adrian
* Student ID: 916835002
* Project: Basic File System
*
* File: fsDirOperations.c
*
* Description:
* Directory operations and neccessary helper functions for directory traversal
* functions for fs_opendir(), fs_readdir(), fs_closedir()
**************************************************************/

#include "fsDirOperations.h"
#include "vcb.h"
#include "fsParsePath.h"
#include "mfs.h"

fdDir *fs_openDir(const char *name) {
    st_vcb *sVCB = returnVCBRef();
    fdDir *fDir = malloc(sizeof(fdDir));
    struct st_directory * incomingDir;
    //  TODO: ERROR implicit declaration of function ‘parsePath’
    //  TODO: ERROR initialization makes pointer from integer without a cast
    // CHECK IF VALUE RETURNED CORRECT OR NOT
    //parsePath();
    if (fDir == NULL){
        printf("Error mallocing fDir");
        return (NULL);
    }

    incomingDir = parsePath(sVCB->startDirectory, sVCB->blockSize, name);
    if (incomingDir == NULL) {
        printf("Error locating directory");
        return (NULL);
    } else if (incomingDir->isDirectory == FALSE) {
        printf("Not a directory");
        return 0;
    }
    //first directory in open at block location with our nDir will be written over
    printf("--------------Directory Found----------------\n");
    fDir->directoryStartLocation = incomingDir->startBlockNb;
    fDir->dirEntryPosition = incomingDir->nbDir;
    fDir->d_reclen = incomingDir->sizeDirectory;
    fDir->fs_dirItemInfoPointer = incomingDir->isDirectory;
    return(fDir);
}

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    struct fs_diriteminfo *dirInfo = malloc(sizeof(struct fs_diriteminfo) * dirp->d_reclen);
    //int *dirInfo = *dirp->fs_diriteminfo;
    short index;
    index = 0;
    if (dirInfo == NULL) {
        printf("Error while mallocing dirInfo\n");
        return (-1);
    }
    // TODO: assignment makes integer from pointer without a cast 
    if (dirp->directoryStartLocation = NULL){
        //reset to logical block address of '.'
        dirp->directoryStartLocation = 0;
        return (NULL);
    }
    //nbDir = dirEntry[0]
    if(index < dirp->d_reclen)
    {
        dirp->directoryStartLocation = dirp->dirEntryPosition; 
        char path[64];
        strcpy(dirInfo->d_name, path);
        dirp->d_reclen = dirInfo->d_reclen;
        index = dirInfo->d_reclen + index;
    }    
    return (dirInfo);
}  

int fs_closedir(fdDir *dirp) {
    free(dirp);
    dirp = NULL;
    return 0;
}

int fs_stat(const char *path, struct fs_stat *buf) {
    st_vcb *sVCB = returnVCBRef();
    struct st_directory *fDir;
    const char *name = path;
    // TODO: initialization makes pointer from integer without a cast
    fDir = parsePath(sVCB->startDirectory, 512, path);
    if (fDir == NULL){
        printf("Directory stats not retrievable from incoming path");
        return 0;
    }
    //read from our 
    LBAread(buf,sVCB->numberOfBlocks,sVCB->startDirectory);
    if(name){
        //size in bytes from malloc of directory
        buf->st_size = fDir->sizeDirectory;
        // this will probably nag about casting
        buf->st_blksize = (fDir->sizeDirectory + 1 / 512);
        buf->st_createtime = fDir->creationDate;
        buf->st_modtime = fDir->lastModDate; 
    }
    //pull back Hugo's parsepath and set our stats with buffer
    //fix the reference error in lines 200 of the shell, argvc[k]. arguments coming in ./* for all
    // two arguments or more
}