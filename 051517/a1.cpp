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


  int w[] = {4,  2,  1,  2 },         // weights
      v[] = {25, 50, 45, 25};         // values
  int n   = 4;                        // num items
  int W   = 10;                       // capacity

  int m[n+1][W+1];

  int i, j;

  for( i = 0; i <= n; ++i)
  {
    for( j = 0; j <= W; ++j)
    {
      if(i == 0 || j == 0) m[i][j] = 0;
      else if(w[i-1] <= j)
      {
        m[i][j] = max(m[i-1][j], m[i-1][j-w[i-1]] + v[i-1]);
      }
      else
      {
        m[i][j] = m[i-1][j];
      }
    }
  }


  for(i = 0; i <= n; ++i)
  {
    for(j = 0; j <= W; ++j)
    {
      cout << m[i][j] << " ";
    }
    cout << "\n";
  }

  cout << endl << m[n][W] << endl;

/*
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 25 25 25 25 25 25 25
0 0 50 50 50 50 75 75 75 75 75
0 45 50 95 95 95 95 120 120 120 120
0 45 50 95 95 120 120 120 120 145 145

145

*/
  return 0;
}

