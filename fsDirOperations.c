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
#include "mfs.h"


struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    struct fs_diriteminfo *dirInfo = malloc(sizeof(struct fs_diriteminfo) * dirp->d_reclen);

    if (dirInfo == NULL) {
        printf("Error while mallocing dirInfo\n");
        return (NULL);
    }

    strcpy(dirInfo->d_name, dirp);
    //strcpy(dirInfo->d_name, );

    // loop for as long as d_reclen is greater than entry position
    dirInfo->d_reclen = dirp->d_reclen;
    //finish loop
    if (dirInfo->fileType = FT_DIRECTORY) {
    }
    //dirInfo->fileType = ;
    return (dirInfo);
} //while this is no NULL print 

fdDir *fs_openDir(const char *name) {
    st_vcb *sVCB = returnVCBRef();
    fdDir *fDir = malloc(sizeof(fdDir));
    struct st_directory *nDir = parsePath(sVCB->startDirectory, sVCB->blockSize, name);

    // CHECK IF VALUE RETURNED CORRECT OR NOT
    parsePath();

    if (fDir == NULL) {
        printf("Error while mallocing fDir\n");
        return (NULL);
    }

    fDir->directoryStartLocation = nDir->startBlockNb;
    fDir->dirEntryPosition = 0;
    fDir->d_reclen = nDir->nbDir;
    printf("--------------Directory Found----------------\n");
    return (fDir);
}

int fs_closedir(fdDir *dirp) {
    return 0;
}
