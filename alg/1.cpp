// A C++ program to find biconnected components in a given undirected graph
#include<iostream>
#include <list>
#include <stack>
#define NIL -1
using namespace std;
int count = 0;
class Edge
{
    public:
    int u;
    int v;
    Edge(int u, int v);
};
Edge::Edge(int u, int v)
{
    this->u = u;
    this->v = v;
}

// A class that represents an directed graph
class Graph
{
    int V;    // No. of vertices
    int E;    // No. of edges
    list<int> *adj;    // A dynamic array of adjacency lists

    // A Recursive DFS based function used by BCC()
    void BCCUtil(int u, int disc[], int low[],
                 list<Edge> *st, int parent[]);
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    void BCC();    // prints strongly connected components
};

Graph::Graph(int V)
{
    this->V = V;
    this->E = 0;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    E++;
}

// A recursive function that finds and prints strongly connected
// components using DFS traversal
// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store visited edges
void Graph::BCCUtil(int u, int disc[], int low[], list<Edge> *st,
                    int parent[])
{
    // A static variable is used for simplicity, we can avoid use
    // of static variable by passing a pointer.
    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    int children = 0;

    // Go through all vertices adjacent to this
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of 'u'

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            children++;
            parent[v] = u;
            //store the edge in stack
            st->push_back(Edge(u,v));
            BCCUtil(v, disc, low, st, parent);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 -- per Strongly Connected Components Article
            low[u]  = min(low[u], low[v]);

            //If u is an articulation point,
            //pop all edges from stack till u -- v
            if( (disc[u] == 1 && children > 1) ||
                (disc[u] > 1 && low[v] >= disc[u]) )
            {
                while(st->back().u != u || st->back().v != v)
                {
                    cout << st->back().u << "--" << st->back().v << " ";
                    st->pop_back();
                }
                cout << st->back().u << "--" << st->back().v;
                st->pop_back();
                cout << endl;
                count++;
            }
        }

        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 -- per Strongly Connected Components Article
        else if(v != parent[u] && disc[v] < low[u])
        {
            low[u]  = min(low[u], disc[v]);
            st->push_back(Edge(u,v));
        }
    }
}

// The function to do DFS traversal. It uses BCCUtil()
void Graph::BCC()
{
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];
    list<Edge> *st = new list<Edge>[E];

    // Initialize disc and low, and parent arrays
    for (int i = 0; i < V; i++)
    {
        disc[i] = NIL;
        low[i] = NIL;
        parent[i] = NIL;
    }

    for (int i = 0; i < V; i++)
    {
        if (disc[i] == NIL)
            BCCUtil(i, disc, low, st, parent);

        int j = 0;
        //If stack is not empty, pop all edges from stack
        while(st->size() > 0)
        {
            j = 1;
            cout << st->back().u << "--" << st->back().v << " ";
            st->pop_back();
        }
        if(j == 1)
        {
            cout << endl;
            count++;
        }
    }
}

// Driver program to test above function
int main()
{
    Graph g(11);
    g.addEdge(0,1);g.addEdge(1,0);
    g.addEdge(0,7);g.addEdge(7,0);
    g.addEdge(0,4);g.addEdge(4,0);
    g.addEdge(1,4);g.addEdge(4,1);
    g.addEdge(1,5);g.addEdge(5,1);
    g.addEdge(2,5);g.addEdge(5,2);
    g.addEdge(2,9);g.addEdge(9,2);
    g.addEdge(3,6);g.addEdge(6,3);
    g.addEdge(3,10);g.addEdge(10,3);
    g.addEdge(4,8);g.addEdge(8,4);
    g.addEdge(5,8);g.addEdge(8,5);
    g.addEdge(5,9);g.addEdge(9,5);
    g.addEdge(6,9);g.addEdge(9,6);
    g.addEdge(6,10);g.addEdge(10,6);
    g.addEdge(9,10);g.addEdge(10,9);
    g.BCC();
    cout << "Above are " << count << " biconnected components in graph a\n";

    count = 0;

    Graph g1(11);
    g1.addEdge(0,1);g1.addEdge(1,0);
    g1.addEdge(1,5);g1.addEdge(5,1);
    g1.addEdge(2,6);g1.addEdge(6,2);
    g1.addEdge(2,3);g1.addEdge(3,2);
    g1.addEdge(2,7);g1.addEdge(7,2);
    g1.addEdge(3,7);g1.addEdge(7,3);
    g1.addEdge(4,5);g1.addEdge(5,4);
    g1.addEdge(5,6);g1.addEdge(6,5);
    g1.addEdge(4,8);g1.addEdge(8,4);
    g1.addEdge(5,9);g1.addEdge(9,5);
    g1.addEdge(6,10);g1.addEdge(10,6);
    g1.addEdge(5,10);g1.addEdge(10,5);
    g1.addEdge(4,9);g1.addEdge(9,4);
    g1.addEdge(9,10);g1.addEdge(10,9);
    g1.BCC();
    cout << "Above are " << count << " biconnected components in graph b\n";

    count = 0;
    Graph g2(10);
    g2.addEdge(0,6);g2.addEdge(6,0);
    g2.addEdge(0,1);g2.addEdge(1,0);
    g2.addEdge(1,4);g2.addEdge(4,1);
    g2.addEdge(2,5);g2.addEdge(5,2);
    g2.addEdge(2,3);g2.addEdge(3,2);
    g2.addEdge(3,5);g2.addEdge(5,3);
    g2.addEdge(4,5);g2.addEdge(5,4);
    g2.addEdge(1,6);g2.addEdge(6,1);
    g2.addEdge(4,6);g2.addEdge(6,4);
    g2.addEdge(5,8);g2.addEdge(8,5);
    g2.addEdge(5,7);g2.addEdge(7,5);
    g2.addEdge(7,8);g2.addEdge(8,7);
    g2.addEdge(8,9);g2.addEdge(9,8);
    g2.BCC();
    cout << "Above are " << count << " biconnected components in graph c\n";

    count = 0;
    Graph g3(12);
    g3.addEdge(0,1);g3.addEdge(1,0);
    g3.addEdge(1,5);g3.addEdge(5,1);
    g3.addEdge(1,4);g3.addEdge(4,1);
    g3.addEdge(2,3);g3.addEdge(3,2);
    g3.addEdge(2,6);g3.addEdge(6,2);
    g3.addEdge(3,6);g3.addEdge(6,3);
    g3.addEdge(4,5);g3.addEdge(5,4);
    g3.addEdge(4,8);g3.addEdge(8,4);
    g3.addEdge(4,9);g3.addEdge(9,4);
    g3.addEdge(4,10);g3.addEdge(10,4);
    g3.addEdge(6,10);g3.addEdge(10,6);
    g3.addEdge(6,7);g3.addEdge(7,6);
    g3.addEdge(7,10);g3.addEdge(10,7);
    g3.addEdge(7,11);g3.addEdge(11,7);
    g3.addEdge(8,9);g3.addEdge(9,8);
    g3.addEdge(9,10);g3.addEdge(10,9);
    g3.addEdge(10,11);g3.addEdge(11,10);
    g3.BCC();
    cout << "Above are " << count << " biconnected components in graph d\n";

    return 0;
}
