K-modes Clustering in C++
==========================

The algorithm is based on K-modes algorithm (Huang, 1998). The implementation is modifed from ASA058 C++ library of K-Means algorithm:

http://people.sc.fsu.edu/~jburkardt/cpp_src/asa058/asa058.html

How-to-use:

* Modify "kmode058_test.cpp" to include the datafile and specify the number of features, observations and clusters.
* Can only cluster categorical attributes. Each level needs to be coded into an integer.


The three main differences of K-means and K-modes:

1. K-modes is for categorical attributes

2. K-modes is using Hamming Distance instead of Euclidean Distance in K-means

3. K-modes choosing cluster centers using the mode of the members instead of the mean


Two testsets are provide:

1. Soybean-small dataset from UCI: https://archive.ics.uci.edu/ml/datasets/Soybean+(Small)

  The algorithm is tested and achieves 81.15% Precision.

2. Internet Ads dataset from UCI: https://archive.ics.uci.edu/ml/datasets/Internet+Advertisements

  3 continuous attributes are removed.

  The algorithm is tested and achieves 93.5% Precision.


Please include references when using the implementation and the datasets.

Huang, Z. (1998). Extensions to the k-means algorithm for clustering large data sets with categorical values. Data mining and knowledge discovery, 2(3), 283-304. Chicago
