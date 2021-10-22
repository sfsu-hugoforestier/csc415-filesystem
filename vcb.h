#ifndef VCB_H_
#define VCB_H_

#include <stdint.h>

typedef struct vcb {
    int freeBlockSize;
    uint64_t blockSize;
    uint64_t numberOfBlocks;
    uint64_t signature;
    struct st_block *next;
} st_vcb;

#endif
