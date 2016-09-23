/*
* Written By: MAHMOUD OTHMAN ADAS, 9 sep 16
* TrieTable Data Structure
* use it to store english words  
*/

#include "trietable.h"

/*
* search in a trie table
* returns true if word is found, false otherwise
*/
bool Search_Node(char* word, node* head)
{
    if (word == NULL || head == NULL)
        return false;       // ERROR

	// store character and its order
	char character = tolower(word[0]);
	int order =  Get_Alpha_Order(character);
	if (order < 0)
		return false;

	// update head to point at next node
	head = head->ptr[order];

	/* CHECK */
	// node or word ended
	if (head == NULL)
		return false;
	// both arrived at end, "recursion base"
	else if (character == '\0' && *(head->c) == '\0')
		return true;
	// equall chars, search next node for next char
	else if (character == *(head->c))
		return Search_Node(++word, head);
	// chars not equall
	else
		return false;
}

/*
* add word to the table
* returns non-zero on failure
*/
int Add_Word(char* word, node* head)
{
    if (word == NULL || head == NULL)
        return 1;       // ERROR

	node* old_node = head;

    // iterate through word characters including '\0'
    for (int i = 0; i <= strlen(word); i++)
    {
		// store character and its order
		char character = tolower(word[i]);
		int order =  Get_Alpha_Order(character);
		if (order < 0)
			return false;

        // make new node
		node* new_node = Generate_Node();

		// link it with the previous
		old_node->ptr[order] = new_node;

		// store the char
        *(new_node->c) = character;

		// swap nodes for next iteration
		old_node = new_node;
    }

    return 0;
}

/*
* delete recursively tri nodes' contents
* returns non-zero on failure
*/
int Delete_Node(node* head)
{
	if (head == NULL)
		return -1;		// node not found

	// free node's char
	if (head->c != NULL)
		free(head->c);

	// free each pointer and its pointers
	for (int i = 0; i < ALPHANUM; i++)
	{
		if (head->ptr[i] != NULL)
			return Delete_Node(head->ptr[i]);
	}
		
	// free all node 
	free(head);

	// success
	return 0;
}

/*
* returns alphabetical order of alpha char
* on failure returns negative num
*/
int Get_Alpha_Order(char c)
{
	if (c == '\0')
		return 26;
	if (isalpha(c) == 0)
	{
		printf("\nError!\n");
		return -1;
	}
	return tolower(c) - 97;;
}

/*
* allocates momory for node and sets all pointers with NULL
* returns node pointer
*/
node* Generate_Node()
{
	node* new = (node*) malloc(sizeof(node));
	new->c = (char*) malloc(sizeof(char));
	for (int i = 0; i < ALPHANUM; i++)
		new->ptr[i] = NULL;
	return new;
}