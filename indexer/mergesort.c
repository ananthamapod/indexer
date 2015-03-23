/*Slightly modified from code at http://www.geeksforgeeks.org/merge-sort-for-linked-list/ 
* Although we did not write this entirely ourselves, we felt that it made more sense not to waste time 
* reinventing the wheel to do the mergesort, a sorting algorithm that we both have easy familiarity with,
* just because we were doing it in a different language (C). We did however make sure to give credit where it
* was due. Have a nice day!
* -Ananth
*
*I LYK 2 STEELE DA CODEZ!!111!!!
* -IMRAN
*/

#define NULL 0
//because why the hell not?

//Also because NULL is only defined in stdlib.h and we liked the idea of having absolutely no includes in a C file for once



typedef struct FileNodeT {
  char *fileName;
  int numOccurrences;
  struct FileNodeT *next;
} FileNode;


/* UTILITY FUNCTION */
/* Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  */

void FrontBackSplit(FileNode* source,
          FileNode** frontRef, FileNode** backRef)
{
  FileNode* fast;
  FileNode* slow;
  if (source== NULL || source->next== NULL)
  {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
        slow = slow->next;
        fast = fast->next;
      }
    }
 
    /* 'slow' is before the midpoint in the list, so split it in two
      at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}

FileNode* SortedMerge(FileNode* a, FileNode* b)
{
  FileNode* result = NULL;
 
  /* Base cases */
  if (a == NULL)
     return(b);
  else if (b == NULL)
     return(a);
 
  /* Pick either a or b, and recur */
  if (a->numOccurrences >= b->numOccurrences)
  {
     result = a;
     result->next = SortedMerge(a->next, b);
  }
  else
  {
     result = b;
     result->next = SortedMerge(a, b->next);
  }
  return(result);
}

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(FileNode** headRef)
{
  FileNode* head = *headRef;
  FileNode* a;
  FileNode* b;
 
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }
 
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b);
 
  /* Recursively sort the sublists */
  MergeSort(&a);
  MergeSort(&b);
 
  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}
