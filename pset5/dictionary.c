/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

// store dictionary file
FILE* dictionary_file = NULL;

// store dictionary words in trie tables
node* dictionary_buffer = NULL;

// number of words
// starts from -1 as there is one empty line
unsigned int words = -1;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    return Search_Node(word, dictionary_buffer);
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // open dictionary file
    dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        printf("\nCouldn't open dictionary!\n");
        return false;
    }

    // generate table
    dictionary_buffer = Generate_Node();
    if (dictionary_buffer == NULL)
    {
        printf("\nCouldn't generate buffer!\n");
        return false;
    }

//    char* word = NULL;      // to store the whole word
	char word[LENGTH];
    int i = 0;    // index of character in word
    char c;    // hold character 

    // iterate through all characters
    while (true)
    {
        // read next character
        c = fgetc(dictionary_file);

        /* CHECK */
        // if word/file ended
        if (c == '\n' || c == EOF)
        {
            words++;
			word[i] = '\0';

            // add to table
            if (Add_Word(word, dictionary_buffer) != 0) 
            {
                printf("\nError!\nCouldn't add word to buffer\n");
                return false;       // ERROR
            }

			i = 0;

            // exit if EOF
            if (c == EOF)
                break;
            else
                continue;
        }
        // if valid character
        else if (Is_Valid(c))
        { 	
            word[i]= c;		// add character to word
            i++;		// next char
        }
    }

    // close dictionary file
    fclose(dictionary_file);

    // success
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    return ( Delete_Node(dictionary_buffer) == 0)? true: false;
}
