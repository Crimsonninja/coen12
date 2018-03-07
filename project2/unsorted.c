#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/*
 * This file defines the struct called set (specifically an unsorted set) and implements its 
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

static int search(SET *sp, char *elt);		// prototyping the search function

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
	free(sp->data);				// we then free the array
	free(sp);					// finally we free the set
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
 * Function: addElement
 * ----------------------------------
 * Summary: First searches (linearly) the set for the specified element. If it is not found, it will add the
 * element to the end of the set and increase count which keeps track of the number of elements.
 * Runtime: O(n)
 */
void addElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);

	// Searches through array first
	if(search(sp, elt)==-1){
		sp->data[sp->count]=strdup(elt);		// if found, then we add element to end of the array
		sp->count++;
	}
}

/*
 * Function: removeElement
 * ----------------------------------
 * Summary: First searches (linearly) the set for the specified element. If it is found, it will free the memory
 * at that location and place the last element in its place.
 * Runtime: O(n)
 */
void removeElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	int index = search(sp,elt);					// Searches through array first

	if(index!=-1){
		free(sp->data[index]);					// Free the memory at that index
		sp->data[index]=sp->data[sp->count-1];	// Place the current last element into that slot
		sp->data[sp->count-1]=NULL;
		sp->count--;
	}
}

/*
 * Function: *findElement
 * ----------------------------------
 * Summary: *findElement is similar to search. It will linearly search through the set and return a specified e
 * lement. If not found, it will return NULL.
 * Runtime: O(n)
 */
char *findElement(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	int i;

	// for loop to iterate through the array
	for(i=0;i<sp->count;i++){
		if(strcmp(sp->data[i],elt)==0){		// use strcmp to compare the string in that index to elt
			return elt;
		}
	}
	return NULL;
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

/*
 * Function: search
 * ----------------------------------
 * Summary: search linearly searches through the set and returns the index of the specified element if 
 * found. It not found, it will return -1
 * Runtime: O(n)
 */
static int search(SET *sp, char *elt){
	assert(sp!=NULL && elt!=NULL);
	int i;

	// for loop to iterate through the array
	for(i=0;i<sp->count;i++){
		if(strcmp(sp->data[i],elt)==0){		// compare the string at the index to element
			return i;
		}
	}
	return -1;								// at this point, element is not found so -1 is returned
}
