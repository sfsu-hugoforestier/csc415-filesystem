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

#include <time.h>
#include <stdbool.h>

typedef struct dir{
    char name[255];  // The name of the directory
	int location; // Where we are
	int numFilesInside; // The number of files inside the directory
	int id; //number to track file, use instead of name
	int parentId;
	time creationDate; 
	time lastModDate;
	time lastAccess;
	int isDirectory
	struct dir *next; // Link to the next directory
} st_dir;

makeRoot(){
     
}

makeDir();
readDir();
writeDir();
closeDir();
