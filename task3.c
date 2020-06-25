/*
Author: Akdeniz Kutay Ocal
Title: Task3 
Description: Sequential Execution - Multiple Queries
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
#define BUFFER 500

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
void readFromFile();

struct TrieNode *root;

int main()
{

    clock_t begin = clock();

    char dataFiles[10][12] = {"data/data1", "data/data2",
                              "data/data3", "data/data4", "data/data5", "data/data6",
                              "data/data7", "data/data8", "data/data9", "data/data10"};


    root = getNode();

    int fileIndex = 0;
    int lineCount = 0;

    FILE *fp;
    FILE *output;

    output = fopen("dictTask3.txt", "w");

    for (fileIndex; fileIndex < 10; fileIndex++)
    {
        fp = fopen(dataFiles[fileIndex], "r+");
        lineCount = 0;

        if (fp == NULL)
        {
            printf("Failed to open the file %s\n", *(dataFiles + fileIndex));
            break;
        }

        readFromFile(fp);
        fclose(fp);
    }

    unsigned char *str;
    str = (unsigned char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    printTrie(root, str, 0, output);
    fclose(output);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Task3 took %f seconds to execute.\n", time_spent);
}

int checkOccurence(unsigned char *s, unsigned char c)
{
    int i, count = 0;
    for (i = 0; s[i]; i++)
    {
        if (s[i] == c)
        {
            count++;
        }
    }
    return count;
}

void readFromFile(FILE *input)
{

    unsigned char *buffer; // string containing more than one queries
    unsigned char *temp;   // used to store leftover query part from prevous buffer

    bool isTempEmpty = true;     // condition of having leftovers from previous buffer
    bool isCompleteLine = false; // condition of having all queries without leftovers
    bool isFirstNew = false;     // condition of having first char '\n'

    buffer = (unsigned char *)calloc(BUFFER + 1, sizeof(unsigned char));
    temp = (unsigned char *)malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);

    while (fread(buffer, sizeof(unsigned char), BUFFER, input))
    {
        isCompleteLine = false;
        isFirstNew = false;

        if (buffer[0] == '\n')
        {
            //query in temp is complete
            insert(root, temp);
            free(temp);
            isTempEmpty = true;
            isFirstNew = true;
        }
        int numberOfQueries = checkOccurence(buffer, '\n');

        if (buffer[strlen(buffer) - 1] == '\n')
        {
            //last char is \n meaning it is complete query
            if (numberOfQueries == 1 && !isTempEmpty)
            {
                buffer[strlen(buffer) - 1] = '\0';
                strcat(temp, buffer);
                insert(root, temp);
                free(temp);
                isTempEmpty = true;
                continue;
            }
            else
            {
                buffer[strlen(buffer) - 1] = '\0';
                isCompleteLine = true;
            }
        }

        if (numberOfQueries == 0)
        {
            // if buffer is too small add it to temp
            
            if (isTempEmpty)
            {
                temp = (unsigned char *)malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
                strcpy(temp, "");
            }
            strcat(temp, buffer);
            isTempEmpty = false;
            continue;
        }

        if (isCompleteLine)
        {

            unsigned char *token = strtok(buffer, "\n");
            while (token != NULL)
            {
                //seperating buffer to queries seperated by '\n'

                if (!isTempEmpty) //there is chars form last buffer
                {
                    strcat(temp, token);
                    insert(root, temp);
                    free(temp);
                    isTempEmpty = true;
                }

                else
                {
                    insert(root, token);
                }

                token = strtok(NULL, "\n");
            }
        }

        else // there is leftovers
        {
    
            if (isFirstNew)
            {

                numberOfQueries--;
                isFirstNew = false;
            }
            
            unsigned char *token = strtok(buffer, "\n");
            int counter = 0;

            while (token != NULL)
            {
                //seperating buffer to queries seperated by '\n'

                if (counter >= numberOfQueries)
                {
                    //last query which is not completed
                    // add it to the temp

                    temp = (unsigned char *)malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
                    strcpy(temp, "");
                    strcpy(temp, token);
                    isTempEmpty = false;
                }
                else
                {
                    if (!isTempEmpty) //there is chars form last buffer
                    {
                        strcat(temp, token);
                        insert(root, temp);
                        free(temp);
                        isTempEmpty = true;
                    }

                    else
                    {
                        insert(root, token);
                    }

                    counter++;
                }

                token = strtok(NULL, "\n");
            }
        }
        free(buffer);
        buffer = (unsigned char *)calloc(BUFFER + 1, sizeof(unsigned char));
    }
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
