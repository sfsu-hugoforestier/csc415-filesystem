/**************************************************************
* Class:  CSC-415
* Name: FORESTIER Hugo
* Student ID: 922296112
* Project: Basic File System
*
* File: vcb.h
*
* Description:
* File with the vcb structure
*
**************************************************************/
#ifndef VCB_H_
#define VCB_H_

#include <stdint.h>
#include <stddef.h>

typedef struct vcb {
    int freeBlockSize;
    uint64_t blockSize;
    uint64_t numberOfBlocks;
    uint64_t signature;
    int indexFreeSpace;
    int startDirectory;
} st_vcb;

st_vcb *returnVCBRef();

#endif
