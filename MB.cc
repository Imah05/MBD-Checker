
// C++ implementation of Hopcroft Karp algorithm for
// maximum matching
#include<bits/stdc++.h>
#include<ctime>
using namespace std;
#define NIL -1
#define INF INT_MAX
 
// A class to represent Bipartite graph for Hopcroft
// Karp implementation
class BipGraph
{
    // m and n are number of vertices on left
    // and right sides of Bipartite Graph
    int m, n;
 
    // adj[u] stores adjacents of left side
    // vertex 'u'. The value of u ranges from 1 to m.
    // 0 is used for dummy vertex
    list<int> *adj;
 
    // These are basically pointers to arrays needed
    // for hopcroftKarp()
    int *pairU, *pairV, *dist;
 
public:
    BipGraph(int m, int n); // Constructor
    void addEdge(int u, int v); // To add edge
 
    // Returns true if there is an augmenting path
    bool bfs();
 
    // Adds augmenting path if there is one beginning
    // with u
    bool dfs(int u);
 
    // Returns size of maximum matching
    int hopcroftKarp();
};
 
// Returns size of maximum matching
int BipGraph::hopcroftKarp()
{
    // pairU[u] stores pair of u in matching where u
    // is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then pairU[u] is NIL
    pairU = new int[m];
 
    // pairV[v] stores pair of v in matching. If v
    // doesn't have any pair, then pairU[v] is NIL
    pairV = new int[n];
 
    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next
    // to u'in augmenting path
    dist = new int[m];
 
    // Initialize NIL as pair of all vertices
    for (int u=0; u<m; u++)
        pairU[u] = NIL;
    for (int v=0; v<n; v++)
        pairV[v] = NIL;
 
    // Initialize result
    int result = 0;
 
    // Keep updating the result while there is an
    // augmenting path.
    while (bfs())
    {
        // Find a free vertex
        for (int u=0; u<m; u++)
 
            // If current vertex is free and there is
            // an augmenting path from current vertex
            if (pairU[u]==NIL && dfs(u))
                result++;
    }
    for (int u=0;u<m;u++) {
    	cout << u << " " << pairU[u] << endl;
    }
    return result;
}
 
// Returns true if there is an augmenting path, else returns
// false
bool BipGraph::bfs()
{
    queue<int> Q; //an integer queue
 
    // First layer of vertices (set distance as 0)
    for (int u=0; u<m; u++)
    {
        // If this is a free vertex, add it to queue
        if (pairU[u]==NIL)
        {
            // u is not matched
            dist[u] = 0;
            Q.push(u);
        }
 
        // Else set distance as infinite so that this vertex
        // is considered next time
        else dist[u] = INF;
    }
 
    // Initialize distance to NIL as infinite
    dist[NIL] = INF;
 
    // Q is going to contain vertices of left side only. 
    while (!Q.empty())
    {
        // Dequeue a vertex
        int u = Q.front();
        Q.pop();
 
        // If this node is not NIL and can provide a shorter path to NIL
        if (dist[u] < dist[NIL])
        {
            // Get all adjacent vertices of the dequeued vertex u
            list<int>::iterator i;
            for (i=adj[u].begin(); i!=adj[u].end(); ++i)
            {
                int v = *i;
 
                // If pair of v is not considered so far
                // (v, pairV[V]) is not yet explored edge.
                if (dist[pairV[v]] == INF)
                {
                    // Consider the pair and add it to queue
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }
 
    // If we could come back to NIL using alternating path of distinct
    // vertices then there is an augmenting path
    return (dist[NIL] != INF);
}
 
// Returns true if there is an augmenting path beginning with free vertex u
bool BipGraph::dfs(int u)
{
    if (u != NIL)
    {
        list<int>::iterator i;
        for (i=adj[u].begin(); i!=adj[u].end(); ++i)
        {
            // Adjacent to u
            int v = *i;
 
            // Follow the distances set by BFS
            if (dist[pairV[v]] == dist[u]+1)
            {
                // If dfs for pair of v also returns
                // true
                if (dfs(pairV[v]) == true)
                {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }
 
        // If there is no augmenting path beginning with u.
        dist[u] = INF;
        return false;	
    }
    return true;
}
 
// Constructor
BipGraph::BipGraph(int m, int n)
{
    this->m = m;
    this->n = n;
    adj = new list<int>[m];
}
 
// To add edge from u to v and v to u
void BipGraph::addEdge(int u, int v)
{
    adj[u].push_back(v); // Add u to v’s list.
}
 
// Driver Program
int main()
{
	int V=21;
	int* degreelist = new int[V];
	for (int i=0; i<16;i++) {
		degreelist[i]=3;
	}
	degreelist[16]=4;
	for (int i=17; i<21;i++) {
		degreelist[i]=6;
	}
	srand(time(0));
	int B=0;
	int scan=0;
	while (degreelist[scan]==3) {
		B++;
		scan++;
	}
	int A=0;
	for (int i=scan; i<V; i++) {
		A+=degreelist[i]-2;
	}
	if (A<B) {
		cout << "There is no point in doing this!" << endl;
		}
	else {
    		BipGraph g(A, B);
    		for (int j=0; j<6;j++) {
    			int neigh=rand()%B;
    			for (int i=0; i<4;i++) {
    				g.addEdge(i,neigh);
    			}
    		}
    		for (int j=0; j<6;j++) {
    			int neigh=rand()%B;
    			for (int i=0; i<4;i++) {
    				g.addEdge(i+4,neigh);
    			}
    		}
    		for (int j=0; j<6;j++) {
    			int neigh=rand()%B;
    			for (int i=0; i<4;i++) {
    				g.addEdge(i+8,neigh);
    			}
    		}
    		for (int j=0; j<6;j++) {
    			int neigh=rand()%B;
    			for (int i=0; i<4;i++) {
    				g.addEdge(i+12,neigh);
    			}
    		}
    		for (int j=0; j<4;j++) {
    			int neigh=rand()%B;
    			for (int i=0; i<2;i++) {
    				g.addEdge(i+16,neigh);
    			}
    		}
    		cout << g.hopcroftKarp() << " is the size of maximum matching." << endl;
    	}
 
    return 0;
}

