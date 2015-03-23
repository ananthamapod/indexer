/*Include statements*/
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "tknz3r.h"

/*structs*/
typedef struct Node {
	char *data;
	struct Node *next;
} Node;

/*Functions*/
void makeTable(FILE*);
void printWords();
int createEntry();
void cleanup();
int exitFail(char*);

