#include <climits>
#include <iostream>

using namespace std;

#include<stdio.h>
#include<limits.h>

/**
 *
 */
void print_matrix(int** mat, int n)
{
  for(int i = 0; i < n; ++i)
  {
    for(int j = 0; j < n; ++j)
    {
      cout << mat[i][j];
      if(j + 1 < n) cout << " ";
    }
    cout << endl;
  }
}
// Matrix Ai has dimension p[i-1] x p[i] for i = 1..n
/**
 *
 */
int MatrixChainOrder(int p[],  int** & m, int** & s, int plen)
{
  int n = plen - 1, i, j, k, q, ii;

  //for(i = 1; i <= n; ++i) m[i][i] = 0;

  for(int ii = 1; ii < n; ++ii)
  {
    for(i = 0; i < n - ii; ++i)
    {
      j = i + ii;
      m[i][j] = INT_MAX;
      for(k = i; k < j; ++k)
      {
        q = m[i][k] + m[k+1][j] + p[i] * p[k+1] * p[j+1];
        if(q < m[i][j])
        {
          m[i][j] = q;
          s[i][j] = k;
        }
      }
    }
  }

  cout << "Rendering m\n";
  print_matrix(m, n);
  cout << "\nRendering s\n";
  print_matrix(s, n);
  cout << endl;

  return m[0][n-1];
}

// Driver program to test above function
int main(int argc, char** argv)
{
  //n = 5 , length of a 
  int a[] = {5, 2, 7, 4, 3}; //M1 5 x 2, M2 2 x 7, M3 7 x 4, M4 4 x 3
  int n = sizeof(a)/sizeof(a[0]);
  cout << "sizeof(a)               : " << sizeof(a) << endl
       << "sizeof(a[0])            : " << sizeof(a[0]) << endl
       << "sizeof(a)/sizeof(a[0]): " << sizeof(a)/sizeof(a[0])
       << endl;

  int** m; //Minimum number of scalar multiplication ops
  int** s; //Cost Matrix

  s = new int*[n];
  m = new int*[n];
  for(int i = 0; i < n; ++i)
  {
    s[i] = new int[n];
    m[i] = new int[n];
  }

  cout << "Minimum number of multiplications is "
       <<  MatrixChainOrder(a, m, s, n) << endl;

  return 0;
}
