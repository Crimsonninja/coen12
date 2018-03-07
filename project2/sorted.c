#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * This file defines the struct called set (specifically an sorted set) and implements its 
 * different functions including creating and destroying the set, adding and removing elements, 
 * and searching (details are commented above each function).
 * @author: Jonathan Trinh
 * @version: 4132017
 */
struct set
{
	int count;	/*number of elements*/
	int length; /*length of array */
	char **data; /*array of strings */
};
typedef struct set SET;

static int search(SET *sp, char *elt, bool *found);		// prototyping the search function

/*
 * Function: *createSet
 * -------------------------------
 * Summary: This function allocates memory and creates a set that keeps track of the number of elements,
 * the length of the array, and the array itself (called "data")
 * Runtime: O(1)
 */
SET *createSet(int maxElts){
	SET *sp;

	sp=malloc(sizeof(SET));						// declare set and allocate memory
	assert(sp!=NULL);

	sp->count = 0;								// this count element keeps track of the number of elements currently in the set
	sp->length=maxElts;							// this is the length of the array
	sp->data = malloc(sizeof(char*)*maxElts);	// this is an character array where the actual data is stored
	assert(sp->data!=NULL);
	return sp;
}

/*
 * Function: destroySet
 * --------------------------------
 * Summary: Destroys the set by freeing all the memory starting with the members of the character array, 
 * the character array itself, and then the set
 * Runtime: O(n)
 */
void destroySet(SET *sp){
	int i;

	// we first free memory of the elements themselves
	for (i=0;i<sp->count;i++){	
		free(sp->data[i]);
	}
	free(sp->data);						// we then free the array
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
 * Summary: This search function performs a binary search. We set bounds called, low, mid, and hi. 
 * diff will return an integer lexicographically comparing the strings. We use this to change our hi or 
 * low bounds depending on whether the first word comes before or after the second.
 * Runtime: O(logn)
 */
int search (SET *sp, char *elt, bool *found) {
	int lo, hi, mid, diff;		// declare variables

	lo = 0;
	hi=sp->count-1;				// defining our lower and upper bounds (on one side of each array)
	while (lo<=hi) {
		mid = (lo + hi)/2;
		diff = strcmp(elt,sp->data[mid]);
		if(diff < 0) {			// if the word being searched for comes before the word in the array, we change our hi bound, so we only consider the first half
			hi = mid - 1;
		}
		else if(diff>0) {		// if the word being searched for comes after the word in the array, we change our lo bound to consider only the second half
			lo = mid + 1;
		}
		else {
			*found = true;		// if the elt is the same as the word in the array, we assign *found to be true and return the index
			return mid;
		}
	}
	*found = false;				// at this point, we have not found the element, so we assign *found to be false
	return lo;					// return where the index where the element should go
}

/*
 * Function: addElement
 * -------------------------------------
 * Summary: Searches (using binary search) for the element. We use a bool to keep track of whether it's been 
 * found. If not found, we shift everything above where the element should go one to the right and insert the element in that spot.
 * Runtime: O(n)
 */
void addElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);			// the index where the element should go
	if(searched==false) {

		// for loop shift everything to the right starting at the end
		for (int i = sp->count; i > index; i--)
		{
			sp->data[i]=sp->data[i-1];
		}
		sp->data[index]=strdup(elt);				// now that everything is shifted, we assign that empty spot in the array to a duplicated string
		sp->count++;
	}
}

/*
 * Function: removeElement
 * ------------------------------------
 * Summary: Searches (using binary search) for the element. If found, we free the memory at the location, 
 * and shift everything above the element one to the left.
 * Runtime: O(n)
 */
void removeElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);			// index where the element should go
	if(searched==true) {
		free(sp->data[index]);						// free the memory at index essentially deleting it
		for (int i = index+1; i < sp->count;i++){	// shift everything above the index one to the left
			sp->data[i-1]=sp->data[i];
		}
		sp->count--;
	}
}

/*
 * Function: *findElement
 * -------------------------------------
 * Summary: Uses the search function. If search is true, then we have found the element and return the 
 * element. Otherwise, return NULL.
 * Runtime: O(logn)
 */
char *findElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	bool searched;
	int index = search(sp, elt, &searched);
	if (searched==true){
		return elt;
	}
	else {
		return NULL;
	}
}
/*
 * Function: **getElements
 * ----------------------------------
 * Summary: getElements returns a copy of the array for the user to play with.
 * Runtime: O(n)
 */
char **getElements(SET *sp){
	assert(sp!=NULL);
	char **arr;
	arr = malloc(sizeof(char*)*sp->count);		// declare array and allocate memory to be size of the number of elements
	for (int i = 0;i < sp->count;i++){
		arr[i]=strdup(sp->data[i]);				// copy the data
	}
	return arr;
}
