#include <stdio.h>
#define MAX_WORD_LENGTH 30

int main(int argc, char* argv[]) {
  if(argc != 2){				// Check to see if the number of arguments is 2
    return 0;
  }
  FILE *fp = fopen(argv[1],"r");		// Initialize pointer to file and open it to be read
  if (fp == NULL) {				// Check to see if the file exists.
    return 0;
  }
  int numOfWords = 0;				// Create a variable to count the number of words
  char singleWord[MAX_WORD_LENGTH+1];		// Allocate memory to store the word in a character array

  while(fscanf(fp,"%s",singleWord)==1) {	// The while loop checks to see if a word is read. If so, then we increment the 
    numOfWords++;				// numOfWords variable. (If it's not, fscanf will return 0, and the loop will exit)
  }

  printf("%d total words\n",numOfWords);	// We print the final number of words.
}
