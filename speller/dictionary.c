// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17565;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Getting the hash value for the word
    unsigned int hash_val = hash(word);

    // Checking for the word in that linked list at index of hash_val in table
    for (node *temp = table[hash_val]; temp != NULL; temp = temp->next)
    {
        if (strcasecmp(word, temp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // values according to first letter of the word 
    // unsigned int index = toupper(word[0]) - 'A';

    unsigned int index = 0;

    int n = (strlen(word) > 3) ? 3 : strlen(word);

    for (int i = n - 1; i >= 0; i--)
    {
        unsigned int letter_en = toupper(word[i]) - 'A';
        index += letter_en * pow(26, n - 1 - i);
    }

    return index % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opening the dictionary file to read from
    FILE *ptr_dictionary = fopen(dictionary, "r");

    // Temporary variable to store words from dictionary file into
    char new_word[LENGTH + 1];
    while (fscanf(ptr_dictionary, "%s", new_word) != EOF)
    {
        // node for new word read from dictionary file
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }

        // Hash value for the word
        unsigned int hash_val = hash(new_word);

        // copying new word to this node
        strcpy(new->word, new_word);
        
        // adding the new node to the starting of hash table element       
        new->next = table[hash_val];
        table[hash_val] = new;
    }

    // Closing the dictionary
    fclose(ptr_dictionary);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (node *temp = table[i]; temp != NULL; temp = temp->next)
        {
            count++;
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return true;
}
