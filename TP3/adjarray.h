/*
Header for adjarray.c file
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define CONST_NAME_NODES "../Graphs/enwiki-names.txt"

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
    unsigned long *orig_to_pros;
    unsigned long *pros_to_orig;
} adjlist;

//reading the edgelist from file
adjlist* readedgelist(char* input);

//building the adjacency matrix
void mkadjlist(adjlist* g);

