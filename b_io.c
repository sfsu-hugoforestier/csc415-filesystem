/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names:
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: b_io.c
*
* Description: Basic File System - Key File I/O Operations
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "b_io.h"
#include "fsDirectory.h"
#include "vcb.h"
#include "fsDirectory.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb {
    /** TODO add al the information you need in the file control block **/
    st_directory *dir; //holding dirInfo for b_FCB
    char * buf;		//holds the open file buffer
    char * cwdSave; //holds our cwd
    int index;		//holds the current position in the buffer
    int buflen;		//holds how many valid bytes are in the buffer
    int curBlock;
} b_fcb;

b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init () {
    printf("in b_init\n");
    //init fcbArray to all free
    for (int i = 0; i < MAXFCBS; i++) {
        fcbArray[i].buf = NULL; //indicates a free fcbArray
    }
    startup = 1;
}

//Method to get a free FCB element
b_io_fd b_getFCB () {
    for (int i = 0; i < MAXFCBS; i++) {
        if (fcbArray[i].buf == NULL) {
            return i;		//Not thread safe (But do not worry about it for this assignment)
        }
    }
    return (-1);  //all in use
}

// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags) {
    b_io_fd returnFd;
    st_vcb *VCBRef = returnVCBRef();
    //*** TODO ***:  Modify to save or set any information needed
    //                 
    //

    printf("in b_open\n");
    if (startup == 0)
        b_init();  //Initialize our system
    
    b_io_fd fd = b_getFCB();				// get our own file descriptor
    // check for error - all used FCB's
    b_fcb* fcb = &fcbArray[i]; //do you want the return? changed from i
    
    /*
    fcb->cwdSave = fs_getcwd();
    fs_setcwd(filename);
    */
    //If file name doesnt exist, make one routine to make here:
    fcb->dir = parsePath(VCBRef->startDirectory, VCBRef->blockSize, filename);
    //Structure has a result on if the dir exists. Likey not to start
    if(fcb->dir == NULL)
    {   
        printf("Didn't find filename\n");
        fcb->cwdSave = fs_getcwd(); //grab from nearest dir from failed find
    } else {

    }
    
    fcb->dir; // equals dir reference of filename (GetFileInfo(filename))
    fcb->buf; //equals dir of filename
    fcb->curBlock; //equals to fileName starting block in Dir
    fcb->index = 0; 
    fcb->buflen = 0; 
    
    return (fd);						// all set
	}


// Interface to seek function
int b_seek (b_io_fd fd, off_t offset, int whence) {
    if (startup == 0)
        b_init();  //Initialize our system

    // check that fd is between 0 and (MAXFCBS-1)
    if ((fd < 0) || (fd >= MAXFCBS)) {
        return (-1); 					//invalid file descriptor
    }

    printf("in seek\n");
    return (0); //Change this
}

// Interface to write function
int b_write (b_io_fd fd, char * buffer, int count) {
    int writeSize = 0;
    int writeRest = 0;
    int spaceLeft = 0;
    int newBlockResult = 0;
    bool enoughSize = TRUE;
    st_vcb *VCBRef = returnVCBRef();
    b_fcb* fcb = &fcbArray[fd];

    //printf("In b_write\n");
    if (startup == 0)
        b_init();  //Initialize our system

    // check that fd is between 0 and (MAXFCBS-1)
    if ((fd < 0) || (fd >= MAXFCBS)) {
        return (-1); 					//invalid file descriptor
    }

    if (fcbArray[fd].dir == NULL)		//File not open for this descriptor
	{
		printf("ERROR: File not open for this descriptor\n");	
		return (-1);
	}

	spaceLeft = VCBRef->blockSize - fcb->index;

    if(count > spaceLeft){
        writeSize = spaceLeft;
        writeRest = count - spaceLeft;
        enoughSize = false;
    }
    else{
        writeSize = count;
        enoughSize = true;
    }

    //write to block
    memcpy(fcb->index + fcb->buf, buffer, writeSize);
	fcb->index += writeSize;
	fcb->index %= VCBRef->blockSize;

    LBAWrite(fcb, buffer, writeSize);

    if(enoughSize == false){
        //Move to next block
        newBlockResult = getFreeSpace(VCBRef,1,VCBRef->blockSize,VCBRef->numberOfBlocks);
        if(newBlockResult == -1){
            printf("ERROR: We had trouble finding a new free block to write to!");
            return -1;
        }
        
        //reset the index
        fcb->index = 0;

        //writes to block
        memcpy(fcb->index + fcb->buf, buffer + copyLength, secondCopyLength);

		fcb->index += secondCopyLength;
		fcb->index %= VCBRef->blockSize;
        LBAWrite(fcb, buffer, writeRest);
        
        return writeSize + writeRest;
    }

    return writeSize; 
}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count) {
    if (startup == 0)
        b_init();  //Initialize our system

    // check that fd is between 0 and (MAXFCBS-1)
    if ((fd < 0) || (fd >= MAXFCBS)) {
        return (-1); 					//invalid file descriptor
    }

    struct b_fcb* fcb = &fcbArray[fd];
    int sizeLeft =fcb->buflen-fcb->index;


    if(count < sizeLeft) {
        memcpy(buffer, fcb->buf + fcb->index, count);
        fcb->index += count;
        return count;
    }
    else {
        // sizeLeft less than the size of buffer
        return (sizeLeft);	
    }
    
}

// Interface to Close the file
void b_close (b_io_fd fd) {
    free(fcbArray[fd].buff);
	fcbArray[fd].buff = NULL;
}
