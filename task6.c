/*
Author: Akdeniz Kutay Ocal
Title: Task6 
Description: Completely Memory-Based Dictionary Creation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

// Alphabet size (# of symbols) from ' ' to all extended chars
#define ALPHABET_SIZE (224)

// Converts key current character into index
// the first ascii char is 032 which is whitespace
#define CHAR_TO_INDEX(c) ((int)c - (int)' ')

#define MAX_LINE_LENGTH 1000

// Trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    bool isEndOfWord; // if the node represents end of a word
    int frequency;    // number of times a query occured
};

struct TrieNode *getNode();
void insert();
bool search();
bool isLeafNode();
void printTrie();

int main()
{

    clock_t begin = clock();

    char dataFiles[10][12] = {"data/data1", "data/data2",
                              "data/data3", "data/data4", "data/data5", "data/data6",
                              "data/data7", "data/data8", "data/data9", "data/data10"};

    struct TrieNode *root = getNode();

    int fileIndex = 0;

    FILE *fp;
    FILE *output;

    output = fopen("dictTask6.txt", "w");

    for (fileIndex; fileIndex < 10; fileIndex++)
    {
        fp = fopen(dataFiles[fileIndex], "r");

        if (fp == NULL)
        {
            printf("Failed to open the file %s\n", *(dataFiles + fileIndex));
            break;
        }

        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        unsigned char *buffer; // string containing more than one queries

        buffer = (unsigned char *)calloc(fsize+1, sizeof(unsigned char));
        fread(buffer, sizeof(unsigned char), fsize, fp);
        unsigned char *token = strtok(buffer, "\n");

        while (token != NULL)
        {
            //seperating buffer to queries seperated by '\n'
            insert(root, token);
            token = strtok(NULL, "\n");
        }

        //free(buffer);
        fclose(fp);
    }

    unsigned char *str;
    str = (unsigned char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    printTrie(root, str, 0, output);
    fclose(output);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Task6 took %f seconds to execute.\n", time_spent);
}

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;

    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode)
    {
        int i;

        pNode->isEndOfWord = false;
        pNode->frequency = 0;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, unsigned char *key)
{

    //printf("%s\n",key);
    int level;
    int length = strlen(key);
    int index;

    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    // increase the frequency
    pCrawl->frequency = (pCrawl->frequency) + 1;
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, unsigned char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

// function to check if current node is leaf node or not
bool isLeafNode(struct TrieNode *root)
{
    return root->isEndOfWord != false;
}

void printTrie(struct TrieNode *root, unsigned char *str, int level, FILE *output)
{

    if (isLeafNode(root))
    {
        str[level] = '\0';
        fprintf(output, "%s\t%d\n", str, root->frequency);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            str[level] = i + ' ';
            printTrie(root->children[i], str, level + 1, output);
        }
    }
}