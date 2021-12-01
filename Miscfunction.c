/**************************************************************
* Class:  CSC-415
* Name: Professor Bierman
* Student ID: N/A
* Project: Basic File System
*
* File: Miscfunction.c
*
* Description:
*	This is the file system interface.
*	This is the interface needed by the driver to interact with
*	your filesystem.
*
**************************************************************/


#include "mfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vcb.h"
//#include "fsInit.h"
#include "fsParsePath.h"
#include "fsUtils.h"

char *MyPath  = NULL;

st_vcb *returnVCBRef();


char *returnPath() {
    return MyPath;
}

char *remove_extra_dir(char *tmp_path) {
    int slash_count = 1;
    int i = strlen(tmp_path);

    for (; i >= 0 ; i--) {
        if (tmp_path[i] == '/' && slash_count == 0) {
            tmp_path[i] = '\0';
            break;
        }
        if (tmp_path[i] == '/')
            slash_count--;
    }


    char *tmp = NULL;

    if (!strcmp(tmp_path, "/..")) {
        tmp = malloc(sizeof(char) + 1);
        if (tmp == NULL) {
            printf("Error while mallocing\n");
            return (NULL);
        }
        strcpy(tmp, "/");
        free(tmp_path);
        return tmp;
    }


    if (tmp_path[0] == '\0') {
        tmp = malloc(sizeof(char) + 1);
        if (tmp == NULL) {
            printf("Error while mallocing\n");
            return (NULL);
        }
        strcpy(tmp, "/");
    } else {
        tmp = malloc(sizeof(char) * strlen(tmp_path));
        if (tmp == NULL) {
            printf("Error while mallocing\n");
            return (NULL);
        }
        strcpy(tmp, tmp_path);
    }
    free(tmp_path);
    return tmp;
}

void remove_extra_dot(char *path) {
    int i = 0;
    int j = 0;
    int len = strlen(path);
    char *temp = malloc(sizeof(char) * len);
    while(path[i] != '\0') {
        if(path[i] == '.') {
            if(i+1 == strlen(path)) {
                i++;
            } else {
                temp[j] = path[i];
                j++;
            }
        } else {
            temp[j] = path[i];
            j++;
        }
        i++;
    }
    temp[j] = '\0';
    strcpy(path, temp);
    free(temp);
}


void remove_extra_slash(char * path)
{
    int i = 0;
    int j = 0;
    int len = strlen(path);
    char *temp = malloc(sizeof(char) * len);
    while(path[i] != '\0') {
        if(path[i] == '/') {
            if(path[i+1] == '/') {
                i++;
            } else {
                temp[j] = path[i];
                j++;
            }
        } else {
            temp[j] = path[i];
            j++;
        }
        i++;
    }
    temp[j] = '\0';
    strcpy(path, temp);
    free(temp);
}

void printdir(struct st_directory* tmp) {
    printf("\n\n");
    printf("nbDir [%d]\n", tmp->nbDir);
    printf("isFree [%d]\n", tmp->isFree);
    printf("startBlockNb [%d]\n", tmp->startBlockNb);
    printf("isDirectory [%d]\n", tmp->isDirectory);
    printf("sizeDirectory [%d]\n", tmp->sizeDirectory);
    printf("creationDate [%ld]\n", tmp->creationDate);
    printf("lastModDate [%ld]\n", tmp->lastModDate);
    printf("name [%s]\n", tmp->name);
    printf("\n\n");
}

void printvcb(st_vcb *tmp) {
    printf("\n\n");
    printf("freeblockSize[%d]\n", tmp->freeBlockSize);
    printf("blockSize[%lu]\n", tmp->blockSize);
    printf("numberOfBlocks[%lu]\n", tmp->numberOfBlocks);
    printf("signature[%lu]\n", tmp->signature);
    printf("indexFreeSpace[%d]\n", tmp->indexFreeSpace);
    printf("startDirectory[%d]\n", tmp->startDirectory);
    printf("\n\n");
}

int fs_setcwd(char *buf) {
    if (MyPath == NULL) {
        //MyPath = malloc(sizeof(char) * strlen("/") + 1);
        MyPath = calloc(2, sizeof(char));
        strcpy(MyPath, "/");
    }

//    printf("\nmy path ==> %s\n", buf);
    st_vcb *tmp = returnVCBRef();
//    printf("[LOG] DEBUG \n");
    if (buf == NULL)
        return (-1);

    char *tmppath = malloc(sizeof(char) * (strlen(buf) + strlen(MyPath) + 2));

    strcpy(tmppath, MyPath);
//    printf(" [BEFORE] tmp path = %s\n", tmppath);
    if (strlen(MyPath) > 1)
        strcat(tmppath, "/");
//    printf(" [BEFORE] tmp path = %s\n", tmppath);
    strcat(tmppath, buf);
//    printf(" [BEFORE] tmp path = %s\n", tmppath);
    if (!strcmp(buf, ".."))
        tmppath = remove_extra_dir(tmppath);


//    printf(" [BEFORE] tmp path = %s\n", tmppath);
    struct st_directory *tmp_dir = parsePath(tmp->startDirectory, tmp->blockSize, tmppath);
    if (tmp_dir == NULL)
        return (-1);

    // TODO add a check of is a dir or not

    if (tmp_dir->isDirectory == 0)
        return (-1);
//    printdir(tmp_dir);

//    printf("\n[LOG] DEBUG \n");
    printDirectory(tmp_dir);
//    printf("\n[LOG] DEBUG \n");

//    printf(" [AFTER] tmp path = %s\n", tmppath);

// TODO do  a FREE of path
    if (strcmp(buf, "."))
        MyPath = tmppath;
//    printf("my path = %s\n", MyPath);
    return 0;
}

char *fs_getcwd(char *buf, size_t size) {
//    printf("MyPath = %s\n", MyPath);
    if (MyPath == NULL) {
        MyPath = malloc(sizeof(char) + 1);
        if (MyPath == NULL) {
            printf("Error while mallocing\n");
            return (NULL);
        }
        strcpy(MyPath, "/");
        strcpy(buf, MyPath);
        return (buf);
    }
//    printf("MyPath = %s\n", MyPath);
    if (buf == NULL) {
        printf("[LOG] getcwd buff is NULL \n");
        return NULL;
    }
    if (size < strlen(MyPath)) {
        printf("[LOG] getcwd buff is too small \n");
        return NULL;
    }
//    if (strlen(MyPath) == 0) {
//        printf("[LOG] getcwd buff is empty \n");
//        return NULL;
//    }
    if (size == 0) {
        printf("[LOG] size is 0 \n");
        return NULL;
    }
//    printf("buf = %s\n", buf);
//    printf("size = %lu\n", size);
    strncpy(buf, MyPath, size);
//    printf("MyPath = %s\n", MyPath);
    return buf;
}


int fs_isDir(char *path) {
    st_vcb *myVCB = returnVCBRef();

    struct st_directory *return_dir = parsePath(myVCB->startDirectory, myVCB->blockSize, path);

    if (return_dir == NULL) {
        printf("[LOG] isDir tmp_dir is NULL \n");
        return (0);
    }

    if (return_dir->isDirectory == 0) {
//        printf("[LOG] isDir return_dir->isDirectory == 0 \n");
        return (0);
    }
    return (1);
}

int fs_isFile(char *path) {
    if (fs_isDir(path) == 1) {
        return (0);
    }
    return (1);
}


int fs_delete(char* filename) {
    //printf("RM_DIR\n");

    //find Dir
    struct st_directory *nDir;
    struct st_directory *nCwd;
    char pathNameHolder[64];
    int found = 0;
    st_vcb *VCBRef = returnVCBRef();
    char * dir_buf = malloc(DIRMAX_LEN + 1);
    unsigned int nbBlocks = 0;
    char *cwd = malloc(DIRMAX_LEN + 1);

    if (dir_buf == NULL || cwd == NULL) {
        printf("Error while mallocing\n");
        return (-1);
    }
    strcpy(pathNameHolder, filename);
    nDir = parsePath(VCBRef->startDirectory, VCBRef->blockSize, pathNameHolder);

    //cant find - return error
    if(nDir == NULL){
        return -1;
    }

    // kill Dir
    // get cwd
    cwd = fs_getcwd(dir_buf, DIRMAX_LEN);
    nCwd = parsePath(VCBRef->startDirectory, VCBRef->blockSize, cwd);

    // search cwd for dir to delete
    for(int i = 0; i != nCwd[0].nbDir; i++){
        if(strcmp(nCwd[i].name, filename) == 0 && && nCwd[i].isDirectory == 0){
            nCwd[i].isFree = TRUE;
            found = 1;
        }
    }
    if(found == 0){
        printf("ERROR: File %s not Found within Directory %s\n",filename,nCwd[0].name);
        return -1;
    }

    // Reset freespace map bit
    nbBlocks = (nDir[0].sizeDirectory/ VCBRef->blockSize) + 1;

    printf("startBlockNb: %i\tnbBlocks: %i\n", nDir[0].startBlockNb, nbBlocks);
    freeSpace(nDir[0].startBlockNb, nbBlocks);

    //free (dir_buf);
    dir_buf = NULL;
    free (cwd);
    cwd = NULL;
    return 0;
}
