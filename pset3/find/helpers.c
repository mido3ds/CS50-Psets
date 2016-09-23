/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    //binary search
    int start = 0, end = n - 1, middle;
    while (true)
    {
        //get middle index
        middle = (end - start) / 2 + start;
        
        //is found
        if (value == values[middle])
            return true;

        //when value isn't in middle and end==start 
        //target value is not found
        else if (end == start)
            return false;

        //move to the right
        else if (value > values[middle])
        {
            //if start == middle, then make start = end
            //to do the last iteration
            if (start == middle)
                start = end;
            else 
                start = middle;
        }

        //move to the left
        else if (value < values[middle])
        {
            //if end == middle, then make end = start
            //to do the last iteration
            if (end == middle)
                end = start;
            else 
                end = middle;
        }
        
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    //selection sort, O(n^2)
    for (int i = 0; i < n; i++)
    {
        int minimum = values[i];
        for (int j = i; j < n; j++)
        {
            if (values[j] < minimum)
            {
                //swap numbers
                int temp = minimum;
                minimum = values[j];
                values[j] = temp;
            }
            values[i] = minimum;
        }
    }
    
}
