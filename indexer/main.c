//This should only work on linux systems. Don't try it on a windows machine without removing the sys/types.h include.

#include <sys/types.h>
#include <dirent.h>
#include "tokenizer.h"
#include "binarytree.h"

/*
Enum defined to tell whether a name is the name of a file or a directory
*/
typedef enum {File,Directory} ForD;

/*
Inputs: FILE pointer from_file to be read from, node** root which is the root of the tree to hold the tokens, 
char* filename which is the name of the file from which we are reading
Outputs: int signal
Tokenizes from_file, updates binary tree storing all the tokens and occurrences in files.
Adds the token if it isn't already there.
*/
int fileScan(FILE *from_file, node **root, char *filename) {
	TokenizerT *tk = TKCreate(from_file);
	char *token = TKGetNextToken(tk);
	while(token != NULL) {
		update(root, token, filename);
		token = TKGetNextToken(tk);
	}
	TKDestroy(tk);
	return 0;
}

/*
Inputs: char* read_dir which is the name passed in to be opened, node** root which is the root of the tree to hold the tokens
Outputs: int signal
 * This function tries to open a given name as a directory. Upon success, it enters the directory
 * and recursively calls itself on each name in the directory. Upon failure, it tries to open the given
 * name as a file. If the name opens as a file, it calls fileScan on the file to tokenize its contents. Upon failure of
 * that, the program deems the name unreadable, returns -1, and quits.
 */
int directorySweep(char *read_dir, node **root) {
	//Assumes Directory initially
	ForD type = Directory;
	void *derp = opendir(read_dir);
	struct dirent *direntp;

	if(derp == NULL){
		derp = fopen(read_dir, "r");
		type = File;

		if(derp == NULL) {
			perror(read_dir);
			return -1;
		}
	}
	if(type == Directory) {
		//If name is a directory
		printf("Entered Directory block\n");
		DIR *dir = (DIR *)derp;
		for(;;){
			direntp = readdir(dir);
			if (direntp == NULL) {
				break;
			}
			//to prevent from traveling back up the directory structure
			if(strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0) {
				continue;
			}

			//Dynamically allocates filepath string
			char* next_dir = (char *) malloc(strlen(direntp->d_name) + strlen(read_dir) + 2);
			strcpy(next_dir,read_dir);
			strcat(next_dir,"/");
			strcat(next_dir,direntp->d_name);
			//Recursively calls directorySweep
			directorySweep(next_dir, root);
			printf("%s\n", next_dir);
			free(next_dir);
			//Prints the filename of each file in directory read_dir.
			printf("%s\n", direntp->d_name);
		}
		closedir(dir);
	} else {
		//If name is a file
		printf("Entered file block\n");
		FILE *file = (FILE *)derp;
		fileScan(file, root, read_dir);
		fclose(file);
	}
	return 0;
}

/*
Inputs: int argc, char** argv
Outputs: int signal
 *This function is the main function. It is the function that first runs when the program begins execution.
 *It checks to see if both arguments are present on execution. Upon failure of that, it prints a message and quits.
 *If the file to write to already exists, this function asks the user to overwrite it. If the user says "no", the program quits.
 *If the file to write to does not already exist, this function creates it.
 *This function creates a null node for later use in fileScan.
 *This function passes the 2nd given argument to directorySweep to read it.
 *At the end of execution, this function clears allocated memory and returns 0.
 */
int main (int argc, char **argv){
	//Makes sure program can only be run with the right number of arguments
	if(argc != 3) {
		printf("Wrong number of arguments\n");
		return -1;
	}	
/*	int filedes = open(argv[1], O_RDONLY);
	if(filedes > 0) {
		char* prompt = "The file you want to write to already exists. Do you want to overwrite? Please answer \"yes\"/\"no\"";
		char response[32] = "";

		while(!strcmp(response,"yes") || !strcmp(response,"no")) {
			printf("%s\n",prompt);
			scanf("%s",response);
		}
		if(!strcmp(response,"no")) {
			close(filedes);
			return 0;
		}
	}
	close(filedes);
*/
	//Tries to open the output file for reading to see if there is anything already there
	FILE *overwrite_test = fopen(argv[1], "r");

	if(overwrite_test != NULL) {
		//If so, asks whether to overwrite
		printf("entered test\n");
		char* prompt = "The file you want to write to already exists. Do you want to overwrite? Please answer \"yes\"/\"no\"";
		char response[32] = "";

		while(!(!strcmp(response,"yes")) && !(!strcmp(response,"no"))) {
			printf("%s\n",prompt);
			scanf("%s",response);
		}
		fclose(overwrite_test);
		if(!strcmp(response,"no")) {
			return 0;
		}
	}
	

	FILE *to_file = fopen(argv[1], "w+");
	//Creates root node of tree to store tokens
	node *root = NULL;

	directorySweep(argv[2], &root);
	//writes to file here
	print_inorder(root, to_file);
	fclose(to_file);
	//frees allocated root
	destroy(root);
	
	return 0;
}
