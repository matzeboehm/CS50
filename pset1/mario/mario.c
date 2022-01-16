#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get user input on height of pyramid
    int n;
    do
    {
        n = get_int("Level of Pyramid: ");
    }
    while (n < 1 || n > 8);
    
    // Iterate Row
    for (int i = 0; i < n; i++)
    {
        // Iterate column
        for (int j = 0; j < n; j++)
        {
            // Check whether to output # or blank
            if ((n - 1 - j) <= i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    
}