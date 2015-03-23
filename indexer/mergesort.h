typedef struct FileNodeT {
  char *fileName;
  int numOccurrences;
  struct FileNodeT *next;
} FileNode;

void FrontBackSplit(FileNode* source,
          FileNode** frontRef, FileNode** backRef);

FileNode* SortedMerge(FileNode* a, FileNode* b);

void MergeSort(FileNode** headRef);
