#include <stdlib.h>
#include <assert.h>
#include "list.h"

/**
 * This file (list.c) defines two structs called node and list and implements the basic methods
 * of a list. We use a doubly ended queue (whose underlying structure is a doubly-linked list).
 * Its specific documentation for each function is done with blockc commenting above it followed 
 * by a few in-line comments.
 * @author: Jonathan Trinh
 * @version: 5132017
 */

/*
 * Struct: node
 * --------------------------------------------------
 * Summary: This is a basic struct called node that will be the building block of our list. It 
 * stores data, and pointers to the next and previous nodes.
 */
struct node
{
	void *data;
	struct node *next;
	struct node *prev;
};

/*
 * Struct: list
 * --------------------------------------------------
 * Summary: This is the overall struct and consists of nodes. It however, must keep track of the
 * number of nodes it has (not including the dummy node). The list has a reference to the head or
 * front of the list. It also contains a compare function that is already given to us.
 */ 
struct list
{
	int count;
	struct node *head;
	int (*compare)();
};

/*
 * Function: *createList
 * --------------------------------------------------
 * Summary: This function allocates memory and creates a list contains a dummy node whose data 
 * we don't care about. It's next and previous pointers are itself because it is the only element
 * in the list at this point. The list also keeps track of the number of
 * elements that it has. (It should be noted that this dummy node is not part of the count)
 * Runtime: O(1)
 */
LIST *createList(int (*compare)()) {
	LIST *lp;

	lp=malloc(sizeof(LIST));						// declare list and allocate memory
	assert(lp!=NULL);

	lp->count = 0;	

	lp->head = malloc(sizeof(struct node));
	lp->head->next = lp->head;
	lp->head->prev = lp->head;

	lp->compare = compare;							// we are given a compare function as a parameter so we initialize it here
	return lp;
}

/*
 * Function: destroyList
 * --------------------------------------------------
 * Summary: This function destroys the list by continually removing the last element while there 
 * exists something in the list (count > 0). (Refer to the removeLast documentation below)
 * Runtime: O(n)
 */
void destroyList(LIST *lp) {
	assert(lp!=NULL);
	while(lp->count>0) {
		removeLast(lp);
	}
}

/*
 * Function: numItems
 * --------------------------------------------------
 * Summary: Simply returns the count of the number of items in the list
 * Runtime: O(1) 
 */
int numItems(LIST *lp) {
	assert(lp!=NULL);
	return lp->count;
}

/*
 * Function: addFirst
 * --------------------------------------------------
 * Summary: A function that inserts a node into a list with some data at the front-end 
 * of the queue. This is done by creating the node with the specified item as its data,
 * then assigning the new node's next pointer to the head's next pointer, the head's
 * next pointer to the new node, the new node's previous pointer to the head, and the
 * next node's (the node that was originally head's next) previous pointer to the new node
 * Runtime: O(1)
 */
void addFirst(LIST *lp, void *item) {
	struct node *newNode = malloc(sizeof(struct node));
	newNode->data = item;

	newNode->next = lp->head->next;
	lp->head->next = newNode;

	newNode->prev = lp->head;
	newNode->next->prev = newNode;

	lp->count++;
}
/* 
 * Function: addLast
 * --------------------------------------------------
 * Summary: A function that inserts a node into a list with some data at the rear-end
 * of the queue. This is done by (as above) creating the node with the specified item
 * as its data then assigning the new node's next pointer to the head (since this is
 * a circular list) and the new node's previous pointer to the head's previous. Then 
 * the head's previous pointer now points to the new node and the new new node's previous' next
 * pointer is the new node (Wow, that was a mouthful).
 * Runtime: O(1)
 */
void addLast(LIST *lp, void *item) {
	struct node *newNode = malloc(sizeof(struct node));
	newNode->data = item;

	newNode->next = lp->head;
	newNode->prev = lp->head->prev;

	lp->head->prev= newNode;
	newNode->prev->next = newNode;

	lp->count++;
}

/*
 * Function: removeFirst
 * --------------------------------------------------
 * Summary: This is a function that removes the first element (at the front end) of the list,
 * and returns its data. We first declare a temp variables that stores the first element, and
 * another to store its data. Then we set the head's next pointer to the first Node's next, and
 * that second node's previous pointer to the head, thereby completely cutting out the first node
 * out of the list. Since we have a reference to that pointer, we can free it, and return the data.
 * O(1)
 */
void *removeFirst(LIST *lp) {
	assert(lp->count>0);

	struct node *firstNode = lp->head->next;
	void *firstData = firstNode->data;

	lp->head->next=firstNode->next;
	firstNode->next->prev=lp->head;

	free(firstNode);
	lp->count--;

	return firstData;
}

/*
 * Function: removeLast
 * --------------------------------------------------
 * Summary: This is a function that removes the last element (at the rear end) of the list,
 * and return its data. We do the same as above in remove Last, but all the action happens
 * on the other side of the list. We set the last node's previous's next to the head, and 
 * and the head's previous pointer to last node's previous (or that second to last node),
 * once again effectively cutting out the last node from the list. Now, we free it.
 * Runtime: O(1)
 */
void *removeLast(LIST *lp) {
	assert(lp->count>0);

	struct node *lastNode = lp->head->prev;
	void *lastData = lastNode->data;

	lastNode->prev->next = lp->head;
	lp->head->prev = lastNode->prev;

	free(lastNode);
	lp->count--;

	return lastData;
}

/*
 * Function: *getFirst
 * --------------------------------------------------
 * Summary: Simply returns the data of the first element in the deque.
 * Runtime: O(1)
 */
void *getFirst(LIST *lp) {
	assert(lp->count>0);
	return lp->head->next->data;
}

/*
 * Function: *getLast
 * --------------------------------------------------
 * Summary: Simply returns the data of the last element in the deque.
 * Runtime: O(1)
 */
void *getLast(LIST *lp) {
	assert(lp->count>0);
	return lp->head->prev->data;
}

/*
 * Function: removeItem
 * --------------------------------------------------
 * Summary: This is a function that removes a node given a list and some data. We have to first
 * traverse down the list until we find the node with the corresponding data. Thus, since we have
 * the number of nodes in the list, we can use a for loop and the compare function to find the
 * correct node to remove. If found, we change the pointers around to cut the node out of the list, free the node, decrement the count,
 * and break out of the loop.
 * Runtime: O(n)
 */
void removeItem(LIST *lp, void *item) {
	assert(lp->compare!=NULL && lp->count>0 && item!=NULL);
	int i;
	struct node *theNode = lp->head->next; 					// here, we keep a reference to the node
	for (i = 0; i < lp->count; i++) {
		if (lp->compare(theNode->data,item)==0)				// it matches
		{
			theNode->prev->next = theNode->next;			// change the specified node's previous's next pointer to the specified node's next
			theNode->next->prev = theNode->prev;			// change the specified node's next's previous pointer to the specified node's previous
			free(theNode);
			lp->count--;
			break;
		}
		else {												// in this case, it doesn't match and we keep traversing
			theNode = theNode->next;
		}
	}
}

/*
 * Function: *findItem
 * --------------------------------------------------
 * Summary: This is a function that returns some data if a node with that data is found in the list. Otherwise,
 * it returns NULL. We traverse through the array (as described above in removeItem), and if we have found the
 * node we return its data. If not, we keep traversing. If we haven't found it at the end, we return NULL.
 * Runtime: O(n)
 */
void *findItem(LIST *lp, void *item) {
	assert(lp->compare!=NULL);
	assert(item!=NULL);
	int i;
	struct node *theNode = lp->head->next;					// keep a reference to the node which we use to traverse
	for (i = 0; i < lp->count; i++) {
		if (lp->compare(theNode->data, item)==0) {			// it matches so return the data
			return theNode->data;
		}
		theNode = theNode->next;							// otherwise, keep traversing
	}
	return NULL;
}
/*
 * Function: *getItems
 * --------------------------------------------------
 * Summary: This is a function that returns an array with all the data from each node for the user to play with.
 * We intialize an array with size big enough to store however many elements are in the list and traverse through
 * the entire list and copy each node's data into the array.
 * Runtime: O(n)
 */
void *getItems(LIST *lp) {
	void **arr;
	arr = malloc(sizeof(void*)*lp->count);
	int i;
	struct node *theNode = lp->head->next;					// keep a reference to the node which we use to traverse
	for (i=0;i<lp->count;i++) {
		arr[i]=theNode->data;								// copy the data
		theNode = theNode->next;							// traverse to the next node
	}
	return arr;
}
