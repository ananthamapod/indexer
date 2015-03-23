/*Main C file*/

/*include statements*/
#include <stdio.h>
#include "tableManager.h"

/*structs*/

/*prototyping*/
Node *sortedListInsert(Node *, Node *);
Node *compareList(Node *, Node *, int);		//Compares 2 linked lists.
Node *populateList(int);

/*global variables*/

/*Main code*/
//Main Function
int main(int argc, char** argv){
	//printing basic usage instructions
	printf("Use \"sa <word> ... <word>\" to find files that contain all the words in the query.\nUse \"so <word> ... <word>\" to find files that contain any subset of the words in the query.\nUse \"p\" to print all the words in the index.\nUse \"q\" to quit\n");
	//Check arguments.
	if(argc != 2){
		exitFail("Incorrect number of arguments.");
	}

	char *filename = argv[1];
	FILE *file;
	file = fopen(filename,"r");

	//Check if file exists.
	if(file == NULL){
		exitFail("File not found.");
	}

	//Create & populate hash table.
	makeTable(file);
	fclose(file);

	char buffer[100];
	char *token;
	int query = 0;
	while(1){
		printf("\nEnter term to find.\n (search) ");
		fflush(stdout);
		scanf("%[^\n]%*c",buffer);
		if(strcmp(buffer,"q")==0){
			break;
		}

		token = strtok(buffer," ");
		if(strcmp(token,"sa")==0){
			query = 1;
			Node *print = populateList(query);
			Node *temp = print;
			if(print == NULL){
				printf("Nothing to print.\n");
				continue;
			}
			while(print != NULL){
				printf("%s\n",print->data);
				print = print->next;
				free(temp->data);
				free(temp);
				temp = print;
			}
		} else if(strcmp(token,"so")==0){
			query = -1;
			Node *print = populateList(query);
			Node *temp = print;
			if(print == NULL){
				printf("Nothing to print.\n");
				continue;
			}
			while(print != NULL){
				printf("%s\n",print->data);
				print = print->next;
				free(temp->data);
				free(temp);
				temp = print;
			}
		} else if(strcmp(token,"p")==0) {
			printWords();
		} else {
			printf("Incorrect syntax.\n");
		}
	}
	
	cleanup();
	printf("Exiting program\n");
	return 0;
}
Node* populateList(int query) {
	char *token = NULL;
	char files[256];
	Node *termsHead = NULL, *termsCurr = NULL;
	Node *fileHead = NULL;
	token = strtok(NULL," ");
	while(token != NULL) {
		if(termsHead == NULL) {
			termsHead = (Node *)malloc(sizeof(Node));
			if(termsHead == NULL) {
				return NULL;
			}
			termsHead->data = token;
			termsHead->next = NULL;
			termsCurr = termsHead;
		} else {
			Node *newNode = (Node *)malloc(sizeof(Node));
			newNode->data = token;
			newNode->next =NULL;
			termsCurr->next = newNode;
			termsCurr = newNode;
		}
		token = strtok(NULL," ");
	}
	termsCurr = termsHead;

	while(termsCurr != NULL) {
		ENTRY search, *res;
		search.key = termsCurr->data;
		res = hsearch(search, FIND);
		Node *nextHead = NULL;

		if(res == NULL) {
			if(query == 1) {
				return NULL;
			} else if(query == -1) {
				termsCurr = termsCurr->next;
				continue;
			} else {
				return NULL;
			}
			printf("Term not found: %s\n",termsCurr->data);
		} else {
			//Considering the sa case only for now.
			strcpy(files,(char *)res->data);
			if(fileHead == NULL) {
				fileHead = (Node *)malloc(sizeof(Node));
				token = strtok(files," ");
				if(token == NULL) {
					free(fileHead);
					return NULL;
				}
				fileHead->data = (char *)malloc(sizeof(char)*(strlen(token)+1));
				strcpy(fileHead->data,token);
				fileHead->next = NULL;
				token = strtok(NULL," ");
				while(token!=NULL){
					Node* newNode = (Node*)malloc(sizeof(Node));
					newNode->data = (char*)malloc(sizeof(char)*(strlen(token)+1));
					strcpy(newNode->data,token);
					newNode->next = NULL;
					fileHead = sortedListInsert(fileHead,newNode);
					token = strtok(NULL," ");
				}
			} else {
				Node *temp = fileHead;
				//printf("\nHead: ");
				nextHead = (Node *)malloc(sizeof(Node));
				strcpy(files,(char *)res->data);
				token = strtok(files," ");
				if(token == NULL) {
					free(nextHead);
					if(query == 1) {
						while(temp != NULL) {
							Node *prev = temp;
							temp = temp->next;
							free(prev->data);
							free(prev);
						}
						return NULL;
					} else {
						continue;
					}
				}
				while(temp != NULL) {
					//printf("%s->",temp->data);
					temp = temp->next;
				}
				nextHead->data = (char *)malloc(sizeof(char)*(strlen(token)+1));
				strcpy(nextHead->data,token);
				nextHead->next = NULL;
				token = strtok(NULL," ");
				//printf("\nHead:");
				while(token!=NULL){
					Node* newNode = (Node*)malloc(sizeof(Node));
					newNode->data = (char*)malloc(sizeof(char)*(strlen(token)+1));
					strcpy(newNode->data,token);
					newNode->next = NULL;
					nextHead = sortedListInsert(nextHead,newNode);
					token = strtok(NULL," ");
				}
				temp = nextHead;
				while(temp != NULL) {
					//printf("%s->",temp->data);
					temp = temp->next;
				}
				//printf("\n");
				fileHead = compareList(fileHead, nextHead, query);
			}
			if(fileHead == NULL && query == 1) {
				return NULL;
			}
//			printf("%s \n",(char*)res->data);
		}
		termsCurr = termsCurr->next;
	}
	return fileHead;
}

Node* compareList(Node* first, Node* second, int query){		//Compares 2 linked lists.
	Node *fCurr = first;
	Node *sCurr = second;
	Node *ret = NULL, *retCurr = NULL, *remove = NULL;

	while((fCurr != NULL) && (sCurr !=NULL)){
		int res = strcmp(fCurr->data,sCurr->data);
		if(res < 0){		//fCurr data appears before sCurr data; not commmon. Remove it.
			if(query == -1) {
				Node *new = (Node *)malloc(sizeof(Node));
				new->data = (char *)malloc(sizeof(char)*(strlen(fCurr->data)+1));
				new->next = NULL;
				strcpy(new->data,fCurr->data);
				if(ret == NULL){
					ret = new;
					retCurr = ret;
				}else{
					retCurr->next = new;
					retCurr = retCurr->next;
				}
			}
			remove = fCurr;
			fCurr = fCurr->next;
			free(remove->data);
			free(remove);
		} else if(res > 0) {	//sCurr data appears before fCurr data; not common. Remove it.
			if(query == -1) {
				Node *new = (Node *)malloc(sizeof(Node));
				new->data = (char *)malloc(sizeof(char)*(strlen(sCurr->data)+1));
				new->next = NULL;
				strcpy(new->data,sCurr->data);
				if(ret == NULL){
					ret = new;
					retCurr = ret;
				}else{
					retCurr->next = new;
					retCurr = retCurr->next;
				}
			}
			remove = sCurr;
			sCurr = sCurr->next;
			free(remove->data);
			free(remove);
		} else {			//Common data. Keep it.
			Node *new = (Node *)malloc(sizeof(Node));
			new->data = (char *)malloc(sizeof(char)*(strlen(fCurr->data)+1));
			new->next = NULL;
			strcpy(new->data,fCurr->data);
			if(ret == NULL) {
				ret = new;
				retCurr = ret;
			} else {
				retCurr->next = new;
				retCurr = retCurr->next;
			}
			remove = fCurr;
			fCurr = fCurr->next;
			free(remove->data);
			free(remove);
			remove = sCurr;
			sCurr = sCurr->next;
			free(remove->data);
			free(remove);
		}
	}
	while(fCurr != NULL) {
		if(query == -1) {
			Node *new = (Node *)malloc(sizeof(Node));
			new->data = (char *)malloc(sizeof(char)*(strlen(fCurr->data)+1));
			new->next = NULL;
			strcpy(new->data,fCurr->data);
			if(ret == NULL){
				ret = new;
				retCurr = ret;
			}else{
				retCurr->next = new;
				retCurr = retCurr->next;
			}
		}
		remove = fCurr;
		fCurr = fCurr->next;
		free(remove->data);
		free(remove);
	}
	while(sCurr != NULL) {
		if(query == -1) {
			Node *new = (Node *)malloc(sizeof(Node));
			new->data = (char *)malloc(sizeof(char)*(strlen(sCurr->data)+1));
			new->next = NULL;
			strcpy(new->data,sCurr->data);
			if(ret == NULL){
				ret = new;
				retCurr = ret;
			}else{
				retCurr->next = new;
				retCurr = retCurr->next;
			}
		}
		remove = sCurr;
		sCurr = sCurr->next;
		free(remove->data);
		free(remove);
	}

	return ret;
}

Node *sortedListInsert(Node *listHead, Node *newNode) {
	Node *curr = listHead, *prev = NULL;
	if(listHead == NULL) {
		return newNode;
	}
	if(newNode == NULL) {
		return listHead;
	}
	while(curr != NULL && (strcmp(curr->data,newNode->data) < 0)) {
		prev = curr;
		curr = curr->next;
	}
	if(prev == NULL) {
		newNode->next = listHead;
		listHead = newNode;
		return listHead;
	}
	newNode->next = prev->next;
	prev->next = newNode;
	return listHead;
}
