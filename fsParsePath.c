/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: parsePath.h
*
* Description:
* File with the function declaration of parsePath.c
*
**************************************************************/

#include "fsParsePath.h"
#include "fsDirectory.h"
#include "fsUtils.h"

struct st_directory *getDir(int startDirectory, int blockSize, struct st_directory *rootDir) {
    unsigned int sizeMallocDir = sizeof(struct st_directory) * DIRECTORY_ENTRIES;
    unsigned int nbBlocks = (sizeMallocDir / blockSize) + 1;
    unsigned int maxSize = nbBlocks * blockSize;
    unsigned int end = 0;

    while (sizeMallocDir < maxSize && end != 1) {
        if (sizeMallocDir + sizeof(struct st_directory) < maxSize) {
            sizeMallocDir+=(sizeof(struct st_directory));
        } else {
            end = 1;
        }
    }

    rootDir = malloc(sizeMallocDir);

    uint64_t rvRead = LBAread(rootDir, nbBlocks, startDirectory);
    return (rootDir);
}

int getNbChar(char *string, char delim) {
    int count = 0;

    for (int i = 0; i != strlen(string); i++) {
        if (string[i] == delim)
            count++;
    }
    return (count);
}

// Ex to call parsePath
//char string[50] = "/Hello/world/";
//parsePath(sVCB->startDirectory, blockSize, string);

struct st_directory *parsePath(int startDirectory, int blockSize, char *path) {
    struct st_directory *nDir = NULL;
    char *token = NULL;
    char *pToken = path;
    unsigned int end = 0;
    unsigned int nbLooped = 0;
    unsigned int nbDelim = getNbChar(path, '/');
    nDir = getDir(startDirectory, blockSize, nDir);

    while ((token = strtok_r(pToken, "/", &pToken)) && token != NULL) {
        for (int i = 0; i != nDir[0].nbDir && end != 1; i++) {
            if (strcmp(nDir[i].name, token) == 0) {
                nDir = getDir(nDir[i].startBlockNb, blockSize, nDir);
                end = 1;
            }
        }
        nbLooped++;
        if (end == 0 && nbLooped != nbDelim) {
            printf("Error could not find %s\n", token);
            return (nDir);
        }
        end = 0;
    }
    return (nDir);
}
