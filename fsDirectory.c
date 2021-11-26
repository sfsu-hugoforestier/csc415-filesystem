#include "fsDirectory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsLow.h"
#include "vcb.h"

#include "mfs.h"
#include "fsParsePath.h"
#include "fsUtils.h"

#define DIRMAX_LEN 4096

struct st_directory *initDefaultDir(struct st_directory *sDir, int iBlock, int sizeMallocDir, int nbDir) {
    time_t t = time(NULL);

    for (int i = 0; i != nbDir; i++) {
        sDir[i].isFree = TRUE;
    }
    sDir[0].nbDir = nbDir;
    sDir[0].isFree = FALSE;
    sDir[0].startBlockNb = iBlock;
    sDir[0].creationDate = time(&t);
    sDir[0].lastModDate = time(&t);
    strcpy(sDir[0].name, ".");
    sDir[0].isDirectory = TRUE;
    sDir[0].sizeDirectory = sizeMallocDir;

    sDir[1].nbDir = nbDir;
    sDir[1].isFree = FALSE;
    sDir[1].startBlockNb = iBlock;
    sDir[1].creationDate = time(&t);
    sDir[1].lastModDate = time(&t);
    strcpy(sDir[1].name, "..");
    sDir[1].isDirectory = TRUE;
    sDir[1].sizeDirectory = sizeMallocDir;
    return (sDir);
}

int initializeDirectories(st_vcb *rVCB, int blockSize, int numberOfBlocks) {
    struct st_directory *sDir;
    unsigned int end = 0;
    unsigned int iBlock = 0;
    unsigned int nbDir = DIRECTORY_ENTRIES;
    unsigned int sizeMallocDir = sizeof(struct st_directory) * DIRECTORY_ENTRIES;
    unsigned int nbBlocks = (sizeMallocDir / blockSize) + 1;
    unsigned int maxSize = nbBlocks * blockSize;

    while (sizeMallocDir < maxSize && end != 1) {
        if (sizeMallocDir + sizeof(struct st_directory) < maxSize) {
            sizeMallocDir+=(sizeof(struct st_directory));
            nbDir+=1;
        } else {
            end = 1;
        }
    }
    sDir = calloc(nbDir, sizeof(struct st_directory));
    if (sDir == NULL) {
        printf("Error while allocating the memory for sDir\n");
        return (-1);
    }

    iBlock = getFreeSpace(rVCB, nbBlocks, blockSize, numberOfBlocks);
    sDir = initDefaultDir(sDir, iBlock, sizeMallocDir, nbDir);
    LBAwrite(sDir, nbBlocks, iBlock);

    free(sDir);
    return (iBlock);
}

struct st_directory *initializeNewDir(struct st_directory *nDir, int iBlock, struct st_directory *cwdDir) {
    time_t t = time(NULL);

    for (int i = 0; i != cwdDir[0].nbDir; i++) {
        nDir[i].isFree = TRUE;
    }

    nDir[0].nbDir = cwdDir[0].nbDir;
    nDir[0].isFree = FALSE;
    nDir[0].startBlockNb = iBlock;
    nDir[0].creationDate = time(&t);
    nDir[0].lastModDate = time(&t);
    strcpy(nDir[0].name, ".");
    nDir[0].isDirectory = TRUE;
    nDir[0].sizeDirectory = cwdDir[0].sizeDirectory;

    nDir[1].nbDir = cwdDir[0].nbDir;
    nDir[1].isFree = FALSE;
    nDir[1].startBlockNb = cwdDir[0].startBlockNb;
    nDir[1].creationDate = time(&t);
    nDir[1].lastModDate = time(&t);
    strcpy(nDir[1].name, "..");
    nDir[1].isDirectory = TRUE;
    nDir[1].sizeDirectory = cwdDir[0].sizeDirectory;

    return (nDir);
}

int createDir(struct st_directory *cwdDir, int index, const char *pathname) {
    //create new dir
    struct st_directory *nDir = calloc(cwdDir[0].nbDir, sizeof(struct st_directory));
    st_vcb *VCBRef = returnVCBRef();
    int iBlock = 0;
    unsigned int nbBlocks = (cwdDir[0].sizeDirectory / VCBRef->blockSize) + 1;
    time_t t = time(NULL);

    if (nDir == NULL) {
        printf("Error while allocating the memory for sDir\n");
        return (-1);
    }

    for (int i = 0; i != cwdDir[0].nbDir; i++)
        nDir[i].isFree = TRUE;

    iBlock = getFreeSpace(VCBRef, nbBlocks, VCBRef->blockSize, VCBRef->numberOfBlocks);
    nDir = initializeNewDir(nDir, iBlock, cwdDir);
    cwdDir[index].nbDir = cwdDir[0].nbDir;
    cwdDir[index].isFree = FALSE;
    cwdDir[index].startBlockNb = nDir[0].startBlockNb;
    cwdDir[index].creationDate = time(&t);
    cwdDir[index].lastModDate = time(&t);
    strcpy(cwdDir[index].name, pathname);
    cwdDir[index].isDirectory = TRUE;
    cwdDir[index].sizeDirectory = cwdDir[0].sizeDirectory;

    //LBAWrite - both cwd and new dir
    LBAwrite(nDir, nbBlocks, iBlock);
    LBAwrite(cwdDir, nbBlocks, cwdDir[0].startBlockNb);

    printf("iBlock: %i\n", iBlock);
    free(nDir);
    nDir = NULL;
    return (0);
}

//TODO: TO REMOVE AFTER MERGE
char *fs_getcwd(char *buf, size_t length) {
    strcpy(buf, "/");
    return (buf);
}

//TODO: Remove directories that are in nested directories, not within the CWD dir
int fs_mkdir(const char *pathname, mode_t mode) {
    struct st_directory *cwDir = NULL;
    int result = 0;
    char *dir_buf = malloc(DIRMAX_LEN + 1);
    char *sCWD = malloc(DIRMAX_LEN + 1);

    if(sCWD == NULL) {
        printf("MALLOC ERROR\n");
        return (-1);
    }

    sCWD = fs_getcwd(dir_buf, DIRMAX_LEN);
    // CHANGE THIS
    dir_buf = fs_getcwd(dir_buf, DIRMAX_LEN);
    cwDir = parsePath(returnVCBRef()->startDirectory, returnVCBRef()->blockSize, dir_buf);

    if (cwDir == NULL) {
        printf("cwDir is null\n");
        return (-1);
    }

    //Check if dir is already created
    for(int i = 0; i != cwDir[0].nbDir; i++) {
        if (strcmp(cwDir[i].name, pathname) == 0) {
            printf("ERROR: FILE already created\n");
            free(sCWD);
            free(cwDir);
            sCWD = NULL;
            cwDir = NULL;
            return (-1);
        }
    }
        printf("AA");

    //No child - create directory
    for(int i = 0; i != cwDir[0].nbDir; i++) {
        if(cwDir[i].isFree == TRUE) {
            result = createDir(cwDir, i, pathname);
            if(result < 0) {
                free(sCWD);
                free(cwDir);
                sCWD = NULL;
                cwDir = NULL;
                return (-1);
            }
            break;
        }
    }
    printDirectory(cwDir);
    free(sCWD);
    free(cwDir);
    sCWD = NULL;
    cwDir = NULL;
    return (0);
}

int fs_rmdir(const char *pathname){
    //printf("RM_DIR\n");

    //find Dir
    struct st_directory *nDir;
    struct st_directory *nCwd;
    char pathNameHolder[64];
    int found = 0;
    st_vcb *VCBRef = returnVCBRef();

    strcpy(pathNameHolder, pathname);
    //printf("BEFORE PARSE\n");
    nDir = parsePath(VCBRef->startDirectory, VCBRef->blockSize, pathNameHolder);

    //cant find - return error
    if(nDir == NULL){
        return -1;
    }

    //if it has children - return error not empty
    if(nDir[0].nbDir > 1){
        printf("ERROR: %s has children files\nCannot delete\n",nDir[0].name);
        return -1;
    }

    //else kill Dir
    //get cwd
    char * dir_buf = malloc (DIRMAX_LEN +1);
    char *cwd = malloc(DIRMAX_LEN +1);
    cwd = fs_getcwd(dir_buf,DIRMAX_LEN);
    nCwd = parsePath(VCBRef->startDirectory, VCBRef->blockSize, cwd);

    //search cwd for dir to delete
    for(int i = 0; i != nCwd[0].nbDir; i++){
       if(strcmp(nCwd[i].name, pathname) == 0){
          nCwd[i].isFree = TRUE;
          found = 1;
       }
    }
    if(found == 0){
        printf("ERROR: Directory %s not Found within Directory %s\n",pathname,nCwd[0].name);
        return -1;
    }

    //Reset freespace map bit
    unsigned int nbBlocks = (nDir[0].sizeDirectory/ VCBRef->blockSize) + 1;
    freeSpace(nDir->startBlockNb,nbBlocks);

    free (dir_buf);
    dir_buf = NULL;
    free (cwd);
    cwd = NULL;
    return 0;
}

//TODO: NEED TO DELETE FUNCTION BELOW AFTER MERGING
int fs_isDir(char * path)
{
    int i;
    for(i = 0; i < strlen(path); i++) {
        if(path[i] == '/') {
            return 1;
        }
    }
    return 0;
}


int fs_isFile(char * path)
{
    if(fs_isDir(path) == 1) {
        return 0;
    } else {
        return 1;
    }
}


int fs_delete(char* filename) {
    // REMOVE THE FILE
    return 0;
}