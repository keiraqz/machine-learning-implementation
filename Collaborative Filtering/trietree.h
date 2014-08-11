/**
 * @file trietree.h
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */

#ifndef TRIETREE_H
#define TRIETREE_H

#include <stdlib.h>
#include <string.h>



class TrieTree
{ 
public:
	int _size;
	void *_root;
	TrieTree();
	~TrieTree();
	void Release(void *ptr);
	int Insert(const char *ptr);
	int Search(const char *ptr);
	int Size();		
};


#endif