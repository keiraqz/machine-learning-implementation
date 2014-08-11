/**
 * @file itemcf.cpp
 * @brief  
 * @author yekeren
 * @version 
 * @date 2014-01-26
 *
 * @modified
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */
#include <time.h>
#include "itemcf.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <sstream>
#include "trietree.h"
 
/**
 * @brief construct 
 */
ItemCF::ItemCF():
    _items_size(0),
    _sim_matrix(NULL)
{
}
 
/**
 * @brief destruct 
 */
ItemCF::~ItemCF()
{
    if (_sim_matrix) {
        free(_sim_matrix);
        _sim_matrix = NULL;
    }
}
 
/**
 * @brief train itemcf simmatrix 
 * @param input
 * @param output
 * @return   
 */
double ItemCF::Similarity(std::vector<int> * userAratings, std::vector<int> * userBratings)
{
    
    std::vector<int> ratingA = *userAratings;
    std::vector<int> ratingB = *userBratings;
    double sumA, sumB, meanA, meanB, sumA2, sumB2, sumAB, sumAbsAminusB, sumAbsAB = 0.0;
    int numAB = 0;
    int noUsers = ratingA.size();

    for (int i = 0; i < noUsers; i++) {
        if (ratingA[i] != 0 && ratingB[i] !=0 ) {
            numAB ++;
            sumA += ratingA[i];
            sumB += ratingB[i];

            sumA2 += ratingA[i] * ratingA[i];
            sumB2 += ratingB[i] * ratingB[i];
            sumAB += ratingA[i] * ratingB[i];
            sumAbsAminusB += std::abs (ratingA[i] - ratingB[i]);
            sumAbsAB += std::abs (ratingA[i] * ratingB[i]);
        }
    }

    meanA = sumA/numAB;
    meanB = sumB/numAB;

    // Correlation
    double top = sumAB - meanB * sumA - meanA * sumB + meanB * meanA * numAB;
    double partA = sumA2 + meanA*meanA*numAB - 2 * meanA * sumA;
    double partB = sumB2 + meanB*meanB*numAB - 2 * meanB * sumB;
    double bottom = 0.0;
    if (partA > 0 && partB > 0) {
        bottom = sqrt(partA) * sqrt(partB);
    } else {
        bottom = 0;
    }
     double result = 0.0;

    if (bottom != 0) {
        result = top/bottom;
    } else {
        result = 0;
    }
    std::cout << result << "\n";
    return result;

    // // Hamming Distance
    // result = 1-sumAbsAminusB/(2*numAB);
    // return result;

    
}

bool ItemCF::Train(const char *input, const char *output)
{
    /* check file */
    FILE *fpin = fopen(input, "r");
    if (!fpin) {
        fprintf(stderr, "open input file failed! errno=%d\n", errno);
        return false;
    }
 
    FILE *fpout = fopen(output, "w");
    if (!fpout) {
        fprintf(stderr, "open output file failed! errno=%d\n", errno);
        fclose(fpin);
        return false;
    }
 
    /**
     * Step 1:
     * Create user-items data structure
     * Create item-pop data structure
     */
    std::map<int, std::set<int>*> mapUserItems;
    std::map<int, std::vector<int>* > mapItemPop; // modified to vector
    std::map<int, std::string> mapItemRawId;
 
    const char *split = ",";
    char line[4096] = { 0 };
 
    TrieTree *userDct = new TrieTree();
    TrieTree *itemDct = new TrieTree();
    int lineNo = 1;

    while (fgets(line, sizeof(line) - 1, fpin) != NULL) {
        int userId, itemId, rating, col = 0;
 
        char *str = line;
        char *ptr = strstr(str, split);
 
        while (ptr) {
            *ptr = '\0';
            if (col == 0) {
                userId = userDct->Insert(str);
            } else if (col == 1) {
                itemId = itemDct->Insert(str);
                mapItemRawId.insert(std::pair<int, std::string>(itemId, str));

            } 
            ++col;
            str = ptr + strlen(split); // str point to current ","
            ptr = strstr(str, split); // ptr find first occurance of "," in current line (substring of line)
        }

        { // Get the rating
            std::istringstream convert(str); 
            if ( !(convert >> rating) ) //give the value to 'Result' using the characters in the stream
                rating = 0;
            //std::cout << rating << "\n";
        }

        {
            /* user-items */
            std::map<int, std::set<int>*>::iterator iter;
            std::set<int> *items = NULL;
 
            if ((iter = mapUserItems.find(userId)) != mapUserItems.end()) {
                items = iter->second;
            } else {
                items = new std::set<int>;
                mapUserItems.insert(std::pair<int, std::set<int>*>(userId, items));
            }
            items->insert(itemId);
        }

        {
            /* item-pop */
            //  MODIFIED: change second "int" into a vector and store all the user ratings in order
            std::map<int, std::vector<int>* >::iterator iter;
            std::vector<int> * ratings = NULL;
 
            if ((iter = mapItemPop.find(itemId)) != mapItemPop.end()) {
                ratings = iter->second;
                if (userId >= (*ratings).size()) {
                    (*ratings).resize(userDct->Size());
                }
                (*ratings).at(userId) = rating;
            } else {            
                ratings = new std::vector<int> (userDct->Size());
                (*ratings).at(userId) = rating;             
                mapItemPop.insert(std::pair<int, std::vector<int>* >(itemId, ratings));
            }
        }

        lineNo++;
    }

 
    _items_size = itemDct->Size();
    fprintf(stderr, "user total: %d\n", userDct->Size());
    fprintf(stderr, "item total: %d\n", _items_size);

    delete userDct;
    delete itemDct;
 
    /**
     * Step 2:
     * Create item-item similarity matrix
     */

    int size = _items_size * (_items_size + 1) / 2;
    if (_sim_matrix) { 
        free(_sim_matrix); 
    }

    _sim_matrix = (double*)malloc(sizeof(double) * size);

    long long progress = 0;
    long long step = mapUserItems.size() / 100 * 13;
 

    std::map<int, std::set<int>*>::iterator iter;

    for (iter = mapUserItems.begin(); iter != mapUserItems.end(); ++iter) {
        std::set<int> &items = *iter->second;
        std::set<int>::iterator iter_i, iter_j;
 
        for (iter_i = items.begin(); iter_i != items.end(); ++iter_i) {
            for (iter_j = iter_i; iter_j != items.end(); ++iter_j) {
                _sim_matrix[this->Index(*iter_i, *iter_j)] += 1.0;
            }
        }
        if (++progress % step == 0) {
            fprintf(stderr, "generating simmatrix: %lld%%\r", progress * 100 / mapUserItems.size());
        }
    }
    fprintf(stderr, "generating simmatrix: %lld%%\n", progress * 100 / mapUserItems.size());
    for (iter = mapUserItems.begin(); iter != mapUserItems.end(); ++iter) {
        delete iter->second;
    }
    

    /**
     * Step 3:
     * Export to output
     */

    progress = 0;
    step = _items_size * (1 + _items_size) * 13 / 200;
    
    time_t time1 = time(NULL);

    fprintf(fpout, "%d\n", _items_size);
    for (int i = 0; i < _items_size; ++i) {
        for (int j = i + 1; j < _items_size; ++j) {
            int index = this->Index(i, j);
            if (_sim_matrix[index] > 0) {

                //_sim_matrix[index] /= sqrt(mapItemPop[i] * mapItemPop[j]);
                // MODIFY THIS: Add another similarity computing function
                _sim_matrix[index] = this->Similarity(mapItemPop[i], mapItemPop[j]);

                //fprintf(fpout, "[%s,%s],%f\n", mapItemRawId[i].c_str(), mapItemRawId[j].c_str(), _sim_matrix[index]);
            }
            if (++progress % step == 0) {
                fprintf(stderr, "writing to output: %lld%%\r", progress * 200 / (_items_size * (_items_size - 1)));
            }
        }
    }
    fprintf(stderr, "writing to output: %lld%%\n", progress * 200 / (_items_size * (_items_size - 1)));
    time_t time2 = time(NULL);
    fprintf(stderr, "Time for computing similarity: %lds\n", time2 - time1);


    fclose(fpin);
    fclose(fpout);
 
    return true;
}


