#include "filter2.h"

bool matchingfilter(const int B, const int W,const int right3,const int surplus,const int* degreelist,const int* wantdegree,const Graph& gpaul) {
	bool isbad=true;
	BipGraph g(B-right3+surplus, B);
	int k=0;
	for (int j=0;j<W;j++) {
    		for (int i=0; i<wantdegree[j];i++) {
    			for (auto elem : gpaul.neighborhood(j)) {
    				g.addEdge(k,elem-W);
    			}
    			k++;
    		}
    	}
    	if (g.hopcroftKarp()==B-right3) isbad=false;
    	return isbad;
}


bool filter1old(const string& g6) {
	Graph gpaul = Graph(g6);
	//gpaul.printGraph();
    	int V=21;
	int* degreelist = new int[V];
	int maxdegree=0;
	int maxindex=0;
	for (int i=0; i<V;i++) {
		degreelist[i]=gpaul.neighborhood(i).size();
		if (degreelist[i]>maxdegree) {
			maxdegree=degreelist[i];
			maxindex=i;
		}
	}
	int scan=0;
	int W=0; //number of big vertices
	while (degreelist[scan]>3) {
		W++;
		scan++;
	}
	int B=V-W; //number of small vertices
    	bool isbad=true;
    	int goal=pow(2,maxdegree+1);
    	bool below1=true;
    	
    	// Staller could pick a degree 3 vertex, compute an upper bound for this case
    	int ub=0;
    	for (int i=0;i<W;i++) {
    		if (i!=maxindex) {
   			ub+=pow(2,maxdegree-degreelist[i]);
   		}
    	}
    	ub+=(B-4-maxdegree)*pow(2,maxdegree-3);
    	ub+=4*pow(2,maxdegree-3);
    	if (ub>=goal) {
   		below1=false;
   		//gpaul.printGraph();
   	}
    	
    	for (int i=0;i<W;i++) {  //Going over all of Stallers moves, picking a large vertex
    	    	int cost=0;
    		if (i!=maxindex) {
    			int countdouble=0;
    			for (int j=0;j<B;j++) {
    				if ((gpaul.hasEdge(maxindex, W+j))&&(gpaul.hasEdge(i, W+j))) {
    					countdouble++;
    				}
   			}
   			int deg2=degreelist[i]-countdouble;
   			for (int j=0;j<W;j++) {
   				if (j==i) {
   					cost+=pow(2,maxdegree-degreelist[j]+1);
   				}
   				else if (j!=maxindex) {
   					cost+=pow(2,maxdegree-degreelist[j]);
   				}
   			}
   			cost+=(B-maxdegree-deg2)*pow(2,maxdegree-3);
   			cost+=deg2*pow(2,maxdegree-2);
   			if (cost>=goal) {
   				below1=false;
   			}
    		}
    	}

  	return !below1;
}

bool filter1(const string& g6) {
    Graph g = Graph(g6);
    Core core = Core(&g);
    bool isbad=!core.filter();
	return isbad;
}


bool filter2(const string& g6) {
	Graph gpaul = Graph(g6);
	//gpaul.printGraph();
    	int V=21;
	int* degreelist = new int[V];
	for (int i=0; i<V;i++) {
		degreelist[i]=gpaul.neighborhood(i).size();
	}
	int scan=0;
	int W=0; //number of big vertices
	while (degreelist[scan]>3) {
		W++;
		scan++;
	}
	int B=V-W; //number of small vertices
    	bool isbad=true;
	for (int right3=0; right3<=2;right3++) {  //right3 counts the remaining degree 3 vertices on the righthand side (B)
		if(isbad&&(right3==2)) {
			int A=0;
			for (int i=0; i<W; i++) {
				A+=degreelist[i]-2;
			}
			if (A==B-2) {
				int* wantdegree = new int[W];
				for (int i=0; i<W; i++) {
					wantdegree[i]=degreelist[i]-2;
				}
				//cout << "We are in case 2" << endl;
				isbad=matchingfilter(B,W,right3,0,degreelist,wantdegree,gpaul);
			}
		}
		else if (isbad&&(right3==1)) {
			int A=0;
			for (int i=0; i<W; i++) {
				A+=degreelist[i]-2;
			}
			int diff=A-B+right3;
			if (diff>=0) {
				for (int i=0;i<W;i++) {
					if (isbad&&(degreelist[i]-diff-2>=0)) {
						int* wantdegree = new int[W];
						for (int j=0; j<W; j++) {
							if (i==j) {
								wantdegree[j]=degreelist[j]-diff-2;
							}
							else {
								wantdegree[j]=degreelist[j]-2;
							}
						}
						isbad=matchingfilter(B,W,right3,0,degreelist,wantdegree,gpaul);
						
					}
				}
			}
		}
		else if (isbad&&(right3==0)) {
			int A=0;
			for (int i=0; i<W; i++) {
				A+=degreelist[i]-2;
			}
			int diff=A-B+right3;
			if (diff==0) {    //will result in 22...2
				int* wantdegree = new int[W];
				for (int i=0; i<W; i++) {
					wantdegree[i]=degreelist[i]-2;
				}
				//cout << "We are in case 00" << endl;
				isbad=matchingfilter(B,W,right3,0,degreelist,wantdegree,gpaul);
			}
			else if (diff==2) {    //will result in 3322...2 or 422...2
				int* wantdegree = new int[W];
				for (int i=0; i<W; i++) {
					wantdegree[i]=degreelist[i]-2;
				}
				//cout << "We are in case 02" << endl;
				isbad=matchingfilter(B,W,right3,2,degreelist,wantdegree,gpaul);
			}
			else if (diff>2) {
				for (int i=0;i<W;i++) { //will result in e22...2
					if (isbad&&(degreelist[i]-diff-2>=0)) {
						int* wantdegree = new int[W];
						for (int j=0; j<W; j++) {
							if (i==j) {
								wantdegree[j]=degreelist[j]-diff-2;
							}
							else {
								wantdegree[j]=degreelist[j]-2;
							}
						}
						//cout << "These are the parameters: diff " << diff << " i " << i << " bad? " << isbad << endl;
						//for (int l=0;l<W;l++) {
						//	cout << "The degrees are " << wantdegree[l] << " / " << degreelist[l] << endl;
						//}
						//cout << "We are in case 04" << endl;
						isbad=matchingfilter(B,W,right3,0,degreelist,wantdegree,gpaul);
						//cout << isbad << endl;
					}
				}
				for (int i=0;i<W;i++) { //will result in o32...2
					if (isbad&&(degreelist[i]-diff-1>=0)) {
						for (int k=0;k<W;k++) {
							if (isbad&&(i!=k)) {
								int* wantdegree = new int[W];
								for (int j=0; j<W; j++) {
									if (i==j) {
										wantdegree[j]=degreelist[j]-diff-1;
									}
									else if (k==j){
										wantdegree[j]=degreelist[j]-3;
									}
									else {
										wantdegree[j]=degreelist[j]-2;
									}
								}
								//cout << "These are the parameters: diff " << diff << " i " << i << " k " << k << " bad? " << isbad << endl;
								//for (int l=0;l<W;l++) {
								//	cout << "The degrees are " << wantdegree[l] << " / " << degreelist[l] << endl;
								//}
								isbad=matchingfilter(B,W,right3,0,degreelist,wantdegree,gpaul);
								//cout << isbad << endl;
								//gpaul.printGraph();
							}
						}
					}
				}
			}
		}
	}
	return isbad;
}


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

