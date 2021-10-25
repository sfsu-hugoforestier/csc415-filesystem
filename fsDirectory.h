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

#define BLOCK_SIZE 512
#define directorySize 295
#define numDirectoryEntry 50 

typedef struct dir{
    char name[255];  // The name of the directory -255 Bytes
	int location; // Where we are -4Bytes
	int numFilesInside; // The number of files inside the directory -4Bytes
	int id; //number to track file, use instead of name -4Bytes
	int parentId; //-4Bytes
	time creationDate; //-8 bytes
	time lastModDate; //-8 bytes
	time lastAccess; //-8 bytes
	//int isDirectory
	struct dir *next; // Link to the next directory
} st_dir;

int makeRoot();
int makeDir();
int readDir();
int writeDir();
void closeDir();