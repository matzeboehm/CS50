// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Init of some values
char *alphabet = "abcdefghijklmnopqrstuvwxyz";
int wordcount = 0;
int hashvalue;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    hashvalue = hash(word);

    // Init temp used for loop
    //node *temp = malloc(sizeof(node));
    //if (temp == NULL)
    //{
    //    free(temp);
    //    return 0;
    //}
    node *temp = table[hashvalue];

    // Continue until last node reached
    while (temp != NULL)
    {
        // if input word matches word from temp file return true
        if (strcasecmp(word, temp->word) == 0)
        {
            //free(temp); // This seems to cause an error
            return true;
        }
        // else go to next node
        else
        {
            temp = temp->next;
        }
    }
    //free(temp); // This seems to cause an error
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    for (int i = 0; i < N; i++)
    {
        if (tolower(word[0]) == alphabet[i])
        {
            return i;
        }
    }
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 1];

    // Scan file until end of file is reached and copy string into buffer
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copy string from fscanf into word argument of n and increase wordcount
        strcpy(n->word, buffer);
        wordcount++;

        // Add node to the front of the list at table
        hashvalue = hash(buffer);
        n->next = table[hashvalue];
        table[hashvalue] = n;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Go from hash a to z in table
    for (int i = 0; i < N; i++)
    {
        // Init temp used for loop
        //node *n = malloc(sizeof(node));
        //if (n == NULL)
        //{
        //    return 0;
        //}
        node *n = table[i];

        //node *temp = malloc(sizeof(node));
        //if (temp == NULL)
        //{
        //    return 0;
        //}
        node *temp;

        // As long as temp is not empty
        while (n != NULL)
        {
            // Like with load, go from first node to second, ... and delete in that order
            temp = n;
            n = n->next;
            free(temp);
        }
    }
    return true;
}