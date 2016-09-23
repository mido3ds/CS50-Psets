#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int main(void)
{
    string name = GetString();
    char character = toupper(name[0]);
    printf("%c", character);
    
    //iterate through name characters
    for (int i = 1; i < strlen(name); i++)
    {
        //check to avoid exceptions
        bool is_last_index = i == strlen(name) - 1;
        
        //get characters after spaces
        if (name[i] == ' ' && !is_last_index)
        {
            character = toupper(name[i + 1]);
            printf("%c", character);
        }
    }
    printf("\n");
    
}