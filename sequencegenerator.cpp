#include "sequencegenerator.h"
#include "game_state.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <cmath>
#include <algorithm>


using namespace std;

 
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


bool step1potential(int degreelist[], int V, int k)
{
	int maxdegree=degreelist[V-1];
	int goal=pow(2,degreelist[V-2]);
    	int ub=0;
    	for (int i=0;i<V-k-maxdegree-2;i++) {
   		ub+=pow(2,degreelist[V-2]-degreelist[i]);
   	}
   	for (int i=V-k-2;i<=V-2;i++) {
   		ub+=pow(2,degreelist[V-2]-degreelist[i]);
   	}
   	return (goal<=ub);
}

bool step2potential(int degreelist[], int V, int k)
{
   	int maxdegree=degreelist[V-1];
        int goal=pow(2,degreelist[V-2]+1);
    	int ub=0;
    	int overlap=max(maxdegree+degreelist[V-2]-V+2,0);
    	for (int i=maxdegree;i<V-2;i++) {
    		if ((i<maxdegree+degreelist[V-2]-overlap)&&(degreelist[i]==3)) {
   			ub+=pow(2,degreelist[V-2]-2);
   		}
   		else {
   			ub+=pow(2,degreelist[V-2]-degreelist[i]);
   		}
   	}
   	ub+=2; //for second highest vertex
   	return (goal<=ub);
}


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
   			if ((beginpotential(degreelist, V))&&(step2potential(degreelist, V, k))&&(totaldegreecheck(degreelist, V))) {
        			printArray(degreelist, V);
        			counter++;
        		}
        	}
	}
	return counter;
} 


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

int partitiongeneration(int sur, int len, int V,  int deg1, int deg2, bool twostep) 
{
	int counter=0;
	// The degrees for the high degree vertices is calculated by partitioning 'len'
	int p[len]; // An array to store a partition 
    	int k = 0; // Index of last element in a partition 
    	p[k] = len; // Initialize first partition as number itself 
 	if (len>0) 
 	{
    		// This loop first prints current partition then generates next 
    		// partition. The loop stops when the current partition has all 1s 
    		bool stillworking=true;
    		while (stillworking) 
    		{ 
        		if (twostep) 
        		{
        			counter+=twostepfilter(p, V, sur, k); //generates all degree sequences for current partition using a two-step filter
        		}
        		else
        		{
        			counter+=onestepfilter(p,V,sur,k,deg1,deg2); //generates all degree sequences for current partition using a one-step filter
        		}
        		
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
    	else if (!twostep) {
    		counter+=onestepfilter(p,V,sur,-1,deg1,deg2); //generates all degree sequences for the case where there are no high degree vertices
    	}
    	else
    	{
    		cout << "The one-step filter should not be applied for surplus 0." << endl;
    	}
    	return counter;
}









void onestepsequences(int V, int maxsur) {
	int counter=0;	//counts the degree sequences
     	ofstream myfile;
  	myfile.open ("sequences.txt");
  	for (int sur=0;sur<=maxsur;sur++) { //consider all possible surpluses
  		cout << "Surplus: " << sur << endl;
  		for (int deg1=0;2*deg1<=sur;deg1++) { //counts the number of degree 1 vertices in the core
  			for (int deg2=0;2*deg1+deg2<=sur;deg2++) { //counts the number of degree 2 vertices in the core
				int len=sur-deg2-2*deg1;  //the part of the surplus that remains for high degree vertices
				counter+=partitiongeneration(sur, len, V,  deg1, deg2, false); 		
  			}
  		}
  	}
  	cout << counter << endl;
  	
  	
	myfile.close();
}

void twostepsequences(int Vmin, int Vmax, int minsur, int maxsur) {
	int counter=0; //counts the degree sequences
     	ofstream myfile;
  	myfile.open ("sequences.txt");
  	for (int V=Vmin;V<=Vmax;V++) { //consider all possible number of vertices
  		cout << endl << "Number of vertices: " << V <<  endl;
  		for (int sur=minsur;sur<=maxsur;sur++) { //consider all possible surpluses
			counter+=partitiongeneration(sur, sur, V,  0, 0, true);		
  		}
  	}
	cout << counter << endl;	
}
