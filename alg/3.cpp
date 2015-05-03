#include <iostream>
#include <string>

using namespace std;

struct M
{
  int i, //index
      w; //weight

  M() { w = 0;};
};

int main(int argc, char** argv)
{

  int w[] = {4,  2,  1,  2,  3,  2 }, // weights
      v[] = {25, 50, 45, 35, 15, 30}; // values

  int n   = 6;                        // num items
  int W   = 10;                       // capacity

  int m[n][W];

  int i, j;
  for( j = 0; j < W; ++j)
  {
      m[0][j] = 0;
  }

  for( i = 1; i < n; ++i)
  {
    for( j = 0; j < W; ++j)
    {
      if(w[i] <= j)
      {
        m[i][j] = max(m[i-1][j], m[i-1][j-w[i]] + v[i]);
      }
      else
      {
        m[i][j] = m[i-1][j];
      }
    }
  }


  for(i = 0; i < n; ++i)
  {
    for(j = 0; j < W; ++j)
    {
      cout << m[i][j] << " ";
    }
    cout << "\n";
  }

  return 0;
}

