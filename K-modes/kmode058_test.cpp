/**
 * @modified
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */

# include <cstdlib>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <time.h>

# include "kmode058.h"

using namespace std;

int main ( );
void test01 (Kmode *clustring);

//****************************************************************************80

int main ( )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for kmode058_test.
//
//  Discussion:
//
//    kmode058_test tests the K-mode algorithm.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Modified:
//
//    11 August 2014
//
//  Author:
//
//    Qi Zhou

{
  Kmode *clustring = new Kmode();

  clustring -> timestamp ( );
  cout << "\n";
  cout << "K-modes modified from ASA058_PRB:\n";
  cout << "  C++ version\n";
  cout << "  Test the implementation.\n";

  test01 (clustring);
//
//  Terminate.
//
  cout << "\n";
  cout << "K-modes:\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
  clustring -> timestamp ( );

  return 0;
}
//****************************************************************************80

void test01 (Kmode *clustring)

//****************************************************************************80
//
//  Purpose:
//
//    TEST01 tries out the ASA058 routine.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Modified:
//
//    11 August 2014
//
//  Author:
//
//    Qi Zhou
{

// Soybean-small
# define K 4
# define M 35
# define N 47
  // Precision: 81.15% 

//  // Ads nonAds (2821 nonads, 458 ads) (Got: 3240, 39)
// # define K 2
// # define M 1555
// # define N 3279
//   // Precision: 93.5%

// // MINST
// # define K 10
// # define M 784
// # define N 10000
  // Time: 5:07 on 2 nodes 20GB memory UVA FIR

// Mushroom
// # define K 2
// # define M 22
// # define N 8124


  int b[N];
  double d[K*M];
  double dev[K];
  double dev_sum;
  int e[K];
  int e_sum;
  double f[N];
  int i;
  ifstream input;
  char *input_filename = "soybean-small.txt";
  //char *input_filename = "ad.txt";
  int j;
  int k2;
  int nz;
  double x[N*M];

  cout << "\n";
  cout << "TEST01\n";
  cout << "  Test the CLUSTR algorithm.\n";
  cout << "  Applied Statistics Algorithm 58\n";
//
//  Read the data.
//
  cout << "\n";
  cout << "  Reading the data.\n";

  input.open ( input_filename );

  if ( !input )
  {
    cout << "\n";
    cout << "TEST01 - Fatal error!\n";
    cout << "  Could not open the input file: \"" << input_filename << "\"\n";
    return;
  }

  for ( i = 1; i <= N; i++ )
  {
    for ( j = 1; j <= M; j++ )
    {
      input >> x[i-1+(j-1)*N];
    }
  }

  input.close ( );
//
//  Print a few data values.
//
  cout << "\n";
  cout << "  First 5 data values:\n";
  cout << "\n";

  for ( i = 1; i <= 5; i++ )
  {
    cout << "  " << setw(8) << i;
    for ( j = 1; j <= M; j++ )
    {
      cout << "  " << setw(14) << x[i-1+(j-1)*N];
    }
    cout << "\n";
  }
//
//  Initialize the cluster centers arbitrarily.
//
  for ( i = 1; i <= K; i++ )
  {
    for ( j = 1; j <= M; j++ )
    {
      d[i-1+(j-1)*K] = x[i-1+(j-1)*N];
    }
  }
    // for ( int kk = 0; kk < K * M; kk++ )
    // {
    //   d[kk] = x[kk];
    // }

//
//  Compute the clusters.
//
  nz = 1;
  k2 = K;

  time_t startTime = time(NULL);
  clustring -> clustr ( x, d, dev, b, f, e, N, M, K, nz, k2 );
  time_t endTime = time(NULL);

  cout << "\n";
  cout << "  Cluster  Population  Energy\n";
  cout << "\n";

  for ( i = 1; i <= K; i++ )
  {
    cout << "  " << setw(8) << i
         << "  " << setw(8) << e[i-1]
         << "  " << setw(14) << dev[i-1] << "\n";
  }

  e_sum = 0;
  dev_sum = 0.0;

  for ( i = 1; i <= K; i++ )
  {
    e_sum = e_sum + e[i-1];
    dev_sum = dev_sum + dev[i-1];
  }

  cout << "\n";
  cout << "  " << "   Total"
       << "  " << setw(8) << e_sum
       << "  " << setw(14) << dev_sum << "\n";

  cout << "job finished, cost=: " << (endTime - startTime) <<"\n";
  cout << "Cluster Assigned: \n{";
    for (int i = 0; i < N; i++) {
      cout << b[i] << " ";
    }
  cout << "}\n";


  return;
# undef K
# undef M
# undef N
}
