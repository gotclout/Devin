#include <iostream>
#include <fstream>
#include <climits>
#include <ctime>

#define minVal adj[i][j].value < min &&  adj[i][j].value != -1
#define setMin min = adj[i][j].value; k = i; l = j;

const char* nn = "ABCDEFG"; // node names

using namespace std;

//const static int n = 40;
const static int n = 7;

struct data
{
  bool marked, connected; int value;
  data(){marked = connected = 0; value = -1;};
};

data adj[n][n];  //Adjacency Matrix Searching O(|V|^2)

void print_adj()
{
  for(int i = 0; i < n; ++i)
  {
    for(int j = 0; j < n; ++j)
    {
      cout << adj[i][j].value << "|";
      if(adj[i][j].connected) cout << "1|"; else cout << "0|";
      if(adj[i][j].marked) cout << "1"; else cout << "0";
      if(j + 1 < n) cout << "  " ; else cout << "\n";
    }
  }
}

void initialize(int & initial)
{
  char temp;
  ifstream in;
  in.open("n2.txt");
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      temp = in.peek();
      if(temp == '-')in >> temp;
      else
      {
        in >> adj[i][j].value;
        initial += adj[i][j].value;
      }
      j == n - 1 ? in.ignore(1, '\n') : in >> temp;
    }
  }
  in.close();
}

bool connected()
{
  for(int i = 0; i < n; i++)
    if(!adj[0][i].connected) return false;
  return true;
}

void markConnected(const int & jj, const int & ll)
{
  for(int i = 0; i < n; i++)
  {
    if(i == jj)
      for(int j = 0; j < n; j++)
        adj[i][j].marked = true;
    adj[i][ll].connected = true;
  }
}

void optimize(int & pruned, int & k, int & l)
{
  int i, j, min;
  do{
    min = INT_MAX;
    for(i = 0; i < n; i++)
    {
      if(adj[i][0].marked)
      {
        for(j = 1; j < n; j++)
          if(!adj[i][j].connected && minVal){setMin}
      }
    }
    pruned += min;
    cout << "Add Edge: " << nn[k] << "--"     << nn[l]  << " weight: "
         << min          << " total weight: " << pruned << endl;
    adj[k][l].value = -1;
    markConnected(l, l);
  }while(!connected());
}

/**
 * Main Implements Prims Algorithm For MST (GREEDY)
 *
 * -,2,4,6,-,-,-
 * 2,-,2,-,6,-,-
 * 4,2,-,1,3,-,-
 * 6,-,1,-,2,3,-
 * -,6,3,2,-,-,5
 * -,-,-,3,-,-,4
 * -,-,-,-,5,4,-
 *
 *     B----E----G
 * G = |\  /|    |
 *     |  C |    |
 *     | / \|    |
 *     A----D----F
 *
 *     0 1 2 3 4 5 6
 *     A B C D E F G
 **/
int main()
{
  clock_t start, end;
  int initial = 0, pruned = 0, i = 0, j = 0, min, k, l;
  start = clock();
  initialize(initial);
  while(adj[i++][j].value == -1)
  setMin
  for(i; i < n; i++)
    if(minVal){setMin}
  adj[k][l].value = -1;
  markConnected(0, l);
  optimize(pruned, k, l);
  return 0;
}
