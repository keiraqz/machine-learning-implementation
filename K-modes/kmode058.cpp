/**
 * @modified
 * @author keiraqz
 * @email keira.qzhou@gmail.com
 * @date 2014-08-11
 */

# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <map>


using namespace std;
# include "kmode058.h"

Kmode::Kmode()
{
}
 
/**
 * @brief destruct 
 */
Kmode::~Kmode()
{
}

//****************************************************************************80

void Kmode::clustr ( double x[], double d[], double dev[], int b[], double f[], 
  int e[], int i, int j, int n, int nz, int k )

//****************************************************************************80
//
//  Purpose:
//
//    CLUSTR uses the K-modes algorithm to cluster data.
//
//  Discussion:
//
//    Given a matrix of I observations on J variables, the
//    observations are allocated to N clusters in such a way that the
//    within-cluster sum of squares is minimised.
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
//    Original FORTRAN77 version by David Sparks.
//    C++ version by John Burkardt.
//
//  Modified: 
//  
//    11 August 2014
//
//  Author:
//
//    Qi Zhou
//
//
//  Reference:
//
//    David Sparks,
//    Algorithm AS 58:
//    Euclidean Cluster Analysis,
//    Applied Statistics,
//    Volume 22, Number 1, 1973, pages 126-130.
//
//  Parameters:
//
//    Input, double X[I*J], the observed data.
//
//    Input/output, double D[K*J], the cluster centers.
//    On input, the user has chosen these.  On output, they have been
//    updated.
//
//    Output, double DEV[K], the sums of squared deviations
//    of observations from their cluster centers.
//
//    Output, int B[I], indicates the cluster to which
//    each observation has been assigned.
//
//    Workspace, double F[I].
//
//    Output, int E[K], the number of observations assigned
//    to each cluster.
//
//    Input, int I, the number of observations.
//
//    Input, int J, the number of variables.
//
//    Input, int N, the number of clusters.
//
//    Input, int NZ, the minimum number of observations
//    which any cluster is allowed to have.
//
//    Input, int K, the maximum number of clusters.
//
{
  double big = 1.0E+10;
  double da;
  double db;
  double dc;
  double de;
  double fl;
  double fm;
  double fq;
  int ia;
  int ib;
  int ic;
  int id;
  int ie;
  int ig;
  int ih;
  int ii;
  int ij;
  int ik;
  int il;
  int in;
  int ip;
  int ir;
  int is;
  int it;
  int iu;
  int iw;
  int ix;
  int iy;
  map<int, int> m[j*n]; 

  for (int ia = 1; ia <= n; ia++ )
  {
    e[ia-1] = 0;
  }
//
//  For each observation, calculate the distance from each cluster
//  center, and assign to the nearest.
//
  for ( ib = 1; ib <= i; ib++ ) // i: number of observations
  {
    f[ib-1] = 0.0;
    da = big;

    for ( id = 1; id <= n; id++ ) // n: number of clusters
    {
      db = 0.0;
      for ( ie = 1; ie <= j; ie++ ) // j: number of features
      {
        dc = x[ib-1+(ie-1)*i] - d[id-1+(ie-1)*k]; // x: observations,  d: cluster center
          // Distance measure: Euclidean Distance: Each feature, x[i] - d[i]
        if (dc != 0) {
          db += 1;
        }
      }

      if ( db < da )
      {
        da = db;
        b[ib-1] = id; // b: indicate which cluster each observation has been assigned
      }
    }
    ig = b[ib-1];
    e[ig-1] ++; // e: number of observations assigned to each cluster
  }

//  *******************************************************
//  Calculate the mode for each cluster.
//
  for ( ix = 1; ix <= n; ix++ ) // n: number of clusters
  {
    dev[ix-1] = 0.0;            // dev: the distance of observations from their cluster centers.
    for ( iy = 1; iy <= j; iy++ ) // j: number of features
    {
      d[ix-1+(iy-1)*k] = 0.0; // d: cluster center
    }
  }

  for ( ic = 1; ic <= i; ic++ ) // i: number of observations
  {
    ig = b[ic-1];               // b: indicate which cluster each observation has been assigned

    map<int, int> * freqCount;
    for ( ih = 1; ih <= j; ih++ )    // j: number of features
    {
      freqCount = & (m[ig-1+(ih-1)*k]);
      int level = x[ic-1+(ih-1)*i];
      ++(*freqCount)[level];

      int countFreq = 0;
      int largestLevel;

      for (map<int, int>::iterator it =(*freqCount).begin(); it != (*freqCount).end(); ++it)
      {
       if (it->second > countFreq) {
        countFreq = it->second;
        largestLevel = it->first;
        }
      }
     d[ig-1+(ih-1)*k] = largestLevel;
    }
  }


  // //
  // // Get the mean for centers
  // //
  // for ( ij = 1; ij <= j; ij++ )
  // {
  //   for ( ii = 1; ii <= n; ii++ )
  //   {
  //     d[ii-1+(ij-1)*k] = d[ii-1+(ij-1)*k] / ( double ) e[ii-1];  // Get the mean (New Centers)
  //   }
  // }

  //
  // Get New within-cluster-distance measure after updating the center
  //
  for ( ij = 1; ij <= j; ij++ )   // j: number of features
  {
    for ( ik = 1; ik <= i; ik++ )
    {
      il = b[ik-1];               // b: indicate which cluster each observation has been assigned
      da = x[ik-1+(ij-1)*i] - d[il-1+(ij-1)*k]; // distance of observations from the cetners
      if (da != 0) {
        db = 1;
      } else {
        db = 0;
      }

      f[ik-1] = f[ik-1] + db;
      dev[il-1] = dev[il-1] + db;
    }
  }

  // for ( ik = 1; ik <= i; ik++ )
  // {
  //   il = b[ik-1];             // b: indicate which cluster each observation has been assigned
  //   fl = e[il-1];             // e: number of observations assigned to each cluster
  //   if ( 2.0 <= fl )
  //   {
  //     f[ik-1] = f[ik-1] * fl / ( fl - 1.0 );
  //   }
  // }


//  *******************************************************
//  Examine each observation in turn to see if it should be
//  reassigned to a different cluster.
//
 for ( ; ; )
 {
    iw = 0;

    for ( ik = 1; ik <= i; ik++ )
    {
      il = b[ik-1];  // b: indicate which cluster each observation has been assigned
      ir = il;
    //
    //  If the number of cluster points is less than or equal to the
    //  specified minimum, NZ, then bypass this iteration.
    //
      if ( nz < e[il-1] )
      {
        fl = e[il-1];
        dc = f[ik-1];

        for ( in = 1; in <= n; in++ )
        {
          if ( in != il )
          {
            // fm = e[in-1];
            // fm = fm / ( fm + 1.0 );

            de = 0.0;
            for ( ip = 1; ip <= j; ip++ )
            {
              da = x[ik-1+(ip-1)*i] - d[in-1+(ip-1)*k];
              if (da != 0) {
                de += 1;
              }
            }

            if ( de < dc )
            {
              dc = de;
              ir = in;
            }
          }
        }
//
//  Reassignment is made here if necessary.
//
        if ( ir != il )
        {
          fq = e[ir-1];
          dev[il-1] = dev[il-1] - f[ik-1];
          dev[ir-1] = dev[ir-1] + dc;
          e[ir-1] = e[ir-1] + 1;
          e[il-1] = e[il-1] - 1;

        // ********************
        // Update the Center

          // for ( is = 1; is <= j; is++ )
          // {
          //   d[il-1+(is-1)*k] = ( d[il-1+(is-1)*k] 
          //                    * fl - x[ik-1+(is-1)*i] ) / ( fl - 1.0 );
          //   d[ir-1+(is-1)*k] = ( d[ir-1+(is-1)*k]
          //                    * fq + x[ik-1+(is-1)*i] ) / ( fq + 1.0 );
          // }

         for ( is = 1; is <= j; is++ ) {
            map<int, int> * freqCount;
            int level = x[ik-1+(is-1)*i];

            // Update the cluster center that x moved away from
            freqCount = & (m[il-1+(is-1)*k]);
            (*freqCount)[level] = (*freqCount)[level] - 1;
            int countFreq = 0;
            int largestLevel;
            for (map<int, int>::iterator it =(*freqCount).begin(); it != (*freqCount).end(); ++it)
            {
              if (it->second > countFreq) {
                countFreq = it->second;
                largestLevel = it->first;
               }
            }
            d[il-1+(is-1)*k] = largestLevel;

            // Update the cluster center that x moved into
            freqCount = & (m[ir-1+(is-1)*k]);
            (*freqCount)[level] = (*freqCount)[level] + 1;
            countFreq = 0;
            int largestLevel2;
            for (map<int, int>::iterator it =(*freqCount).begin(); it != (*freqCount).end(); ++it)
            {
              if (it->second > countFreq) {
                countFreq = it->second;
                largestLevel2 = it->first;
               }
            }
            d[ir-1+(is-1)*k] = largestLevel2;
          }
          // ********************


          b[ik-1] = ir;

          for ( it = 1; it <= i; it++ )
          {
            ij = b[it-1];

            if ( ij == il || ij == ir )
            {
              f[it-1] = 0.0;
              for ( iu = 1; iu <= j; iu++ )
              {
                da = x[it-1+(iu-1)*i] - d[ij-1+(iu-1)*k];
                if (da != 0) {
                  f[it-1] ++;
                }
              }
              // fl = e[ij-1];
              // f[it-1] = f[it-1] * fl / ( fl - 1.0 );
            }
          }
          iw = iw + 1;
        }
      }
    }
//
//  If any reassignments were made on this pass, then do another pass.
//
    if ( iw == 0 )
    {
      break;
    }
  }
  return;
}
//****************************************************************************80

void Kmode::timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
