// Nicholas Espinosa
// PID 2431401, NID ni714231
// 10.18.2015
// Assignment 3, ListyString.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

// Obtains a string from 
char *createString(FILE *ifp)
{
	char *str = malloc(sizeof(char) * 1024);

	// Reading in the string from the file and allocating proper amount of space
	fscanf(ifp, "%s", str);
	str = (char *)realloc(str, (strlen(str) + 1));

	// Returns the necessary string
	return str;
}

// Adapted from function createNode written by Sean Szumlanski 
// Original located in Linked Lists (Part 1 of 2), linked-lists.c
// Creates a new node
node *makeNode(char a)
{
	node *newNode = malloc(sizeof(node));
	
	// If memory cannot be allocated, program returns NULL
	if(newNode == NULL)
		return NULL;
	
	// Otherwise, node is created
	newNode->data = a;
	newNode->next = NULL;
	
	return newNode;
}

// Adapted from function insertNode written by Sean Szumlanski 
// Original located in Linked Lists (Part 1 of 2), linked-lists.c
// Adds a node to the list
node *addNode(node *head, char a)
{
	node *temp;

	// Ensures that the head exists
	if(head == NULL)
		return makeNode(a);

	// Finds the next available spot
	for(temp = head; temp->next != NULL; temp = temp->next);

	// Next node is created
	temp->next = makeNode(a);

	return head;
}

// Convert string into an array list
node *stringToList(char *str)
{
	int i;
	node *head = NULL;
	
	// If the string is NULL, the head is null
	if(str == NULL || str[0] == '\0')
		return NULL;

	for(i = 0; i < strlen(str); i++)
		head = addNode(head, str[i]);

	return head;
}

// Replaces char with either 
node *replaceChar(node *head, char key, char *str)
{
	int i;
	node *temp, *prev = NULL, *store, *secTemp;

	// Removing a character from the list
	if(str == NULL || str[0] == '\0')
	{
		for(temp = head; temp != NULL; temp = temp->next)
		{
			store = malloc(sizeof(node));

			// Removing from the head
			if(temp->data == key && temp == head)
			{
				store = head;
				head = head->next;
			} // Removing from middle
			else if(temp->data == key && temp->next != NULL && prev != NULL) 
			{
				store = temp;
				prev->next = temp->next;
				temp = prev;
			} // Removing from tail
			else if(temp->data == key && temp->next == NULL)
			{
				store = temp;
				temp = prev;
				prev->next = NULL;
			}
			
			// Pointer to previous node is updated and necessary data is freed
			prev = temp;
			free(store);
		}
		return head;
	}

	// If we are replacing 
	for(temp = head; temp != NULL; temp = temp->next)
	{		
		if(temp->data == key)
		{	
			// Saving the remainder of the list
			temp->data = str[0];

			// Inserting the string into the list
			for(i = 1; i < strlen(str); i++)
			{
				secTemp = temp->next;
				temp->next = makeNode(str[i]);
				temp->next->next = secTemp;
				temp = temp->next;
			}
		}
	}

	return head;
}

// Reverses the linked list
node *reverseList(node *head)
{
	int i = 0, count = 0;
	node *temp;
	char *word;

	// Obtaining the size of the list
	for(temp = head; temp->next != NULL; temp = temp->next)
		count++;

	// Allocating an array based upon that size
	word = malloc(sizeof(char) * (++count));
	
	// Placing the word into that array
	for(temp = head; temp != NULL; temp = temp->next)
		word[i++] = temp->data;


	temp = head;
	
	// Placing the letters in reverse order
	for(i = count - 1; i >= 0 ; i--)
	{
		temp->data = word[i];
		temp = temp->next;
	}
	
	// Freeing the character array
	free(word);

	return head;
}

// Prints the current list
void printList(node *head)
{
	node *temp;

	// If the list is currently empty
	if(head == NULL)
		printf("(empty string)\n");
	else // Otherwise, print all characters within the list
	{
		for(temp = head; temp != NULL; temp = temp->next)
			printf("%c", temp->data);

		printf("\n");
	}
}

int main(int argc, char **argv)
{
	node *head;
	char action, key;
	char *str, *replace;
	FILE *ifp;
	
	// Obtaining the file name and accessing the file
	ifp = fopen(argv[1], "r");

	// Creating the initial linked list
	str = createString(ifp);
	head = stringToList(str);

	// While the program is recieving input
	while(!feof(ifp))
	{	
		// Reading in the action to be performed
		fscanf(ifp, "%c", &action);

		if(action == '@') // Replacing a key with String
		{
			// Reading in necessary data
			fscanf(ifp, " %c ", &key);
			replace = createString(ifp);

			// Replacing that key within the string
			head = replaceChar(head, key, replace);

			// Freeing replace if it is used
			free(replace);	
		}
		else if(action == '-') // Deleting a key from the list
		{
			// Reading in the key
			fscanf(ifp, " %c ", &key);
	
			// Deleting that character
			head = replaceChar(head, key, NULL);		
		}
		else if(action == '~') // Reversing the current list
			head = reverseList(head);	
		else if(action == '!') // Printing out the current list
			printList(head);
	}

	fclose(ifp);

	return 0;
}
