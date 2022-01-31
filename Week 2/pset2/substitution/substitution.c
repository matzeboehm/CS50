#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Encryption Key has not been provided.\n");
        return 1;
    }
    else
    {
        string key = argv[1];
        int len = strlen(key);
        char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
        
        if (len != 26)
        {
            printf("Length of key is wrong.\n");
            return 1;
        }
        else
        {
            // set all keyes to  lowercase
            for (int i = 0; i < len; i++)
            {
                key[i] = tolower(key[i]);
            }
            
            // check if key is only alphabetical and if letters appear only once
            for (int i = 0; i < len; i++)
            {
                // if not every char in key is alphabetical
                if (isalpha(key[i]) == 0)
                {
                    printf("Not every char is alphabetical. \n");
                    return 1;
                }
                
                // Look if every char of key appears only once in alphabet
                int in = 0;
                for (int j = 0; j < len; j++)
                {
                    if (key[j] == alphabet [i])
                    {
                        in ++;
                    }
                }
                
                // if letter appears more than once or not at all an error is thrown
                if (in != 1)
                {
                    printf("Not every letter in the key appears only once.\n");
                    return 1;
                }
            }
        }
    
        // If code reaches here input key has been validated
        // input text
        string input = get_string("plaintext: ");
        int inputlen = strlen(input);
    
        // set output to input, so the character length is correct and punctuation is correct
        string output = input;
    
        // go trhough every char
        for (int i = 0; i < inputlen; i++)
        {
            // check if not alpha
            if (isalpha(input[i]) == 0)
            {
                output[i] = input[i];
            }
        
            // if it is alphabetic
            else
            {
                int pos = 0;
                    
                // go through alphabet
                for (int j = 0; j < len; j++)
                {
                    if (tolower(input[i]) == alphabet[j])
                    {
                        pos = j;
                    }  
                }
            
                // substitute after checking lettercase
                // if lower case
                if (isupper(input[i]) == 0)
                { 
                    output[i] = key[pos];
                }
                // if upper case
                else
                {
                    output[i] = toupper(key[pos]);
                }
            }
        }
        
        printf("ciphertext: %s\n", output);
        return 0;
    }
}