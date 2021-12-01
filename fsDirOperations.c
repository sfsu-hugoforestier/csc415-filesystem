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
* functions for fs_opendir(), fs_readdir(), fs_closedir(), fs_stat()
**************************************************************/

#include "fsParsePath.h"
#include "fsDirOperations.h"
#include "vcb.h"
#include "mfs.h"


fdDir *fs_opendir(const char *name) {
    // Reference to our st_vcb which contains data needed for parsePath: 
    // Location, Blocksize and Path
    st_vcb *sVCB = returnVCBRef();
    // Malloc a new fdDir structure to return on completion after setting fields
    fdDir *fDir = malloc(sizeof(fdDir));
    // Struct st_directory pointer initliazed to NULL, which will be used for as landing for our parsePath()
    struct st_directory * incomingDir = NULL;
    
    char *copyName = malloc(sizeof(name));
    char *copyName2 = malloc(sizeof(name));

    // If fdDir fails to malloc or our malloc of char name returns NULL, return NULL and exit
    if (fDir == NULL || copyName == NULL) {
        printf("Error mallocing fDir\n");
        return (NULL);
    }
    strcpy(copyName, name);
    // Landing for our parsePath and incomingDir will be used to fill our fDir struct
    // Though nly if it tracks through these two conditions below 
    incomingDir = parsePath(sVCB->startDirectory, sVCB->blockSize, copyName);
    if (incomingDir == NULL) {
        printf("Error locating directory\n");
        return (NULL);
    } else if (incomingDir->isDirectory == FALSE) {
        printf("Not a directory\n");
        return (NULL);
    }
    // Writing our validated parsePath data into our fDir structure and returning the struct
    fDir->directoryStartLocation = incomingDir->startBlockNb;
    fDir->dirEntryPosition = 0;
    fDir->d_reclen = incomingDir->nbDir;
    strcpy(fDir->directoryPath, name);
    return(fDir);
}

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    // Reference to our st_vcb which contains data needed for parsePath: 
    // Location, Blocksize and Path
    st_vcb *sVCB = returnVCBRef();
    // Malloc a new fs_diriteminfo struct to return after setting fields
    struct fs_diriteminfo *dirInfo = malloc(sizeof(struct fs_diriteminfo) * dirp->d_reclen);
    // Landing for our parsePath
    struct st_directory * nDir;
    // if our malloc fails, return NULL
    if (dirInfo == NULL) {
        printf("Error while mallocing dirInfo\n");
        return (NULL);
    }
    // if our incoming dirEntryPos is greater than our d_reclen, return NULL
    if (dirp->dirEntryPosition > dirp->d_reclen) {
        return (NULL);
    }
    if (dirp->directoryStartLocation == -1) {
        //Resets to directory start location '.'
        dirp->directoryStartLocation = 0;
        return (NULL);
    }
    // Landing for our parsePath and nDir will be used to fill our dirInfo 
    nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, dirp->directoryPath);
    //No path exists, exit
    if (nDir == NULL){
        printf("Error locating directory\n");
        return (NULL);
    }
    // d_reclen equal to the number of items in this directory
    dirInfo->d_reclen = nDir->nbDir;
    // populate filetype field in dirInfo depending on isDirectory?
    if (nDir->isDirectory == TRUE) {
        dirInfo->fileType = DT_DIR;
    } else {
        dirInfo->fileType = DT_REG;
    }
    // populate name per position
    strcpy(dirInfo->d_name, nDir[dirp->dirEntryPosition].name);
    // Increment on our position in the directory and return our filled dirInfo
    dirp->dirEntryPosition++;
    return (dirInfo);
}

int fs_closedir(fdDir *dirp) {
    //Free our fdDir struct and return 0
    free(dirp);
    dirp = NULL;
    return 0;
}

int fs_stat(const char *path, struct fs_stat *buf) {
    // Reference to our st_vcb which contains data needed for parsePath: 
    // Location, Blocksize and Path
    st_vcb *sVCB = returnVCBRef();
    // Landing for parsePath
    struct st_directory *fDir;
    fDir = parsePath(sVCB->startDirectory, sVCB->blockSize, (char*)path);
    // On failure, return 0, else, fill our fDir with our buffer and return 0
    if (fDir == NULL) {
        printf("Directory stats not retrievable from incoming path\n");
        return 0;
    }
    if(path) {
        buf->st_size = fDir->sizeDirectory;
        buf->st_createtime = fDir->creationDate;
        buf->st_modtime = fDir->lastModDate;
    }
    return (0);
}
