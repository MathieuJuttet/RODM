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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
} adjlist;

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");

	adjlist *graph=malloc(sizeof(adjlist));
	graph->n=0;
	graph->e=0;
	graph->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

	while (fscanf(file,"%lu %lu", &(graph->edges[graph->e].s), &(graph->edges[graph->e].t))==2) {
		graph->n=max3(graph->n,graph->edges[graph->e].s,graph->edges[graph->e].t);
		if (++(graph->e)==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			graph->edges=realloc(graph->edges,e1*sizeof(edge));
		}
	}
	fclose(file);

	graph->n++;

	graph->edges=realloc(graph->edges,graph->e*sizeof(edge));

	return graph;
}

//building the adjacency matrix
void mkadjlist(adjlist* graph){
	unsigned long i,u,v;
	unsigned long *d=calloc(graph->n,sizeof(unsigned long));

	for (i=0;i<graph->e;i++) {
		d[graph->edges[i].s]++;
		d[graph->edges[i].t]++;
	}

	graph->cd=malloc((graph->n+1)*sizeof(unsigned long));
	graph->cd[0]=0;
	for (i=1;i<graph->n+1;i++) {
		graph->cd[i]=graph->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	graph->adj=malloc(2*graph->e*sizeof(unsigned long));

	for (i=0;i<graph->e;i++) {
		u=graph->edges[i].s;
		v=graph->edges[i].t;
		graph->adj[graph->cd[u] + d[u]++ ]=v;
		graph->adj[ graph->cd[v] + d[v]++ ]=u;
	}

	free(d);
}


//freeing memory
void free_adjlist(adjlist *graph){
	free(graph->edges);
	free(graph->cd);
	free(graph->adj);
	free(graph);
}




// Define node stucture
typedef struct node node;
struct node {
	unsigned long value;
	node *next;
};

typedef struct {
	node *first;
	node *last;
} queue;



inline unsigned long max_long(unsigned long a, unsigned long b) {
  return a > b ? a : b;
}


queue *init_q(unsigned long s){ 
	queue *q = malloc(sizeof(*q));
	node *new = malloc(sizeof(*new));
	new->value = s;
	new->next = NULL;
	q->first = new;
	q->last = new;
	return q;
}


void add_q(queue *q, unsigned long new_node_value){
	node *new = malloc(sizeof(*new));
	new->value = new_node_value;
	new->next = NULL;
	q->last->next = new;
	q->last = new;
}


unsigned long pop_queue(queue *q){
	node * pop_node = q->first;
	unsigned long pop_node_value = pop_node->value;
	q->first = pop_node->next;
	free(pop_node);
	return pop_node_value;
}

typedef struct {
	unsigned long node_number;
	int distance;
} bfs_result;



bfs_result bfs(adjlist *graph, unsigned long start_node) { 

	queue *q = init_q(start_node);
	bool *visited_nodes = calloc(graph->n,sizeof(bool)); 
	int *distance = calloc(graph->n, sizeof(int)); 
	bfs_result result; 
	result.distance = 0;
	result.node_number = start_node;
	
	
	visited_nodes[start_node] = 1;

	while (q->first != NULL) {  
		unsigned long node_to_pop = q->first->value;
		unsigned long degree = graph->cd[node_to_pop+1] - graph->cd[node_to_pop]; 

		for (int i=0;i<degree;i++) { 
			unsigned long neighbor = graph->adj[ graph->cd[node_to_pop] + i ];

			if (visited_nodes[neighbor] == 0) { 
				distance[neighbor] = distance[node_to_pop] + 1;

				if (distance[neighbor] > result.distance) {
					result.node_number = neighbor;
					result.distance = distance[neighbor];
				}
				add_q(q, neighbor);
				visited_nodes[neighbor] = 1;
			}
		}
		pop_queue(q);
	}

	free(visited_nodes);
	free(distance);	
	free(q);
	return result;
}


int low_bd_estimate(adjlist *graph, int alpha) { 

	int bound = 0;

	for (int i=0;i<alpha;i++) {
		unsigned long w = rand() % (graph->n);
		unsigned long u = bfs(graph, w).node_number;
		int bound_test = bfs(graph, u).distance; 
		bound = max_long(bound, bound_test);
	}
	return bound;
}


int main(int argc,char** argv){
	adjlist* graph;
	time_t t1,t2;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	graph=readedgelist(argv[1]);

	printf("Number of nodes: %lu\n",graph->n);
	printf("Number of edges: %lu\n",graph->e);

	mkadjlist(graph);
	

	t1=time(NULL);
	printf("lower bound : %d with alpha = %d\n", low_bd_estimate(graph, 10), 10); 
	t2=time(NULL);
	t2 = t2-t1;
	printf("- Time for lower bound = %ldh%ldm%lds\n",t2/3600,t2%3600/60,t2%60);

	free_adjlist(graph); 
	return 0;
}







