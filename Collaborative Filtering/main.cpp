/**
 * @file main.cpp
 * @brief  
 * @author yekeren
 * @version 1.0.0
 * @date 2014-01-26
 */
 
#include <time.h>
#include <stdio.h>
#include "itemcf.h"
 
int main(int argc, char *argv[])
{
    time_t startTime = time(NULL);
 
    ItemCF *itemcf = new ItemCF();
 
    itemcf->Train("movie1m_1to5_train.dat", "output.txt");
    delete itemcf;
 
    time_t endTime = time(NULL);
    fprintf(stderr, "job finished, cost=%lds\n", endTime - startTime);
    return 0;
}