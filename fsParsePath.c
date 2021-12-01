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
#include "mfs.h"

#define DIRMAX_LEN 4096

// Fetch the root dir by reading from the volume
struct st_directory *getDir(int startDirectory, int blockSize, struct st_directory *rootDir) {
    unsigned int sizeMallocDir = sizeof(struct st_directory) * DIRECTORY_ENTRIES;
    unsigned int nbBlocks = (sizeMallocDir / blockSize) + 1;
    unsigned int maxSize = nbBlocks * blockSize;
    unsigned int end = 0;
    int nbDir = DIRECTORY_ENTRIES;

    while (sizeMallocDir < maxSize && end != 1) {
        if (sizeMallocDir + sizeof(struct st_directory) < maxSize) {
            sizeMallocDir+=(sizeof(struct st_directory));
            nbDir++;
        } else {
            end = 1;
        }
    }
    rootDir = calloc(nbDir, sizeMallocDir);
    uint64_t rvRead = LBAread(rootDir, nbBlocks, startDirectory);
    if (rvRead != nbBlocks) {
        printf("Error while reading\n");
        return (NULL);
    }
    return (rootDir);
}

// Count the number of character given as argument
int getNbChar(char *string, char delim) {
    int count = 0;

    for (int i = 0; i != strlen(string); i++) {
        if (string[i] == delim)
            count++;
    }
    return (count);
}

struct st_directory *findDirectory(struct st_directory *nDir, char *path, int blockSize) {
    char *token = NULL;
    char *pToken = path;
    unsigned int end = 0;
    unsigned int nbLooped = 0;
    unsigned int nbDelim = getNbChar(path, '/');

    // If we only need to search for one directory
    if (path[0] != '/' && nbDelim == 0) {
        for (int i = 0; i != nDir[0].nbDir && end != 1; i++) {
            if (strcmp(nDir[i].name, path) == 0) {
                nDir = getDir(nDir[i].startBlockNb, blockSize, nDir);
                return (nDir);
            }
        }
        printf("Could not find %s\n", path);
        return (NULL);
    }
    // Loop through all the directory entries, when we find the one we
    // are looking for we go inside of it and keep going until we reach
    // the directory we want
    while ((token = strtok_r(pToken, "/", &pToken))) {
        for (int i = 0; i != nDir[0].nbDir && end != 1; i++) {
            if (strcmp(nDir[i].name, token) == 0 && nDir[i].isFree != TRUE) {
                nDir = getDir(nDir[i].startBlockNb, blockSize, nDir);
                end = 1;
            }
        }
        nbLooped++;
        if (end == 0 && (nbLooped - 1) != nbDelim) {
            printf("Error could not find %s\n", token);
            return (NULL);
        }
        end = 0;
    }
    return (nDir);
}

char *shiftPath(char *path) {
    for (int i = 0; i != strlen(path) - 1; i++) {
        path[i] = path[i + 1];
    }
    return (path);
}

struct st_directory *parsePath(int startDirectory, int blockSize, char *path) {
    struct st_directory *nDir = NULL;
    char *dir_buf = malloc(DIRMAX_LEN + 1);

    // Make sure that the malloc worked properly
    if (dir_buf == NULL) {
        printf("Error while mallocing dir_buf\n");
        return (NULL);
    }
    // Ignore if the path starts with a '.'
    if (path[0] == '.')
        path = shiftPath(path);
    dir_buf = fs_getcwd(dir_buf, DIRMAX_LEN);
    // Fetch the root directory of the filesystem, in order to start searching
    nDir = getDir(startDirectory, blockSize, nDir);
    // If absolute path we directly search for the path
    if (path[0] == '/')
        return (findDirectory(nDir, path, blockSize));
    // Fetch the directory struct for the current working directory
    // so that we can start searching for the path, start at the right place
    nDir = findDirectory(nDir, dir_buf, blockSize);
    if (nDir == NULL) {
        printf("Error when fetching CWD\n");
        return (NULL);
    }
    // Search for the directory with the given path
    nDir = findDirectory(nDir, path, blockSize);
    return (nDir);
}
