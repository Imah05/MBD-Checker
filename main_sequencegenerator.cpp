#include "graph.h"
#include "game_state.h"
#include "filter2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <cmath>
#include <algorithm>


using namespace std;

// A utility function to print an array p[] of size 'n' 
void printArray(int p[], int n) 
{ 
    for (int i = 0; i < n; i++) 
    cout << p[i] << " "; 
    cout << endl; 
}

int* gengeratedegreelist(int p[], int V, int k, int deg1, int deg2)
{
	int* degreelist = new int[V];
        for (int i=0;i<=k;i++) {
        	degreelist[V-i-2]=3+p[i];
        }
        for (int i=0;i<deg1;i++) {
        	degreelist[i]=1;
        }
        for (int i=deg1;i<deg1+deg2;i++) {
        	degreelist[i]=2;
        }
        for (int i=deg1+deg2;i<V-k-2;i++) {
        	degreelist[i]=3;
        }
        degreelist[V-1]=-1;
        return degreelist;
}

       //check the potential at the beginning of the game
bool beginpotential(int degreelist[], int V)
{
	int maxdegree=degreelist[V-1];
	int goal=pow(2,maxdegree+1);
    	int ub=0;
    	for (int i=0;i<V;i++) {
   		ub+=pow(2,maxdegree-degreelist[i]);
   	}
   	return (goal<=ub);
}

   //check the potential after dominator made one move
bool step1potential(int degreelist[], int V, int k)
{
	int maxdegree=degreelist[V-1];
	int goal2=pow(2,degreelist[V-2]);
    	int ub2=0;
    	for (int i=0;i<V-k-maxdegree-2;i++) {
   		ub2+=pow(2,degreelist[V-2]-degreelist[i]);
   	}
   	for (int i=V-k-2;i<=V-2;i++) {
   		ub2+=pow(2,degreelist[V-2]-degreelist[i]);
   	}
   	return (goal2<=ub2);
}

//check whether there is a graph with this degree sequence
bool totaldegreecheck(int degreelist[], int V)
{
	int degreediff=0;
   	for(int i=0; i<V;i++) {
   		if (degreelist[i]<=3) {
   			degreediff+=3;
   		}
   		else {
   			degreediff-=degreelist[i];
   		}
   	}
   	return (degreediff>=0);
}

int twostepfilter(int p[], int V, int sur, int k)
{
	int counter=0;
	int* degreelist = new int[V];
	degreelist=gengeratedegreelist(p, V, k, 0, 0);
	        			for (int j=0;2*j<=V+1;j++) {
        				int maxdegree=sur%2+2*j+(V-1)%2;
        				if ((maxdegree>=degreelist[V-2])&&(maxdegree<=V-1)) {
        					degreelist[V-1]=maxdegree;
   						if (beginpotential(degreelist, V)) {
   							//check the potential after each player made one move
        						int goal2=pow(2,degreelist[V-2]+1);
    							int ub2=0;
    							int overlap=max(maxdegree+degreelist[V-2]-V+2,0);
    							for (int i=maxdegree;i<V-2;i++) {
    								if ((i<maxdegree+degreelist[V-2]-overlap)&&(degreelist[i]==3)) {
   									ub2+=pow(2,degreelist[V-2]-2);
   								}
   								else {
   									ub2+=pow(2,degreelist[V-2]-degreelist[i]);
   								}
   							}
   							ub2+=2; //for second highest vertex
   							if (goal2<=ub2) {
   								if (totaldegreecheck(degreelist,V)) {
        								printArray(degreelist, V);
        								if (degreelist[maxdegree-1]>3) cout << "This case should be looked at!" << endl;
        								counter++;
        							}
        						}
        					}
        					
        				}
        			}
        			return counter;
} 

//allows filtering for degree sequences with mindegree at least 1
int onestepfilter(int p[], int V, int sur, int k, int deg1, int deg2)
{
	int counter=0;
	int* degreelist = new int[V];
	degreelist=gengeratedegreelist(p, V, k, deg1, deg2);
	for (int j=0;2*j<=V+1;j++) {
        	int maxdegree=2*j+(V-sur-1)%2;
        	if ((maxdegree>=degreelist[V-2])&&(maxdegree<=V-1)) {
        	degreelist[V-1]=maxdegree;
   			if ((beginpotential(degreelist, V))&&(step1potential(degreelist, V, k))&&(totaldegreecheck(degreelist, V))) {
        			printArray(degreelist, V);
        			counter++;
        		}				
        	}
        }
        return counter;
} 




int main() {

/*
    string filename;
    cout << "Enter the name of the file containing graphs in graph6 format: ";
    cin >> filename;

    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    string graph6;
    vector<string> graphList;
    while (getline(inputFile, graph6)) {
        graphList.push_back(graph6);
    }
    inputFile.close();
	
	cout << "Loading complete." << endl;
    int counter = 0;
    int badcounter = 0;
    int* when = new int[5];  //counts when graphs get filtered
    for (int i=0; i<5; i++) {
    	when[i]=0;
    }
    int numberOfGraphs = graphList.size();
    bool schalter = true;
     ofstream myfile;
  	myfile.open (filename+"Filter2.g6");
  	
    for (const string& g6 : graphList) {
        ++counter;
        bool isbad=filter2(g6);
        if (isbad) {
        	isbad=filter1(g6);
        }

	if (isbad) {
		badcounter++;
		Graph gpaul = Graph(g6);
		//gpaul.printGraph();	
		myfile << g6 << endl;
	}
    	if (counter%10000==0) {
        	cout << "At the moment we handeled " << counter << "/" << numberOfGraphs << " graphs." << endl;
        	cout << badcounter << " of them behave badly." << endl;
        }
    }
	cout << "In total we handeled " << counter << "/" << numberOfGraphs << " graphs." << endl;
       	cout << badcounter << " of them behave badly." << endl;
	myfile.close();
	*/
	
	
	
	//This is the part where we print the degree list
	//Generating partitions from https://www.geeksforgeeks.org/generate-unique-partitions-of-an-integer/
	
	int V=12;
	int counter=0;
	
     	ofstream myfile;
  	myfile.open ("sequences.txt");
  		for (int sur=0;sur<=5;sur++) {
  		cout << "Surplus: " << sur << endl;
  			for (int deg1=0;2*deg1<=sur;deg1++) {
  			for (int deg2=0;2*deg1+deg2<=sur;deg2++) {
			int len=sur-deg2-2*deg1;
			int p[len]; // An array to store a partition 
    			int k = 0; // Index of last element in a partition 
    			p[k] = len; // Initialize first partition as number itself 
 			if (len>0) {
    				// This loop first prints current partition then generates next 
    				// partition. The loop stops when the current partition has all 1s 
    				bool stillworking=true;
    				while (stillworking) 
    				{ 
					//if(twostepfilter(p,V,sur,k)) counter++;
        				counter+=onestepfilter(p,V,sur,k,deg1,deg2);
        				// Generate next partition 
 
        				// Find the rightmost non-one value in p[]. Also, update the 
        				// rem_val so that we know how much value can be accommodated 
        				int rem_val = 0; 
        				while (k >= 0 && p[k] == 1) 
        				{ 
            					rem_val += p[k]; 
            					k--; 
        				} 
 
        				// if k < 0, all the values are 1 so there are no more partitions 
        				if (k < 0) {
        					stillworking=false;
        				}
 					else {
        					// Decrease the p[k] found above and adjust the rem_val 
        					p[k]--;  
        					rem_val++; 
 
 
        					// If rem_val is more, then the sorted order is violated. Divide 
        					// rem_val in different values of size p[k] and copy these values at 
        					// different positions after p[k] 
        					while (rem_val > p[k]) 
        					{ 
            						p[k+1] = p[k]; 
            						rem_val = rem_val - p[k]; 
            						k++; 
        					} 
 
        					// Copy rem_val to next position and increment position 
        					p[k+1] = rem_val; 
        					k++;
        					if (k>=V) stillworking=false;
        				}
        			}
    			}
    			else {
    				counter+=onestepfilter(p,V,sur,-1,deg1,deg2);
    			}  		
  		}
  		}
  		}
  	cout << counter << endl;
  		
  	/*
  	int V=21;
	int* degreelist = new int[V];
	int maxdegree=0;
	int maxindex=0;
	for (int i=0; i<V;i++) {
		degreelist[i]=gpaul.neighborhood(i).size();
		if (degreelist[i]>maxdegree) {3 
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
  */	
  	
  	
	myfile.close();
	
	
    return 0;
}
