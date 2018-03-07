#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#define AVG_LENGTH 20

/*
 * This file defines the struct called set and implements its 
 * different functions including creating and destroying the 
 * set and adding and removing elements. This file depends 
 * on functions written in list.c (details are commented above
 * each function).
 * @author: Jonathan Trinh
 * @version: 5182017
 */
struct set
{
	int count;	/*number of elements*/
	int length; /*length of array */
	LIST **lists; /*array of strings */
	int (*compare)(); /* a compare function in the set */
	unsigned(*hash)(); /* equivalent of strhash stored in the set */
};
typedef struct set SET;

/*
 * Function: *createSet
 * -------------------------------
 * Summary: This function allocates memory and creates a set that keeps track of the number of elements,
 * the length of the array, and lists. Thus, we have to go through each spot and create a list that will store
 * nodes that contain that data that has to that array location. We are additionally given compare and hash
 * functions which we much initialize.
 * Runtime: O(n)
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)() ){
	SET *sp;

	sp=malloc(sizeof(SET));									// declare set and allocate memory
	assert(sp!=NULL);

	sp->count = 0;											// this count element keeps track of the number of elements currently in the set
	sp->length=maxElts/AVG_LENGTH;							// this is the length of the array
	sp->lists = malloc(sizeof(void*)*sp->length);			// this is an array where the actual list (which stores the nodes which store the data)
	sp->compare = compare;
	sp->hash = hash;

	int i;
	for (i = 0;i<sp->length;i++) {							// at each spot in the array, we want to create an empty list, which we will populate with node's with data that hash to that array location
		sp->lists[i]=createList(compare);
	}

	assert(sp->lists!=NULL);
	return sp;
}

/*
 * Function: destroySet
 * --------------------------------
 * Summary: We simply just free the lists array and the set itself
 * Runtime: O(1)
 */
void destroySet(SET *sp){
	free(sp->lists);						// free the lists array
	free(sp);								// finally we free the set
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
 * -------------------------------------
 * Summary: Most of the heavily lifting was done in "list.c." We first keep track of the hash location using
 * the hash function given to us. Then use findItem (as written in "list.c") to determine if we have found the
 * item in that spot in the array. If we have not found it, we can add it to the list.
 * Runtime: O(n)
 */
void addElement(SET *sp, void *elt){
	assert(sp!=NULL && elt!=NULL);
	int index = (*sp->hash)(elt)%sp->length;
	if (findItem(sp->lists[index],elt)==NULL) {			// it is not found
		addFirst(sp->lists[index],elt);
		sp->count++;
	}
	
}

/*
 * Function: removeElement
 * ------------------------------------
 * Summary: Like add element, we keep track of the hash location and use findItem to search for the specified
 * element. If found, we use removeItem (written in list.c) to take the node containing that element ouf of 
 * the list.
 * Runtime: O(n)
 */
void removeElement(SET *sp, void *elt){
	assert(sp!=NULL && elt!=NULL);
	int index = (*sp->hash)(elt)%sp->length;
	if (findItem(sp->lists[index],elt)!=NULL) {			// it is found
		removeItem(sp->lists[index],elt);
		sp->count--;
	}
}

/*
 * Function: *findElement
 * -------------------------------------
 * Summary: This function returns the element if found, and if not, it returns NULL. This is kind of like findItem
 * in list.c. Then, all we have to do is get the hash location in the array and call findItem on it returning whatever
 * it gives us.
 * Runtime: O(n)
 */
void *findElement(SET *sp, void *elt){
	assert(sp!=NULL && elt!=NULL);
	int index = (*sp->hash)(elt)%sp->length;
	return findItem(sp->lists[index],elt);
}
/*
 * Function: **getElements
 * ----------------------------------
 * Summary: getElements returns a copy of the array for the user to play with. But, since we originally had an array
 * of linked lists which we now have to turn into a 1-D array, sof ro each index in the "lists" array, we have to 
 * create another array which we copy the data from the list into that array. From there, we use another for loop to
 * copy the data from the "copiedArray" into the actual array we want to return.
 * Runtime: O(n^2)
 */
void *getElements(SET *sp){
	assert(sp!=NULL);
	void **arr;
	int copiedcount = 0;
	arr = malloc(sizeof(void*)*sp->count);		// declare array and allocate memory to be size of the number of elements
	for (int i = 0;i < sp->length;i++){
		void **copiedArray = getItems(sp->lists[i]);

		for (int j = 0; j < numItems(sp->lists[i]); j++) {
			arr[copiedcount] = copiedArray[j];
			copiedcount++;
		}
		
	}
	return arr;
	
}
