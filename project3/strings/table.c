#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#define EMPTY 0
#define FILLED 1
#define DELETED 2

/*
 * This file defines the struct called set and implements its 
 * different functions including creating and destroying the set, adding and removing elements, 
 * and searching (details are commented above each function).
 * @author: Jonathan Trinh
 * @version: 5022017
 */
struct set
{
	int count;	/*number of elements */
	int length; /*length of array */
	char **data; /*array of strings */
	char* flags; /*array of flags */
};
typedef struct set SET;

static int search(SET *sp, char *elt, bool *found);		// prototyping the search function

/**
 * Function: strhash
 * -------------------------------
 * Summary: This function assigned a unique (hopefully) hash value for each word, by running through each 
 * character in the word and multiplying its value by 31 and adding those values up.
 * Runtime: O(1);
 */
unsigned strhash(char *s) {
	unsigned hash = 0;
	while (*s != 0) {
		hash = 31 * hash + *s ++;
	}
	return hash;
}

/*
 * Function: *createSet
 * -------------------------------
 * Summary: This function allocates memory and creates a set that keeps track of the number of elements,
 * the length of the array, and the array itself (called "data"). In addition, we create an additional
 * array called flags and initialize each of its elements to EMPTY.
 * Runtime: O(n)
 */
SET *createSet(int maxElts){
	SET *sp;

	sp=malloc(sizeof(SET));						// declare set and allocate memory
	assert(sp!=NULL);

	sp->count = 0;								// this count element keeps track of the number of elements currently in the set
	sp->length=maxElts;							// this is the length of the array
	sp->data = malloc(sizeof(char*)*maxElts);	// this is an character array where the actual data is stored
	sp->flags = malloc(sizeof(char)*maxElts);
	int i;
	for (i = 0; i < maxElts; i++) {				// run through array and initialize each element to EMPTY
		sp->flags[i] = EMPTY;
	}
	assert(sp->data!=NULL);
	return sp;
}

/*
 * Function: destroySet
 * --------------------------------
 * Summary: Destroys the set by freeing all the memory starting with the members of the character array, 
 * the character array itself, the flags array, and then the set
 * Runtime: O(n)
 */
void destroySet(SET *sp){
	int i;

	// we first free memory of the elements themselves by checking to see whether the element in the corresponding flags array is FILLED
	for (i=0;i<sp->length;i++){	
		if (sp->flags[i]==FILLED){
			free(sp->data[i]);
		}
	}
	free(sp->flags);					// free the flags array
	free(sp->data);						// we then free the data array
	free(sp);							// finally we free the set
}

/*
 * Function: numElements
 * ----------------------------------
 * Summary: Simply returns the number of elements in the set
 * Runtime: O(1)
 */
int numElements(SET *sp){
	assert(sp!=NULL);
	return sp->count;
}

/*
 * Function: search
 * ------------------------------------
 * Summary: This search function performs a search using linear probing. It starts by assigning a starting hash value and keeps
 * track of a deleted location. We then loop through the array checking to see if the locations are FILLED, DELETED, or EMPTY.
 * If FILLED, then we have found the element and simply return its index. If DELETED, then we remember the first deleted location.
 * Otherwise, if it is not the above options,it must be EMPTY, and we have not found the element. If we have not seen a deleted location
 * previously, we return the empty index all the way at the end. Otherwise, we return the first deleted location/index.
 * Runtime Expected/Worst: O(1)/O(n)
 */
int search (SET *sp, char *elt, bool *found) {
	int head = strhash(elt)%sp->length;
	int i, locn;
	int deletedlocn=-1;
	for (i = 0; i < sp->length; i++) {					// looping through the array
		locn = (head + i)%sp->length;					// but now we start with the hash value.
		if (sp->flags[locn] == FILLED) {				// FILLED means found, so return location/index
			if (strcmp(sp->data[locn],elt)==0){
				*found = true;
				return locn;
			}
		}
		else if (sp->flags[locn]==DELETED) {			// DELETED means it still could be further down in the array
			if (deletedlocn==-1) {						// so we just remember the first DELETED location
				deletedlocn = locn;
			}
		}
		else {											// must be EMPTY
			break;
		}
	}
	*found = false;										// we have not found the element
	if (deletedlocn == -1) {							// so if we haven not seen a deleted location, return that new empty location at the end
		return locn;
	}
	return deletedlocn;									// otherwise, return the first deleted location/index
}

/*
 * Function: addElement
 * -------------------------------------
 * Summary: Searches using linear probing for the element. We use a bool to keep track of whether it's been 
 * found. If not found, we insert the element in that spot and set the corresponding flag to FILLED.
 * Runtime Expected/Worst: O(1)/O(n)
 */
void addElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);			// the index where the element should go
	if(searched==false) {
		sp->data[index]=strdup(elt);				// same as previous lab, but now we have to update the flags array to FILLED
		sp->flags[index] = FILLED;
		sp->count++;
	}
}

/*
 * Function: removeElement
 * ------------------------------------
 * Summary: Searches using linear probing for the element. If found, we free the memory at the location, 
 * and set the corresponding flag to DELETED
 * Runtime Expected/Worst: O(1)/O(n)
 */
void removeElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);			// index where the element should go
	if(searched==true) {
		free(sp->data[index]);						// free the memory at index essentially deleting it
		sp->flags[index] = DELETED;					// now we set the flag in the same index of the flags array to DELETED
		sp->count--;
	}
}

/*
 * Function: *findElement
 * -------------------------------------
 * Summary: Uses the search function. If search is true, then we have found the element and return the 
 * element. Otherwise, return NULL.
 * Runtime Expected/Worst: O(1)/O(n)
 */
char *findElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);
	if (searched==true){
		return sp->data[index];
	}
	else {
		return NULL;
	}
}
/*
 * Function: **getElements
 * ----------------------------------
 * Summary: getElements returns a copy of the array for the user to play with
 * Runtime: O(n)
 */
char **getElements(SET *sp){
	assert(sp!=NULL);
	char **arr;
	int lastIndex = 0;
	arr = malloc(sizeof(char*)*sp->count);		// declare array and allocate memory to be size of the number of elements
	for (int i = 0;i < sp->length;i++){
		if (sp->flags[i] == FILLED){						// we only want an array of elements and not the entire thing so check if FILLED
			arr[lastIndex]=strdup(sp->data[i]);				// copy the data
			lastIndex++;
		}
	}
	return arr;
}
