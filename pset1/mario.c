#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    do
    {
        printf("height: ");
        height = GetInt();
    }while ((height < 0 || height > 23));
    
    for (int row = 1; row <= height; row++)
    {
        for (int space = height - row; space > 0; space--)
            printf(" ");
        for (int sharp = row + 1; sharp > 0 ; sharp--)
            printf("#");
        printf("\n");
    }
}
