/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "dictionary.h"

// pointer to the dictionary
FILE* dictionary_file= NULL;
unsigned long fsize = 0;        // file size

// store dictionary text
char* dictionary_buffer = NULL;


/**
 * Returns true if word is in dictionary else false.
 * uses binary search
 */
bool check(const char* word)
{
    char* dictionary_word = NULL;         // pointer to word from dictionary
    unsigned long first = 0, last = fsize - 1, middle = 0;    // indexes

    while (true)
    {
        // update middle
        middle = (first + last) / 2;

        // get word in dictionary
        dictionary_word = Get_Word(middle);
        if (dictionary_word == NULL)
        {
            printf("\nError,something wrong happened!\n");
            return false;
        }

        // compare words
        for (int i = 0; ;i++)
        {
            // they equall when dictionary_word and given_word comes to end successfully
            if (dictionary_word[i] == '\n' && i == strlen(word))
            {
                return true;
            }
            else if (tolower(word[i]) > dictionary_word[i])
            {
                // search in the lower half
                first = (last == first + 1)? last : middle;     
                break;
            }
            else if (tolower(word[i]) < dictionary_word[i])
            {
                // search in the upper half
                last = (last == first + 1)? first : middle;    
                break;
            }
        }

        if (first > last)      // binary search ended 
            return false;
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // open dictionary
    dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        printf("\nCouldn't open dictionary!\n");
        return false;
    }

    // get file size    
    unsigned long fsize = Get_File_Size(dictionary_file);   
    if (fsize == 0)     // empty
    {
        printf("\nError, couldn't get dictionary size!\n");
        return false;
    }

    // allocate memory for that size
    dictionary_buffer = malloc(fsize);
    if (dictionary_buffer == NULL)
    {
        printf("\nError, Couldn't allocate memory!\n");
        return false;
    }

    // read to buffer
    if ( fread(dictionary_buffer, fsize, 1, dictionary_file) == 0 )
    {
        printf("\nCpuldn't read dictionary!\n");
        return false;
    }

    // close dictionary file
    fclose(dictionary_file);

    // success
    return true;
}

/** 
* returns Size of file
* file must be open
*/
unsigned long Get_File_Size(FILE* f)
{
    struct stat buf;    // A structure where data about the file will be stored
    int fd = fileno(f);

    // c system call to the file to get its info, returns negative on failure 
    if (fstat(fd, &buf) < 0)
    {
        printf("\nError, couldn't get file size!\n");
        return 0;
    }
    fsize = buf.st_size;
    return fsize;
}

/*
* takes middle value and returns pointer to 
* the first char in the middle line
* used for check fnc.
*/
char* Get_Word(int middle)
{
    char* word = NULL;
    do
    {
        word = dictionary_buffer + middle;      // points at middle of buffer
        middle--;           // we get back one char and check it
    }
    while (word[0] != '\n');

    // return it back to first char in word after \n
    middle += 2;

    // store address of that char
    word = dictionary_buffer + middle;

    return word;        // return that address
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    unsigned int words = 0;
    for (int i = 0; i < fsize; i++)
        if (dictionary_buffer[i] == '\n')
            words++;

    // include the last line, as it does't end with \n
    if (words > 0)
        words++;

    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    free(dictionary_buffer);
    return true;
}
