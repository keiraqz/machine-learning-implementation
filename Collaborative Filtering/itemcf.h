/**
 * @file itemcf.h
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */

#ifndef ITEMCF_H
#define ITEMCF_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "trietree.h"



class ItemCF
{ 
	public:
		int _items_size;
    	double *_sim_matrix;
		ItemCF();
		~ItemCF();
		double Similarity(std::vector<int> * userAratings, std::vector<int> * userBratings);
		bool Train(const char *input, const char *output);
		int Index(const int a, const int b)
   		{
   		    return (a+1) * (a+2) / 2 - a - 1 + b ;
  		}

};


#endif