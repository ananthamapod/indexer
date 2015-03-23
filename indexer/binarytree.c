#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.h"

typedef struct BSTNodeT {
	char *token;
	FileNode *occurrences;
	struct BSTNodeT *left, *right;
} node;

/*
Inputs: node** tree which is the root of tree/subtree, char* filename which is the name of the file in which the token occurs
Outputs: node** tree which is the root of the tree/subtree
 *This function updates a binary tree by adding nodes to it and the linked lists in the tree nodes.
 *If the tree does not yet exist, it creates the tree.
 */
node **update(node **tree, char *val, char *filename) {
	node *temp = NULL;
	
	//Tree not yet created.
	if(!(*tree)) {
		temp = (node *)malloc(sizeof(node));
		temp->left = NULL, temp->right = NULL;
		temp->token = val;

		FileNode *newData = (FileNode *)malloc(sizeof(FileNode));
		newData->fileName = (char *)malloc(sizeof(char)*strlen(filename)+1);
		strcpy(newData->fileName, filename);
		newData->numOccurrences = 1;
		newData->next = NULL;

		temp->occurrences = newData;
		*tree = temp;
	}else {
		//New entry is (alphanumerically) lower than current node. Move recursively left.
		if(strcmp(val,(*tree)->token) < 0) {
			(*tree)->left = *(update(&((*tree)->left),val, filename));
		//New entry is (alphanumerically) higher than current node.	Move recursively right.
		} else if(strcmp(val,(*tree)->token) > 0) {
			(*tree)->right = *(update(&((*tree)->right),val, filename));
		//New entry matches an already present, update it.
		} else {
			//Entry has been previously encountered in current file.
			if(strcmp((*tree)->occurrences->fileName,filename) == 0) {
				(*tree)->occurrences->numOccurrences += 1;
			//Entry newly encountered in current file.
			} else {
				FileNode *newData = (FileNode *)malloc(sizeof(FileNode));
				newData->fileName = (char *)malloc(sizeof(char)*strlen(filename)+1);
				strcpy(newData->fileName, filename);
				newData->numOccurrences = 1;
				newData->next = (*tree)->occurrences;
				(*tree)->occurrences = newData;
	
			}
		}
	}
	return tree;
}
/*
Inputs:node* tree which is the root of tree/subtree, FILE* to_file which is the file to write to
Outputs: None
 *An inorder traversal of the binary tree that prints to file.
 *Sorts the lists in each node for printing.
 */
void print_inorder(node *tree, FILE *to_file) {
	int count = 1;
	if(tree) {
		//First to the left
		print_inorder(tree->left, to_file);

		//Then current

		MergeSort(&(tree->occurrences));

		fprintf(to_file, "<list> %s\n", tree->token);
		FileNode *occurrence = NULL;
		for(occurrence = tree->occurrences; occurrence != NULL; occurrence = occurrence->next) {
			fprintf(to_file, "\"%s\" %d ", occurrence->fileName, occurrence->numOccurrences);
			if(count%5 == 0) {
				fprintf(to_file,"\n");
			}
			count++;
		}
		
		fprintf(to_file,"\n<\\list>\n");

		//Then to the right
		print_inorder(tree->right, to_file);
	}	
}

/*This function frees all memory associated with the binary tree and all its elements and all their elements.
 *
 */
void destroy(node *tree) {
	if(tree == NULL) {
		return;
	}
	destroy(tree->left);
	destroy(tree->right);

	FileNode *curr = tree->occurrences;
	FileNode *prev = curr;
	while(curr != NULL) {
		curr = curr->next;
		free(prev->fileName);
		free(prev);
		prev = curr;
	}
	free(tree->token);
}
