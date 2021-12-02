#include "fsDirectory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsLow.h"
#include "vcb.h"
#include "mfs.h"
#include "fsParsePath.h"
#include "fsUtils.h"

// Fetch only the path string by removing the name of the directory
char *parsePathName(const char *pathname, char *parsedPath) {
    int nbSlash = getNbChar((char*)pathname, '/');
    int countSlash = 0;
    int lenPathname = strlen(pathname);
    char *tmpPathname = malloc(DIRMAX_LEN + 1);
    parsedPath = fs_getcwd(parsedPath, DIRMAX_LEN);

    if (tmpPathname == NULL) {
        printf("Error while mallocing tmpPathname\n");
        return (NULL);
    }
    strcpy(tmpPathname, pathname);
    if (nbSlash == 0) {
        return (parsedPath);
    }
    if (tmpPathname[lenPathname] == '/')
        tmpPathname[lenPathname] = '\0';
    for (int i = 0; tmpPathname[i] != '\0'; i++) {
        if (tmpPathname[i] == '/')
            countSlash++;
        if (countSlash == nbSlash) {
            tmpPathname[i] = '\0';
            strcat(parsedPath, tmpPathname);
            return (parsedPath);
        }
    }
    return (parsedPath);
}

// Fetch only the name of the directory from the path
char *fetchDirName(const char *pathname, char *folderName) {
    int countDelim = 0;
    int nbDelim = getNbChar((char*)pathname, '/');
    int lenPathname = strlen(pathname);

    strcpy(folderName, pathname);
    if (nbDelim == 0 || (nbDelim == 1 && (folderName[0] == '/' || folderName[lenPathname] == '/'))) {
        return (folderName);
    }
    for (int i = 0; i != lenPathname; i++) {
        if (folderName[0] == '/')
            countDelim++;
        if (folderName[0] == '/' && nbDelim == countDelim) {
            folderName++;
            return (folderName);
        }
        folderName++;
    }
    return (folderName);
}

// Init the default values for the struct of the directory
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

// Find the size to malloc the new structure for the directory, and allocate the space for it
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
    sDir = calloc(nbDir, sizeof(struct st_directory) + 1);
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

// Init the default values for the struct of the directory
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

int createDir(int nbDir, int index, const char *pathname) {
    struct st_directory *nDir = calloc(nbDir, sizeof(struct st_directory) + 1);
    struct st_directory *prevDir = calloc(nbDir, sizeof(struct st_directory) + 1);
    st_vcb *VCBRef = returnVCBRef();
    int iBlock = 0;
    unsigned int nbBlocks = 0;
    time_t t = time(NULL);
    char *parsedPathName = malloc(DIRMAX_LEN + 1);
    char *parsedFolderName = malloc(DIRMAX_LEN + 1);

    // Make sure that the callocs succeeded
    if (nDir == NULL || prevDir == NULL || parsedPathName == NULL
     || parsedFolderName == NULL) {
        printf("Error while allocating the memory\n");
        return (-1);
    }
    // Fetch the directory we want to create the directory in
    // Allocate the space, initialize the default values and add in the parent directory structure the
    // new directory we just created
    // Make sure we write it to the volume
    parsedPathName = parsePathName(pathname, parsedPathName);
    parsedFolderName = fetchDirName(pathname, parsedFolderName);
    prevDir = parsePath(returnVCBRef()->startDirectory, returnVCBRef()->blockSize, parsedPathName);
    nbBlocks = (prevDir[0].sizeDirectory / VCBRef->blockSize) + 1;
    iBlock = getFreeSpace(VCBRef, nbBlocks, VCBRef->blockSize, VCBRef->numberOfBlocks);
    nDir = initializeNewDir(nDir, iBlock, prevDir);
    prevDir[index].nbDir = nbDir;
    prevDir[index].isFree = FALSE;
    prevDir[index].startBlockNb = nDir[0].startBlockNb;
    prevDir[index].creationDate = time(&t);
    prevDir[index].lastModDate = time(&t);
    strcpy(prevDir[index].name, parsedFolderName);
    prevDir[index].isDirectory = TRUE;
    prevDir[index].sizeDirectory = prevDir[0].sizeDirectory;
    LBAwrite(nDir, nbBlocks, iBlock);
    LBAwrite(prevDir, nbBlocks, prevDir[0].startBlockNb);
    free(nDir);
    nDir = NULL;
    return (0);
}

int fs_mkdir(const char *pathname, mode_t mode) {
    struct st_directory *cwDir = NULL;
    int result = 0;
    char *dir_buf = malloc(DIRMAX_LEN + 1);
    char *sCWD = malloc(DIRMAX_LEN + 1);
    char *folderName = malloc(DIRMAX_LEN + 1);

    if(sCWD == NULL || folderName == NULL) {
        printf("MALLOC ERROR\n");
        return (-1);
    }

    // Fetch the parent directory structure of where we want to create a new directory
    sCWD = fs_getcwd(dir_buf, DIRMAX_LEN);
    sCWD = parsePathName(pathname, sCWD);
    cwDir = parsePath(returnVCBRef()->startDirectory, returnVCBRef()->blockSize, sCWD);

    if (cwDir == NULL) {
        printf("cwDir is null\n");
        return (-1);
    }
    folderName = fetchDirName(pathname, folderName);
    //Check if dir is already created
    for(int i = 0; i != cwDir[0].nbDir; i++) {
        if (strcmp(cwDir[i].name, folderName) == 0 && cwDir[i].isFree != TRUE) {
            printf("ERROR: FILE already created\n");
            free(sCWD);
            free(folderName);
            free(cwDir);
            sCWD = NULL;
            cwDir = NULL;
            folderName = NULL;
            return (-1);
        }
    }

    //No child - create directory
    for(int i = 0; i != cwDir[0].nbDir; i++) {
        if(cwDir[i].isFree == TRUE) {
            result = createDir(cwDir[0].nbDir, i, pathname);
            if(result < 0) {
                free(sCWD);
                free(cwDir);
                free(folderName);
                sCWD = NULL;
                cwDir = NULL;
                folderName = NULL;
                return (-1);
            }
            break;
        }
    }

    free(sCWD);
    free(cwDir);
    sCWD = NULL;
    cwDir = NULL;
    folderName = NULL;
    return (0);
}

int findChildren(struct st_directory *nDir) {
    // Make sure that we dont delete a directory with children
    for (int i = 0; i != nDir[0].nbDir; i++) {
        if (strlen(nDir[i].name) != 0 &&
        strcmp(nDir[i].name, ".") != 0 && strcmp(nDir[i].name, "..") != 0)
            return (1);
    }
    return (0);
}

int fs_rmdir(const char *pathname){
    struct st_directory *nDir;
    struct st_directory *nCwd;
    char pathNameHolder[64];
    int found = 0;
    st_vcb *VCBRef = returnVCBRef();
    char * dir_buf = malloc(DIRMAX_LEN + 1);
    unsigned int nbBlocks = 0;
    char *cwd = malloc(DIRMAX_LEN + 1);

    // Make sure that the mallocs worked properly
    if (dir_buf == NULL || cwd == NULL) {
        printf("Error while mallocing\n");
        return (-1);
    }

    // Fetch the dir we want to delete
    strcpy(pathNameHolder, pathname);
    nDir = parsePath(VCBRef->startDirectory, VCBRef->blockSize, pathNameHolder);

    //cant find - return error
    if(nDir == NULL){
        return -1;
    }

    // Fetch the parent directory
    cwd = fs_getcwd(dir_buf,DIRMAX_LEN);
    nCwd = parsePath(VCBRef->startDirectory, VCBRef->blockSize, cwd);

    //if it has children - return error not empty
    if (findChildren(nDir)) {
        printf("ERROR: Directory has children files\nCannot delete\n");
        return -1;
    }

    //search cwd for dir to delete
    for(int i = 0; i != nCwd[0].nbDir; i++){
       if(strcmp(nCwd[i].name, pathname) == 0){
          nCwd[i].isFree = TRUE;
          strcpy(nCwd[i].name, "");
          found = 1;
       }
    }
    if (found == 0) {
        printf("ERROR: Directory %s not Found within Directory %s\n",pathname,nCwd[0].name);
        return -1;
    }

    //Reset freespace map bit
    nbBlocks = (nDir[0].sizeDirectory/ VCBRef->blockSize) + 1;

    freeSpace(nDir[0].startBlockNb, nbBlocks);

    LBAwrite(nCwd, nbBlocks, nCwd[0].startBlockNb);

    printf("Successfully removed directory\n");
    dir_buf = NULL;
    free (cwd);
    cwd = NULL;
    return 0;
}

