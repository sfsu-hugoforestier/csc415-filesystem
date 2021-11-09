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
#include <unistd.h>
#include "vcb.h"
#include "fsParsePath.h"

char *path  = NULL;

void remove_extra_slash(char * path)
{
    int i = 0;
    int j = 0;
    int len = strlen(path);
    char * temp = (char *)malloc(sizeof(char) * len);
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

// rVCB->startDirectory
st_vcb *getref() {
    return rVCB;
}

//int freeBlockSize;
//uint64_t blockSize;
//uint64_t numberOfBlocks;
//uint64_t signature;
//int indexFreeSpace;
//int startDirectory;

void printvcb(st_vcb *tmp) {
    printf("freeblockSize[%d]\n", tmp->freeBlockSize);
    printf("blockSize[%llu]\n", tmp->blockSize);
    printf("numberOfBlocks[%llu]\n", tmp->numberOfBlocks);
    printf("signature[%llu]\n", tmp->signature);
    printf("indexFreeSpace[%d]\n", tmp->indexFreeSpace);
    printf("startDirectory[%d]\n", tmp->startDirectory);
}
int fs_setcwd(char *buf) {
    printf("test");
    st_vcb *tmp = getref();
    printvcb(tmp);



//    if (path == NULL) {
//        path = malloc(sizeof(char) * (strlen(buf) + 1));
//        strcpy(path, buf);
//    } else {
//        char *temp = malloc(sizeof(char) * (strlen(path) + strlen(buf) + 2));
//        strcpy(temp, path);
//        strcat(temp, "/");
//        remove_extra_slash(temp);
//        strcat(temp, buf);
//        free(path);
//        path = temp;
//    }
//    return 0;
}

char *fs_getcwd(char *buf, size_t size) {
    if (path == NULL) {
        return NULL;
    }
    
    strncpy(buf, path, size);
    return path;
}

int fs_isDir(char * path) {
    int i;
    for(i = 0; i < strlen(path); i++) {
        if(path[i] == '/') {
            return 1;
        }
    }
    return 0;
}


int fs_isFile(char *path) {
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

int main() {
    // printf("%s\n", path);
    fs_setcwd("/");
    // printf("%s\n", path);
//    fs_setcwd("/home");
//    printf("%s\n", path);
//    fs_setcwd("france");
//    fs_setcwd("test");
//    printf("%s\n", path);
//
//
//    char *tmp = fs_getcwd(malloc(sizeof(char) * 100), 100);
//    printf("%s\n", tmp);
    return 0;
}