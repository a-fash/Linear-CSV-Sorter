#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	void *data;
	char *line;
	struct Node *next;
}Node;

Node * sortedMerge(Node *a, Node *b, int (*comparatorFnPtr)(void*, void*));
void frontBackSplit(Node *source, Node **front, Node **head);
int comparator_INT(void *data0, void *data1);
int comparator_STR(void *data0, void *data);
void printList(Node *);

void mergeSort(Node **headRef, int (*comparatorFnPtr)(void*, void*))
{
	Node *head = *headRef;
	Node *a;
	Node *b;

	if((head == NULL) || (head->next == NULL))
	{
		return;
	}

	frontBackSplit(head, &a, &b);

	mergeSort(&a, comparatorFnPtr);
	//printList(*headRef);

	mergeSort(&b, comparatorFnPtr);
	//printList(*headRef);


	*headRef = sortedMerge(a, b, comparatorFnPtr);

	//printList(*headRef);

	return;
}

Node * sortedMerge(Node *a, Node *b, int (comparatorFnPtr)(void*, void*))
{
	Node *result = NULL;

	if(a == NULL)
	{
		//printf("hit\n");
		return(b);
	}

	if(b == NULL)
	{
		//printf("hit\n");
		return(a);
	}

	int outcome = comparatorFnPtr(a->data, b->data);

	if(outcome >= 0)
	{
		result = a;
		result->next = sortedMerge(a->next, b, comparatorFnPtr);
	}
	else
	{
		result = b;
		result->next = sortedMerge(a, b->next, comparatorFnPtr);
	}

	return result;
}

void frontBackSplit(Node *source, Node **front, Node **back)
{
	Node *fast;
	Node *slow;
	slow = source;
	fast = source->next;

	while(fast != NULL)
	{
		fast = fast->next;
		if(fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	*front = source;
	*back = slow->next;
	slow->next = NULL;
}

void push(Node **headRef, void *data, size_t data_size, char *line, size_t line_size)
{


	Node *newNode = (Node *)malloc(sizeof(Node));

	//newNode->data = (void *)malloc(sizeof(void *));
	newNode->data = data;

	//newNode->line = (char *)malloc(sizeof(char *));
	newNode->line = line;

	if(*headRef == NULL)
		newNode->next = NULL; 
	else
		newNode->next = (*headRef);

	//newNode->next = (*headRef);

	/*
	int i;
	for(i=0; i<data_size; i++)
		*(char *)(newNode->data +i) = *(char *)(data + i);

	newNode->line = strcpy(newNode->line, line);
	*/

	(*headRef) = newNode;
}

void printList(Node *head)
{
	Node *ptr = head;
	int i = 1;

	while(ptr != NULL)
	{
		printf("%d: %s\n", i++, (char *)ptr->data);
		ptr = ptr->next;
	}
}

void freeList(Node *head)
{
	Node *ptr = head;
	Node *trail = head;
	//int i = 1;

	while(ptr != NULL)
	{
		//printf("%d: %s\n", i++, (char *)ptr->data);
		free(ptr->data);
		free(ptr->line);
		ptr = ptr->next;
		free(trail);
		trail = ptr;
	}
}


int comparator_INT(void *data0, void *data1)
{

	if( strcmp((char *) data0, "NULL") == 0 || strcmp((char *)data1, "NULL") == 0)
	{
		if(strcmp((char *)data0, "NULL") == 0 && strcmp((char *)data1, "NULL") == 0)
			return comparator_STR(data0, data1);

		if(strcmp((char *)data0, "NULL") == 0)
			return comparator_STR(data0, data1);

		if(strcmp((char *)data1, "NULL") == 0)
			return comparator_STR(data0, data1);

	}

	return *((int *)data1) - *((int *)data0);
}

int comparator_STR(void *data0, void *data1)
{
	if( strcmp((char *) data0, (char *) data1) == 0)
		return -1;

	if( strcmp((char *) data0, "NULL") == 0 )
		return 1;

	if( strcmp((char *) data1, "NULL") == 0 )
		return -1;

	return strcmp((char *)data1, (char *)data0);
}
