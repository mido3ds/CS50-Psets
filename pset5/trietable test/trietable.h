/*
* Written By: MAHMOUD OTHMAN ADAS, 9 sep 16
* TrieTable Data Structure
* use it to store english words  
*/

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// numbers of alphbets
// added one for '\0' to indicate end of word
#define ALPHANUM 27

/*
* structure of TrieTable for alphabets
*/
typedef struct node
{
    char* c;		// pointer to character in the node
    struct node* ptr[ALPHANUM];		// array of pointers to other nodes
}node;


/*
* search in a trie table
* returns true if word is found, false otherwise
*/
bool Search_Node(char* word, node* head);

/*
* add word to the table
* returns non-zero on failure
*/
int Add_Word(char* word, node* head);

/*
* delete recursively tri nodes' contents
* returns non-zero on failure
*/
int Delete_Node(node* head);

/*
* returns alphabetical order of alpha char
* on failure returns negative num
*/
int Get_Alpha_Order(char c);

/*
* allocates momory for node and sets all pointers with NULL
* returns node pointer
*/
node* Generate_Node();