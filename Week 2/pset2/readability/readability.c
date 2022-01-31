#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

float formula(int words, int letters, int punctuation);

int main(void)
{
    string input = get_string("Input text: ");

    // Get amount of characters per input (including \0)
    int n = strlen(input);
    
    // Introduction of variables
    int words = 0;
    int letters = 0;
    int punctuation = 0;

    // Check every character
    for (int i = 0; i < n; i++)
    {
        // If it is a letter
        if (isalpha(input[i]) != 0)
        {
            letters ++;
        }
        else
        {
            // If it is a space or the last letter
            if (input[i] == ' ' || input[i + 1] == '\0')
            {
                words ++;
            }    
            
            // If it is the end of a sentence
            if (input[i] == '.' || input[i] == '?' || input[i] == '!')
            {
                punctuation ++;
            }
        }
    }

    // Get index from function
    float index = formula(words, letters, punctuation);
    
    // Round value
    int grade = round(index);
    
    // Print result
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

float formula(int words, int letters, int punctuation)
{
    float L = letters / (float) words * 100.0;
    float S = punctuation / (float) words * 100.0;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    
    return index;
}