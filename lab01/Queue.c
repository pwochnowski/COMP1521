// ADT for a FIFO queue
// COMP1521 17s2 Week01 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

typedef struct QueueNode {
	int jobid;  // unique job ID
	int size;   // size/duration of job
	struct QueueNode *next;
} QueueNode;

struct QueueRep {
	int nitems;      // # of nodes
	QueueNode *head; // first node
	QueueNode *tail; // last node
};



// create a new node for a Queue
	static
QueueNode *makeQueueNode(int id, int size)
{
	QueueNode *new;
	new = malloc(sizeof(struct QueueNode));
	assert(new != NULL);
	new->jobid = id;
	new->size = size;
	new->next = NULL;
	return new;
}



// make a new empty Queue
Queue makeQueue()
{
	Queue new;
	new = malloc(sizeof(struct QueueRep));
	assert(new != NULL);
	new->nitems = 0; new->head = new->tail = NULL;
	return new;
}

// release space used by Queue
void  freeQueue(Queue q)
{
	assert(q != NULL);
	QueueNode* iter = q->head;
	while (iter) {
		QueueNode* next = iter->next; 
		free(iter);
		iter = NULL;
		iter = next;
	}
	free(q);
	q = NULL;
}

// add a new item to tail of Queue
void  enterQueue(Queue q, int id, int size)
{
	assert(q != NULL);
	QueueNode *n = makeQueueNode(id, size);
	if (q->tail) q->tail->next = n;
	else q->head = n;
	q->nitems++; 	
	q->tail=n;
}

// remove item on head of Queue
int   leaveQueue(Queue q)
{
	assert(q != NULL);
	int id = 0;
	if (q->head) {
		QueueNode *n = q->head;
	  id = n->jobid;
		q->head = n->next;
		if (q->nitems == 1) q->tail = NULL;
		q->nitems--;
		free(n);
	}
	return id; // replace this statement
}

// count # items in Queue
int   lengthQueue(Queue q)
{
	assert(q != NULL);
	return q->nitems;
}

// return total size in all Queue items
int   volumeQueue(Queue q)
{
	assert(q != NULL);
	QueueNode* iter = q->head;
	int vol = 0;
	while (iter) {
		vol += iter->size;
		iter = iter->next;
	}
	return vol;
}

// return size/duration of first job in Queue
int   nextDurationQueue(Queue q)
{
	assert(q != NULL);
	if (!q->head) return 0;		
	return q->head->size; // replace this statement
}


// display jobid's in Queue
void showQueue(Queue q)
{
	QueueNode *curr;
	curr = q->head;
	while (curr != NULL) {
		printf(" (%d,%d)", curr->jobid, curr->size);
		curr = curr->next;
	}
}
