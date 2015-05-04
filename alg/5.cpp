#include <iostream>
#include <climits>

using namespace std;

const int n = 6;
const int m = 2;
int* a;
int intervalmax[n][n];
int memo[m+1][n];

//Set up O(1) lookup for max in interval (i, j).
//Initialize memo table
void preprocess()
{
    int i, j;

    for(i=0; i<n; i++)
    {
        intervalmax[i][i]=a[i];
        for(j=i+1; j<n; j++)
        {
            intervalmax[i][j]=max(intervalmax[i][j-1], a[j]);
        }
    }

    for(i=1; i<=m; i++)
    {
        for(j=0; j<n; j++)
        {
            memo[i][j]=-1;
        }
    }
}

//a is the array, n is the size of the list, m is the number of partitions to be made
int solution(int *a, int i, int n, int m)
{
    if(memo[m][i]!=-1)
        return memo[m][i];

    if(m==1)
    {
        memo[m][i]=intervalmax[i][n-1];
        return memo[m][i];
    }

    int minresult=INT_MAX;

    for(int k=i; k<=n-m; k++)
    {
        minresult=min(minresult, intervalmax[i][k]+solution(a, k+1, n, m-1));
    }

    memo[m][i]=minresult;
    return memo[m][i];
}

int main()
{

  a = new int[n];

  a[0] = 50;
  a[1] = 1;
  a[2] = 1;
  a[3] = 1;
  a[4] = 100;
  a[5] = 1;


  preprocess();
  cout << solution(a, 0, n, m) << endl;
  return 0;
}
