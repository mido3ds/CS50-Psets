#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check user entered k value as argu.
    if (argc == 1)
    {
        //no argument, kill program
        printf("please, execute the program again and enter an argument value");
        return 1;
    }
    
    int k = atoi(argv[1]); 
    char p;
    string sentence = GetString();
    
    //go through all letters
    for (int i = 0; i < strlen(sentence); i++)
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
            c = (p + k - 65) % 26 + 65;
        else
            c = (p + k - 97) % 26 + 97;
            
        printf("%c", c);
    }
    
    
    printf("\n");
    return 0;
}