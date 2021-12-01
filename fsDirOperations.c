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

#include "fsParsePath.h"
#include "fsDirOperations.h"
#include "vcb.h"
#include "mfs.h"

fdDir *fs_opendir(const char *name) {
    st_vcb *sVCB = returnVCBRef();
    fdDir *fDir = malloc(sizeof(fdDir));
    struct st_directory * incomingDir = NULL;

    // CHECK IF VALUE RETURNED CORRECT OR NOT
    //parsePath();
    if (fDir == NULL){
        printf("Error mallocing fDir");
        return (NULL);
    }

    incomingDir = parsePath(sVCB->startDirectory, sVCB->blockSize, (char*)name);
    if (incomingDir == NULL) {
        printf("Error locating directory");
        return (NULL);
    } else if (incomingDir->isDirectory == FALSE) {
        printf("Not a directory");
        return (NULL);
    }
    //first directory in open at block location with our nDir will be written over
    printf("--------------Directory Found----------------\n");
    fDir->directoryStartLocation = incomingDir->startBlockNb;
    fDir->dirEntryPosition = 0;
    //strcpy(name, fDir->path);
    fDir->d_reclen = incomingDir->nbDir;
    strcpy(fDir->directoryPath, name);
    return(fDir);
}

unsigned short readdirCounter = 0;

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    struct fs_diriteminfo *dirInfo = malloc(sizeof(struct fs_diriteminfo) * dirp->d_reclen);
    struct st_directory * nDir;
    st_vcb *sVCB = returnVCBRef();

    if (dirInfo == NULL) {
        printf("Error while mallocing dirInfo\n");
        return (NULL);
    }

    if (dirp->dirEntryPosition > dirp->d_reclen)
        return (NULL);
    // TODO: assignment makes integer from pointer without a cast
    if (dirp->directoryStartLocation == -1) { // TO CHANGE PROBABLY
        //reset to logical block address of '.'
        dirp->directoryStartLocation = 0;
        return (NULL);
    }
    nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, dirp->directoryPath);
    if (nDir == NULL){
        printf("Error locating directory");
        return (NULL);
    }
    dirInfo->d_reclen = nDir->nbDir;
    if (nDir->isDirectory == TRUE) {
        dirInfo->fileType = DT_DIR;
    } else {
        dirInfo->fileType = DT_REG;
    }

    strcpy(dirInfo->d_name, nDir[dirp->dirEntryPosition].name);
    dirp->dirEntryPosition++;
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
    fDir = parsePath(sVCB->startDirectory, sVCB->blockSize, (char*)path);
    if (fDir == NULL) {
        printf("Directory stats not retrievable from incoming path");
        return 0;
    }
    //read from our
    //LBAread(buf, sVCB->numberOfBlocks, sVCB->startDirectory);
    if(name) {
        //size in bytes from malloc of directory
        buf->st_size = fDir->sizeDirectory;
        // this will probably nag about casting
        buf->st_createtime = fDir->creationDate;
        buf->st_modtime = fDir->lastModDate;
    }
    //pull back Hugo's parsepath and set our stats with buffer
    //fix the reference error in lines 200 of the shell, argvc[k]. arguments coming in ./* for all
    // A VOIR
    return (0);
    // two arguments or more
}
