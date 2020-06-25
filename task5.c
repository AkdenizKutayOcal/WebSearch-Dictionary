/*
Author: Akdeniz Kutay Ocal
Title: Task5 
Description: Threaded Execution - Multiple Tries
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

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
void *readFromFile();
void mergeDictionaries();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char dataFiles[10][12] = {"data/data1", "data/data2",
                          "data/data3", "data/data4", "data/data5", "data/data6",
                          "data/data7", "data/data8", "data/data9", "data/data10"};


struct TrieNode *rootList[10];
struct TrieNode *finalRoot;

int main()
{

    clock_t begin = clock();

    pthread_t threadList[10];

    for (int i = 0; i < 10; i++)
    {
        rootList[i] = getNode();
    }
    int fileIndex = 0;

    for (fileIndex; fileIndex < 10; fileIndex++)
    {

        int rc = pthread_create(&threadList[fileIndex], NULL, readFromFile, (void *)fileIndex);
        if (rc) /* could not create thread */
        {
            printf("\n ERROR: return code from pthread_create is %d \n", rc);
            exit(1);
        }
        //printf("\n Created new thread (%lu) ... \n", threadList[fileIndex]);
    }

    fileIndex = 0;
    for (fileIndex; fileIndex < 10; fileIndex++)
    {
        pthread_join(threadList[fileIndex], NULL);
    }

    finalRoot = getNode();

    unsigned char *str;
    str = (unsigned char *)malloc(sizeof(char) * MAX_LINE_LENGTH);

    for (int i = 0; i < 10; i++)
    {
        mergeDictionaries(rootList[i], str, 0);
        free(rootList[i]);
    }

    FILE *output;
    output = fopen("dictTask5.txt", "w");

    printTrie(finalRoot, str, 0, output);
    fclose(output);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Task5 took %f seconds to execute.\n", time_spent);
}

void *readFromFile(void *index)
{

    int fileIndex = (int)index;
    unsigned char *line;
    line = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);

    FILE *fp;
    fp = fopen(dataFiles[fileIndex], "r+");

    if (fp == NULL)
    {
        printf("Failed to open the file\n");
        return NULL;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {

        line[strlen(line) - 1] = '\0';
        insert(rootList[fileIndex], line,0);
    }

    free(line);
    fclose(fp);
    pthread_exit(NULL);
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
void insert(struct TrieNode *root, unsigned char *key, int mergeOccurence)
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
    if (mergeOccurence > 0)
    {
        pCrawl->frequency = (pCrawl->frequency) + mergeOccurence;
    }
    else
    {
        pCrawl->frequency = (pCrawl->frequency) + 1;
    }
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

void mergeDictionaries(struct TrieNode *root, unsigned char *str, int level)
{
    if (isLeafNode(root))
    {
        str[level] = '\0';
        insert(finalRoot, str,(root->frequency));
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            str[level] = i + ' ';
            mergeDictionaries(root->children[i], str, level + 1);
        }
    }
}