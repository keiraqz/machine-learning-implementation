/**
 * @file trietree.cpp
 * @brief  
 * @author yekeren
 * @version 1.0.0
 * @date 2014-01-26
 *
 * @modified
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */
 
#include "trietree.h"
#include <stdlib.h>
#include <string.h>
 
#define INVALID_INDEX -1
 
struct TrieNode {
    struct TrieNode *links[256];    /* links of the trie node */
    int index;                      /* index of the trie node */
};
 
/**
 * @brief construct 
 */
TrieTree::TrieTree():
    _size(0),
    _root(NULL)
{
    _root = malloc(sizeof(TrieNode));
 
    memset(_root, 0, sizeof(TrieNode));
    ((TrieNode*)_root)->index = INVALID_INDEX;
}
 
/**
 * @brief destruct 
 */
TrieTree::~TrieTree()
{
    this->Release(_root);
 
    _root = NULL;
    _size = 0;
}
 
/**
 * @brief release nodes recursively
 * @param ptr
 */
void TrieTree::Release(void *ptr)
{
    TrieNode *node = NULL;
 
    if ((node = (TrieNode*)ptr) != NULL) {
        for (int i = 0; i < 256; ++i) {
            if (node->links[i]) {
                this->Release(node->links[i]);
            }
        }
        free(node);
    }
}
 
/**
 * @brief insert a word into trie tree 
 * @param ptr
 * @return   
 */
int TrieTree::Insert(const char *ptr)
{
    TrieNode *node = (TrieNode*)_root;
 
    while (*ptr != '\0') {
        unsigned char v = (unsigned char)*ptr;
 
        if (!node->links[v]) {
            node->links[v] = (TrieNode*)malloc(sizeof(TrieNode));
 
            memset(node->links[v], 0, sizeof(TrieNode));
            node->links[v]->index = INVALID_INDEX;
        }
        node = node->links[v];
        ++ptr;
    }
    if (INVALID_INDEX == node->index) {
        node->index = _size++;
    }
    return node->index;
}
 
/**
 * @brief search for word's index in the trie tree
 * @param ptr
 * @return   
 */
int TrieTree::Search(const char *ptr)
{
    TrieNode *node = (TrieNode*)_root;
 
    while (*ptr != '\0') {
        unsigned char v = (unsigned char)*ptr;
 
        if (!node->links[v]) {
            return INVALID_INDEX;
        }
        node = node->links[v];
        ++ptr;
    }
    return node->index;
}

int TrieTree::Size()
{

    return _size;
}