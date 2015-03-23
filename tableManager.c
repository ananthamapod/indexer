/*Hashing/File reading C file*/

/*Include statements*/
#include "tableManager.h"

/*defines*/
#define TABLE_SIZE 4096
#define DEFAULT_DATA_SIZE 1024

/*global variables*/
TokenizerT *tk;
Node *tokenHead, *tokenCurr;

/*Main code*/
void makeTable(FILE *file){				//creates and populates a hash table.
	//Create table
	int res = hcreate(TABLE_SIZE);
	if(res == 0)
		exitFail("Could not create hashtable.");	//Failure.
	//Create tokenizer
	tk = TKCreate(file);
	int cont;
	do{
		cont = createEntry();
	}while(cont!=0);					//While loop populates table with ENTRY items made from index file.
	return;
}

//print out all of the searchable words
void printWords() {
	Node *temp = tokenHead;
	int count = 0;
	while(temp != NULL) {
		if(count == 5) {
			printf("\n");
			count = 0;
		}
		printf("%s\t\t",temp->data);
		count++;
		temp = temp->next;
	}
	printf("\n");
}

//Our problem child.
int createEntry(){					//Creates entries for the hashtable.
	//Create entry for table and associated strings.
	ENTRY newEnt, *temp;
	char* token, *hold;
	char* dataStr = (char*) malloc(DEFAULT_DATA_SIZE);
	if(dataStr == NULL) {
		cleanup();
		exitFail("malloc failed.");
	}

	//Get key token.
	token = TKGetNextToken(tk);
	if(token == NULL)
		return 0;					//No more tokens if TKGetNextToken return NULL => no more entries to add. Break loop in  calling function.
	if(strcmp(token, "<list>") == 0){
		hold = TKGetNextToken(tk);
		newEnt.key = (char *) malloc(strlen(hold)+1);	//Set ENTRY key to word token
		Node *newNode = (Node *) malloc(sizeof(Node));
		newNode->data = (char *) malloc(strlen(hold)+1);
		newNode->next = NULL;
		strcpy(newEnt.key,hold);
		strcpy(newNode->data,hold);
		if(tokenHead == NULL) {
			tokenHead = newNode;
			tokenCurr = newNode;
		} else {
			tokenCurr->next = newNode;
			tokenCurr = newNode;
		}
	}
	//Initialize data string.
	strcpy (dataStr, " ");	

	//Loop through tokens & add file names to data list.
	for(token = TKGetNextToken(tk); strcmp(token,"<\\list>")!=0; token = TKGetNextToken(tk)){
		if(strtol(token,NULL,10) != 0L) {		//Discard occurrence frequencey numbers.
			continue;
		}
		/*Be sure to check size of current data string and string to add.
		 *Catch for total string bigger than allocated space.
		 */
		strcat (dataStr, token);			//Add filename to data string.
		strcat (dataStr, " "); 				//Use spaces to separate file names.
	}

	//Add data string to ENTRY struct and place in table.
	newEnt.data = (void*) dataStr;
	temp = hsearch(newEnt, ENTER);
	if(temp == NULL)
		exitFail("Error in adding entry to table.");

	return 1;
}

void cleanup(){
	ENTRY *temp = NULL;
	Node *prev = NULL;
	TKDestroy(tk);
	tokenCurr = tokenHead;
	while(tokenCurr != NULL) {
		prev = tokenCurr;
		ENTRY blah;
		blah.key = prev->data;
		tokenCurr = tokenCurr->next;
		temp = hsearch(blah,FIND);
		if(temp != NULL) {
			free(temp->key);
			free(temp->data);
		}
		free(prev->data);
		free(prev);
	}

	hdestroy();
	return;
}

int exitFail(char* message){				//Failure message printer. Quits with EXIT_FAILURE.	
	fprintf(stderr, "%s Exiting program.\n",message);
	exit(EXIT_FAILURE);
	return 0;
}
