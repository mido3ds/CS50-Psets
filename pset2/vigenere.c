#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check user entered k value as argu.
    if (argc != 2)
    {
        //no argument, kill program
        printf("please, execute the program again and enter an argument value");
        return 1;
    }
    
    string k = argv[1];
    //make sure not includes digits
    for (int i = 0; i < strlen(k); i++)
        if (isdigit(k[i]))
        {
            printf("Uncorrect keyword, please try again!");
            return 1;
        }
    
    char p;
    string sentence = GetString();
    
    //go through all letters
    for (int i = 0, k_index = 0; i < strlen(sentence); i++)
    {
        //get the character
        p = sentence[i];
        
        //check whether it is a digit or not a letter
        //conditions after || to check it isn't a capital letter or small one
        //if something from those is true, so print that char unchanged
        if (isdigit(p) || (!isupper(p) && !islower(p)))
        {
            printf("%c", p);
            continue;
        }
        
        
        //rotate
        int c;
        if (isupper(p))
            c = (p + toupper(k[k_index]) - 2*65) % 26 + 65;
        else
            c = (p + tolower(k[k_index]) - 2*97) % 26 + 97;
        
        
        //move to the next key
        if (k_index == strlen(k) - 1)
        {
            k_index = 0;
        }
        else
        {
            k_index++;
        }
        
        printf("%c", c);
    }
    
    printf("\n");
    return 0;
}