#include "fsDirectory.h"
#include <stdio.h>
#include <string.h>
#include "fsLow.h"
#include "vcb.h"

#include "mfs.h"
#include "fsParsePath.h"

#define DIRMAX_LEN		4096

int sDirIndex = 1;

struct st_directory *initDefaultDir(struct st_directory *sDir, int iBlock, int sizeMallocDir, int nbDir) {
    time_t t = time(NULL);

    sDir[0].nbDir = nbDir;
    sDir[0].isFree = FALSE;
    sDir[0].startBlockNb = iBlock;
    sDir[0].creationDate = time(&t);
    sDir[0].lastModDate = time(&t);
    sDir[0].id = 1;
    sDir[0].parentId = 0;
    strcpy(sDir[0].name, ".");
    sDir[0].isDirectory = TRUE;
    sDir[0].sizeDirectory = sizeMallocDir;

    sDir[1].nbDir = nbDir;
    sDir[1].isFree = FALSE;
    sDir[1].startBlockNb = iBlock;
    sDir[1].creationDate = time(&t);
    sDir[1].lastModDate = time(&t);
    sDir[1].id = 0;
    sDir[1].parentId = -1;
    strcpy(sDir[1].name, "..");
    sDir[1].isDirectory = TRUE;
    sDir[1].sizeDirectory = sizeMallocDir;
    return (sDir);
}

int initializeDirectories(st_vcb*rVCB, int blockSize, int numberOfBlocks) {
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
    sDir = malloc(sizeMallocDir);
    if (sDir == NULL) {
        printf("Error while allocating the memory for sDir\n");
        return (-1);
    }
    for (int i = 0; i != nbDir; i++)
        sDir[i].isFree = TRUE;
    iBlock = getFreeSpace(rVCB, nbBlocks, blockSize, numberOfBlocks);
    sDir = initDefaultDir(sDir, iBlock, sizeMallocDir, nbDir);
    LBAwrite(sDir, nbBlocks, iBlock);
    return (iBlock);
}

int generateID(){
    return 0;
}

struct st_directory *initializeNewDir(struct st_directory *nDir, int iBlock, struct st_directory *cwdDir){
    time_t t = time(NULL);

    nDir[0].nbDir = nDir[0].nbDir;
    nDir[0].isFree = FALSE;
    nDir[0].startBlockNb = iBlock;
    nDir[0].creationDate = time(&t);
    nDir[0].lastModDate = time(&t);
    nDir[0].id = generateID();
    nDir[0].parentId = cwdDir->id;
    strcpy(nDir[0].name, ".");
    nDir[0].isDirectory = TRUE;
    nDir[0].sizeDirectory = nDir[0].sizeDirectory;

    nDir[1].nbDir = nDir[0].nbDir;
    nDir[1].isFree = FALSE;
    nDir[1].startBlockNb = cwdDir[0].startBlockNb;
    nDir[1].creationDate = time(&t);
    nDir[1].lastModDate = time(&t);
    nDir[1].id = generateID();
    nDir[1].parentId = cwdDir->id;
    strcpy(nDir[1].name, "..");
    nDir[1].isDirectory = TRUE;
    nDir[1].sizeDirectory = nDir[0].sizeDirectory;

    return nDir;
}

int createDir(struct st_directory *cwdDir, int index, const char *pathname){

    //create new dir
    struct st_directory *nDir;
    nDir = malloc(cwdDir[0].sizeDirectory);
    st_vcb *VCBRef = returnVCBRef();
    int iBlock = 0;
    unsigned int nbBlocks = (cwdDir[0].sizeDirectory/ VCBRef->blockSize) + 1;

    if (nDir == NULL){
        printf("Error while allocating the memory for sDir\n");
        return (-1);
    }

    for (int i = 0; i != cwdDir[0].nbDir; i++)
        nDir[i].isFree = TRUE;

    iBlock = getFreeSpace(VCBRef, nbBlocks, VCBRef->blockSize, VCBRef->numberOfBlocks);
    
    nDir = initializeNewDir(nDir, iBlock, cwdDir);

    //cwd - add the dir entry with value of location
    time_t t = time(NULL);

    cwdDir[index].nbDir = cwdDir[0].nbDir;
    cwdDir[index].isFree = FALSE;
    cwdDir[index].startBlockNb = nDir[0].startBlockNb;
    cwdDir[index].creationDate = time(&t);
    cwdDir[index].lastModDate = time(&t);
    cwdDir[index].id = index;
    cwdDir[index].parentId = 0; //wrong
    strcpy(cwdDir[index].name, pathname);
    cwdDir[index].isDirectory = TRUE;
    cwdDir[index].sizeDirectory = cwdDir[0].sizeDirectory;

    //LBAWrite - both cwd and new dir
    LBAwrite(nDir, nbBlocks, iBlock);
    LBAwrite(cwdDir, nbBlocks, cwdDir[0].startBlockNb);

    return 0;
}

char *fs_getcwd(char *buf,size_t length){
    strcpy(buf,"/");

    return buf;
}

int fs_mkdir(const char *pathname, mode_t mode){
    struct st_directory *nDir;
    char pathNameHolder[64];
    int result = 0;

    strcpy(pathNameHolder, pathname);

    char * dir_buf = malloc (DIRMAX_LEN +1);
    char *cwd = malloc(DIRMAX_LEN +1);
    if(cwd == NULL){
        printf("MALLOC ERROR\n");
        return -1;
    }
    cwd = fs_getcwd(dir_buf,DIRMAX_LEN);
    
    nDir = parsePath(returnVCBRef()->startDirectory, 512, cwd);

    for(int i = 0; i != nDir[0].nbDir; i++){
       if(strcmp(nDir[i].name, pathname) == 0){
           printf("ERROR: FILE already created\n");
           free (cwd);
           cwd = NULL;
           return -1;
       }
    }
    
    //No child - create directory
    for(int i = 0; i != nDir[0].nbDir; i++){
        if(nDir[i].isFree == TRUE){
            result = createDir(nDir, i, pathname);
            if(result < 0){
                free (cwd);
                cwd = NULL;
                return -1;
            }
            break;
        }  
    }
    free (dir_buf);
	dir_buf = NULL;
    free (cwd);
	cwd = NULL;
    return 0;
}


int fs_rmdir(const char *pathname){
    
}