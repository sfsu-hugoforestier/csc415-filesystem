#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fsLow.h"
#include "mfs.h"
#include "vcb.h"
#include "fsFree.h"
#include "fsInit.c"

//Use this file later to define the dir functions

/*
void initDir(){
    st_vcb VCBCopy = returnVCB();
    printf("\nVCB - %d\n",VCBCopy->numberOfBlocks);

}*/