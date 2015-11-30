# indexer
Indexer and search program built for Rutgers Computer Science Class, Systems Programming 198:214

## Functionality
Goes through a directory and reads through all files (only simple text formats supported). Finds all words, here identified as any string delimited by white space, and indexes them in order of highest number of occurrences.

Once a directory has been indexed, an interactive search console becomes available, allowing search of single words as well as search of multiple words. With multiple word search, either OR or AND search should be specified, which allows more advanced searching such as searching for files that countain all terms or files that contain any of the terms. With OR multi-term searching, results are ordered first by which files contain more words, and then by the number of occurrences of each of those words. 
