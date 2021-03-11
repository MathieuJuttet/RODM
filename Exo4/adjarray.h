/*
Header for adjarray.c file
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
    unsigned long *labels;
} adjlist;

//compute the maximum of three unsigned long
unsigned long max3(unsigned long a,unsigned long b,unsigned long c);

//reading the edgelist from file
adjlist* readedgelist(char* input);

//building the adjacency matrix
void mkadjlist(adjlist* g);

//freeing memory
void free_adjlist(adjlist *g);
