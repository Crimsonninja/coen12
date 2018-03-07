#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"
#define START_LENGTH 10
#define p(x) (((x)-1)/2)
#define l(x) (((x)*2)+1)
#define r(x) (((x)*2)+2)

/**
 * This file defines the pqueue struct and all of its accompanying functions that will 
 * be needed for Huffman Coding including creating and destroying the queue as well as
 * adding and removing entries to the queue itself. What makes it a priority queue is 
 * the way it organizes data when something is added to the queue (pleas refer to add
 * and remove for specific documentation).
 * @author Jonathan Trinh
 * @version 5252017
 */

/**
 * Struct: pqueue
 * -----------------------------------
 * Summary: This struct represents a priority queue which keeps track of the cnumber 
 * of elements it has, the length of the array (in case we need to realloc and create
 * more space), the array itself, and a compare function.
 */
struct pqueue
{
	int count;
	int length;
	void **data;
	int (*compare)();
};

/**
 * Function: createQueue
 * ------------------------------------
 * Summary: This functiona allocates memory and creates a priority queue that keeps
 * track of the count initializing it to 0, the length which we assign to a starting
 * value of 10, and for the data array, we allocate memory to hold 10 void* types foro
 * now. We are also given a compare function as a parameter, and we thus initialize
 * our compare function that we defined earlier in the struct.
 * Runtime: O(1)
 */
PQ *createQueue(int (*compare)()) {
	PQ *pq;
	pq=malloc(sizeof(PQ));
	assert(pq!=NULL);

	pq->count = 0;
	pq->length = START_LENGTH;
	pq->data =malloc(sizeof(void*)*START_LENGTH);
	pq->compare = compare;
	assert(compare!=NULL);

	return pq;
}
/**
 * Function: destroyQueue
 * -------------------------------------
 * Summary: Destroys the queue by freeing each element in the datay array, the array 
 * itself, and then finally the priority queue.
 * Runtime: O(n)
 */
void destroyQueue(PQ *pq) {
	for(int i = 0; i < pq->count; i++){
		free(pq->data[i]);
	}
	free(pq->data);
	free(pq);
}

/**
 * Function: numEntries
 * --------------------------------------
 * Summary: Simply returns the count of the number of elements the priority queue has
 * Runtime: O(1)
 */
int numEntries(PQ *pq) {
	return pq->count;
}

/**
 * Function: addEntry
 * ---------------------------------------
 * Summary: addEntry adds an element to the end of the array, but needs to determine 
 * whether that addition has maintained the min-heap nature of the array. If not, it 
 * needs to reheapup and keep swapping the added element with its parent until it 
 * becomes a min-heap. In addition, if the count reaches the length of the array, we'll
 *  need to reallocate and make the size of the array bigger by some factor (in this 
 * case by 2).
 * Runtime: O(log n)
 */
void addEntry(PQ *pq, void *entry) {
	assert(pq!=NULL && entry!=NULL);
	if (pq->count==pq->length)
	{
		pq->data=realloc(pq->data,sizeof(void*)*pq->length*2);	// since we have now completely filled the array, double its size
		pq->length = pq->length * 2;
	}
	pq->data[pq->count]=entry;									// add the element to the end of the array
	int index = pq->count;										// this will keep track of where we are in the array when swapping
	while (pq->compare(pq->data[index],pq->data[p(index)])<0) {				// reheapup by continually comparing with parent
		//swap with parent
		void* temp = pq->data[p(index)];
		pq->data[p(index)] = pq->data[index];					// if child is smaller than parent, then we swap
		pq->data[index] = temp;

		index=p(index);
	}
	pq->count++;												// increment count
}
/**
 * Function: *removeEntry
 * ----------------------------------------
 * Summary: This function removes the first element in the array (since it is a priority 
 * queue), replaces that spot with the last thing in the array, and does reheapdown. We 
 * check if the new first element has any children. If it has both children, we will need 
 * to compare the two children to see which one is smaller, and we will swap with that one
 * (provided that the child is smaller than the parent). If it only has a left child and 
 * if that left child is smaller than its parent, then we will swap. We finally return the 
 * deleted element.
 * Runtime: O (log n)
 */
void *removeEntry(PQ *pq) {
	void* root = pq->data[0];												// store the data of the first element for return later
	int index = 0, smallestIndex = 0;										// index will keep track of the index we are at when we swap, and smallestIndex will determine which child is hypothetically smallest (although it does not guarantee swapping)
	pq->data[index]=pq->data[pq->count-1];									// replace the first element with the last
	// int smallestIndex = 0;
	
	pq->count--;															// decrement the count
	while (l(index)<pq->count){												// while we have a left child, we will continually run this loop
		smallestIndex = l(index);
		if (r(index)<pq->count) {											// we have a right child so we now must compare between the two children
			if (pq->compare(pq->data[l(index)],pq->data[r(index)])<0){
				smallestIndex=l(index);
			} else {
				smallestIndex=r(index);
			}
		}
		
		if (pq->compare(pq->data[smallestIndex],pq->data[index])<0) {		// in either case, which ever children is hypothetically smallest. If it is smaller than the parent, then we swap
			void* temp = pq->data[smallestIndex];
			pq->data[smallestIndex] = pq->data[index];
			pq->data[index] = temp;
			index=smallestIndex;											
		}
		else {
			break;
		}
	}
	return root;															// return the data stored in the beginning
}