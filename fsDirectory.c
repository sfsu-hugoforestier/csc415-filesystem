/**************************************************************
* Class:  CSC-415-0# - Fall 2021
* Names:
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: 
*
* Description:
*
**************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define BLOCK_SIZE 512 

typedef struct dir{
    char name[255];  // The name of the directory -255 Bytes
	int location; // Where we are -4Bytes
	int numFilesInside; // The number of files inside the directory -4Bytes
	int id; //number to track file, use instead of name -4Bytes
	int parentId; //-4Bytes
	time creationDate; //-3 bytes
	time lastModDate; //-3 bytes
	time lastAccess; //-3 bytes
	//int isDirectory
	struct dir *next; // Link to the next directory
} st_dir;
//280 bytes total

makeRoot(){
    st_dir newRoot = malloc(50);
    int totalSizeNeeded = 280 * 50;
    int totalBlocks = totalSizeNeeded/BLOCK_SIZE; 

    newRoot->name[255] = ".";
    newRoot->id = 1;

    newRoot->parentId = 0;


    newRoot->location;
    newRoot->numFilesInside;


}

makeDir();
readDir();
writeDir();
closeDir();
