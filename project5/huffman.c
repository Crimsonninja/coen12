#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"

/**
 * This file uses Huffman coding to compress files. It makes use of the priority queue written the earlier 
 * week and from it, we build a huffman tree bottom-up starting with its leaves and building each node 
 * upward with new nodes as the sum of the data of its two children. Once the tree is built, we assign 1's as
 * we go down the right and 0's as we go down the left and the leaves are assigned a bit encoding (this is all
 * abstracted into the "pack" function), but we still need to "count the number of hops it takes for the leaves
 * to get to the root").
 * @author Jonathan Trinh
 * @version 5302017
 */

typedef struct node NODE;

/*
 * Function: hopNumber
 * Summary: This function takes in a leaf of a tree and traverses up the tree until it reaches the root.
 * While doing so, it counts the number of hops it takes to get there and returns how far away the specified
 * leaf is from the root.
 * Runtime: O(n)
 */
int hopNumber(struct node *leaf) {
	int hops = 0;
	while (leaf->parent!=NULL) {
		leaf = leaf->parent;
		hops++;
	}
	return hops;
}
/*
 * Function: compare
 * Summary: Like integer compare, but this time compares the data in two nodes.
 * Runtime: O(1)
 */
int compare(struct node *first, struct node *second) {
	return (first->count < second->count) ? -1 : (first->count > second->count);
}

int main(int argc, char *argv[])
{
	/*
	 * Step 1: We will start by counting the number of occurrences of each character in the file.
	 * We do this by using the fgetc function and we will constantly readin the next character
	 * until we hit the end of the file. Each time we do this, we will increment the counter
	 * at the index corresponding to the ascii value of the character.
	 */
	FILE *fp = fopen(argv[1],"r");		// Initialize pointer to file and open it to be read
	if (fp == NULL) {					// Check to see if the file exists.
  		return 0;
	}

	int occurences[257] = {0};			// initalize occurences array to all 0 with size 257 (one extra space for end of file)

	do {								// we will constantly grab the next character
	  int c;
	  
      c = fgetc(fp);
      if( feof(fp) )					// we hit the end of file so we break
      {
         break;
      }
      occurences[c]++;
    } while(1);

    /*
     * Step 2: We now create a priority queue which will store trees with a non zero count as well as leaves which
     * will store the nodes that are leaves of the tree.
     */
    PQ *pqueue = createQueue(compare);

    struct node* leaves[257] = {0};			// create leaves array
    for (int i = 0; i < 257; i++) {			// and initialize all its values to NULL
    	leaves[i] = NULL;
    }


    for (int i = 0; i < 256; i++) {							// for each nonzero count, we create a new node for it, add ito to the pqriority queue as well as the leaves array
    	if (occurences[i]>0)
    	{
    		NODE *thenode=malloc(sizeof(struct node));
    		thenode->count = occurences[i];
    		thenode->parent = NULL;
    		addEntry(pqueue,thenode);
    		leaves[i]= thenode;
    	}
    }

    // creating a tree with a zero count for the end of file marker
    NODE *zeroCountNode = malloc(sizeof(struct node));
    zeroCountNode->count = 0;
    zeroCountNode->parent = NULL;
    addEntry(pqueue,zeroCountNode);
    leaves[256] = zeroCountNode;

    /*
     * Step 3: We remove the two lowest priority trees as long as the priority queue has more than 
     * one tree in it, and make a new tree whose data or "count" is the sum of the ones we just removed.
     * We then insert this new tree back into the priority queue, and eventually if we do this enough times,
     * we will get one big tree at the very end of the process in the priority queue.
     */
    while (numEntries(pqueue)>1) {
    	NODE *first = removeEntry(pqueue);				// the first of the lowest priority trees
    	NODE *second = removeEntry(pqueue);				// the second of the lowest priority trees
    	NODE *third = malloc(sizeof(struct node));		// the new node created that will have its count as the count of its two children (the ones we just removed)
    	third->count = first->count + second->count;
    	third->parent = NULL;
    	first->parent = third;
    	second->parent = third;
    	addEntry(pqueue, third);						// we add that new tree back into the priority queue
    }


    /*
     * Step 4: We iterate through the leaves array, and if there is a valid node, we use the hopNumber 
     * function (reference the method above) to find the number of hops it takes to get from that leaf 
     * to the root, and we print the stats of that character.
     */
    for (int i = 0; i < 257; i++) {
    	if (leaves[i]!=NULL) {
    		int numberOfHops;
    		numberOfHops = hopNumber(leaves[i]);
    		if (isprint(i))								// check if the character is printable
    		{
    			printf("%c: %d x %d bits = %d\n", i, occurences[i], numberOfHops, occurences[i]*numberOfHops);
    		}
    		else {
    			printf("%03o: %d x %d bits = %d\n", i, occurences[i], numberOfHops, occurences[i]*numberOfHops);
    		}
    		
    	}
    }

    /*
     * Step 5: Pack the input file (the first parameter) using the pack function given to us.
     */
    pack(argv[1],argv[2],leaves);
}
