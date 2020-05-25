#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

// Alphabet size (# of symbols) from ' ' to all extended chars
#define ALPHABET_SIZE (224)

// Converts key current character into index
// the first ascii char is 032 which is whitespace
#define CHAR_TO_INDEX(c) ((int)c - (int)' ')

// Trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    bool isEndOfWord; // if the node represents end of a word
    int frequency;    // number of times a query occured
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;

    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode)
    {
        int i;

        pNode->isEndOfWord = false;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, unsigned char *key)
{
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

bool isLeafNode(struct TrieNode *root)
{

    if (root->isEndOfWord)
        return true;

    else
        return false;
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