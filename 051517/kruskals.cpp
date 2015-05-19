// Kruskal's algortihm to find Minimum Spanning Tree of a given connected,
// undirected and weighted graph
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

const char* nn = "ABCDEFG"; // node names

// a structure to represent a weighted edge in graph
struct Edge
{
    int src, dest, weight;
};

// a structure to represent a connected, undirected and weighted graph
struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges. Since the graph is
    // undirected, the edge from src to dest is also edge from dest
    // to src. Both are counted as 1 edge here.
    struct Edge* edge;
};

// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;

    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );

    return graph;
}

// A structure to represent a subset for union-find
struct subset
{
    int parent;
    int rank;
};

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high rank tree
    // (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and increment
    // its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare two edges according to their weights.
// Used in qsort() for sorting an array of edges
int myComp(const void* a, const void* b)
{
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}

// The main function to construct MST using Kruskal's algorithm
void KruskalMST(struct Graph* graph)
{
    int V = graph->V;
    struct Edge result[V];  // Tnis will store the resultant MST
    int e = 0;  // An index variable, used for result[]
    int i = 0;  // An index variable, used for sorted edges

    // Step 1:  Sort all the edges in non-decreasing order of their weight
    // If we are not allowed to change the given graph, we can create a copy of
    // array of edges
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);

    // Allocate memory for creating V ssubsets
    struct subset *subsets =
        (struct subset*) malloc( V * sizeof(struct subset) );

    // Create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Number of edges to be taken is equal to V-1
    while (e < V - 1)
    {
        // Step 2: Pick the smallest edge. And increment the index
        // for next iteration
        struct Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // If including this edge does't cause cycle, include it
        // in result and increment the index of result for next edge
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Else discard the next_edge
    }

    // print the contents of result[] to display the built MST
    printf("Following are the edges in the constructed MST\n");
    int sum = 0;
    for (i = 0; i < e; ++i)
    {
       sum +=  result[i].weight;
       cout << "Add Edge: " << nn[result[i].src] << "--" << nn[result[i].dest]
            << " weight: "  << result[i].weight
            << " total weight: " << sum << endl;
    }
    return;
}

// Driver program to test above functions
int main()
{
    /* Let us create following weighted graph

       B----E----G
       |\  /|    |
       |  C |    |
       | / \|    |
       A----D----F

       0 1 2 3 4 5 6
       A B C D E F G
     */
    int V = 7;  // Number of vertices in graph
    int E = 11;  // Number of edges in graph
    struct Graph* graph = createGraph(V, E);



    // add edge A B
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;
    graph->edge[0].weight = 2;

    // add edge A D
    graph->edge[1].src = 0;
    graph->edge[1].dest = 3;
    graph->edge[1].weight = 6;

    // add edge A C
    graph->edge[2].src = 0;
    graph->edge[2].dest = 2;
    graph->edge[2].weight = 4;

    // add edge 1-3 B E
    graph->edge[3].src = 1;
    graph->edge[3].dest = 4;
    graph->edge[3].weight = 6;

    // add edge 2-3 B C
    graph->edge[4].src = 1;
    graph->edge[4].dest = 2;
    graph->edge[4].weight = 2;

    // add edge 2-3 C D
    graph->edge[5].src = 2;
    graph->edge[5].dest = 3;
    graph->edge[5].weight = 1;

    // add edge 2-3 C E
    graph->edge[6].src = 2;
    graph->edge[6].dest = 4;
    graph->edge[6].weight = 3;

    // add edge 2-3 D E
    graph->edge[7].src = 3;
    graph->edge[7].dest = 4;
    graph->edge[7].weight = 2;

    // add edge 2-3 E G
    graph->edge[8].src = 4;
    graph->edge[8].dest = 6;
    graph->edge[8].weight = 5;

    // add edge 2-3 D F
    graph->edge[9].src = 3;
    graph->edge[9].dest = 5;
    graph->edge[9].weight = 3;

    // add edge F G
    graph->edge[10].src = 5;
    graph->edge[10].dest = 6;
    graph->edge[10].weight = 4;

    KruskalMST(graph);

    return 0;
}


