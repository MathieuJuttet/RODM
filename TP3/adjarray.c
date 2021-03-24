/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com

Info:
Feel free to use these lines as you wish. This program loads a graph in main memory.

To compile:
"gcc adjlist.c -O9 -o adjlist".

To execute:
"./adjlist edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph will load the graph in main memory and then terminate.

Note:
If the graph is directed (and weighted) with selfloops and you want to make it undirected unweighted without selfloops, use the following linux command line.
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt

Performance:
Up to 200 million edges on my laptop with 8G of RAM: takes more or less 4G of RAM and 30 seconds (I have an SSD hardrive) for 100M edges.
*/

#include "adjarray.h"

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;
    unsigned long e2=NLINKS;
    FILE *file=fopen(input,"r");

	adjlist *g=malloc(sizeof(adjlist));
	g->n=0;
	g->e=0;

    g->orig_to_pros=malloc(e2*sizeof(unsigned long));
    g->pros_to_orig=malloc(e2*sizeof(unsigned long));
	unsigned long node, u , v;
	char name[1000];
    FILE *file_names=fopen(CONST_NAME_NODES,"r");
    while (fscanf(file_names,"%lu %1000[^\n]", &node, name)==2) {
        g->orig_to_pros[node] = g->n;
        g->pros_to_orig[g->n] = node;
        if (++(g->n)==e2) {//increase allocated RAM if needed
            e2+=NLINKS;
            g->orig_to_pros=realloc(g->orig_to_pros, e2*sizeof(unsigned long));
            g->pros_to_orig=realloc(g->pros_to_orig,e2*sizeof(unsigned long));
        }
    }
    g->orig_to_pros=realloc(g->orig_to_pros,node*sizeof(unsigned long));
    g->pros_to_orig=realloc(g->pros_to_orig,g->n*sizeof(unsigned long));

    g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges
	while (fscanf(file,"%lu %lu", &u, &v)==2) {
        g->edges[g->e].s =g->orig_to_pros[u];
        g->edges[g->e].t =g->orig_to_pros[v];
		if (++(g->e)==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);

	g->n++;
	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//building the adjacency matrix
void mkadjlist(adjlist* g){
	g->cd=calloc(g->n, sizeof(unsigned long));
    for (int i=0; i < g->e ;i++) {
        g->cd[g->edges[i].s]++;
    }
}

//freeing memory
void free_adjlist(adjlist *g){
	free(g->edges);
	free(g->cd);
	free(g);
}