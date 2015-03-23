#include <stdio.h>

typedef struct BSTNodeT {
	char *token;
	int numOccurrences;
	struct Node *left, *right;
} node;

typedef struct FileNodeT{
	char *fileName;
	int numOccurrences;
	struct FileNodeT *next;
} FileNode;

node **update(node **tree, char *token, char *filename);

void print_inorder(node *tree, FILE *to_file);

void destroy(node *tree);
