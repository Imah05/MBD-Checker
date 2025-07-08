#ifndef SEQUENCEGENERATOR_H
#define SEQUENCEGENERATOR_H


// A utility function to print an array p[] of size 'n' 
void printArray(int p[], int n);

//generates a degreelist for a graph on V vertices, where there are deg1 many vertices of degree 1,
//deg2 many of degree 2 and k many vertices of degree 3+p[n], all other degrees are 3
int* gengeratedegreelist(int p[], int V, int k, int deg1, int deg2);

//check the potential at the beginning of the game
bool beginpotential(int degreelist[], int V);

//check the potential after dominator made one move
bool step1potential(int degreelist[], int V, int k);

//check the potential after each player made one move
bool step2potential(int degreelist[], int V, int k);

//check whether there is a graph with a given degree sequence
bool totaldegreecheck(int degreelist[], int V);

//allows filtering for degree sequences with mindegree at least 3
int twostepfilter(int p[], int V, int sur, int k);

//allows filtering for degree sequences with mindegree at least 1
int onestepfilter(int p[], int V, int sur, int deg1, int deg2);

//Generating partitions from https://www.geeksforgeeks.org/generate-unique-partitions-of-an-integer/
int partitiongeneration(int sur, int len, int V, int k,  int deg1, int deg2, bool twostep);

//lists all the degree sequences for graphs of V vertices with surplus <=maxsur, that can not be
//ruled out by one-step potential considerations
void onestepsequences(int V, int maxsur);

//lists all the degree sequences for graphs of Vmin to Vmax vertices with surplus between minsur and maxsur, that can not be
//ruled out by two-step potential considerations
void twostepsequences(int Vmin, int Vmax, int minsur, int maxsur);





#endif // SEQUENCEGENERATOR_H
