#ifndef DENSET_H_INCLUDED
#define DENSET_H_INCLUDED

#define NEDGES 100000000 //maximum number of edges for memory allocation, will increase

// Below are the structuctures and functions needed for the DENSET TP

// Structures :

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

inline unsigned long max_of_3(unsigned long a,unsigned long b,unsigned long c);

adjlist* readedgelist(char* input);

void mkadjlist(adjlist* g);

void free_adjlist(adjlist *g);

typedef struct{
    unsigned long size;
    unsigned long* nodes;
} NodeArray;

// Usefull functions :

//reading the edgelist from file
adjlist* readedgelist(char* input){
    unsigned long e1=NEDGES;
    FILE *file=fopen(input,"r");

    adjlist *g=malloc(sizeof(adjlist));
    g->n=0;
    g->e=0;
    g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

    while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
        g->n=max_of_3(g->n,g->edges[g->e].s,g->edges[g->e].t);
        if (++(g->e)==e1) {//increase allocated RAM if needed
            e1+=NEDGES;
            g->edges=realloc(g->edges,e1*sizeof(edge));
        }
    }
    fclose(file);

    g->n++;

    g->edges=realloc(g->edges,g->e*sizeof(edge));

    return g;
};

//building the adjacency matrix
void mkadjlist(adjlist* g){
    unsigned long i,u,v;
    unsigned long *d=calloc(g->n,sizeof(unsigned long));

    for (i=0;i<g->e;i++) {
        d[g->edges[i].s]++;
        d[g->edges[i].t]++;
    }

    g->cd=malloc((g->n+1)*sizeof(unsigned long));
    g->cd[0]=0;
    for (i=1;i<g->n+1;i++) {
        g->cd[i]=g->cd[i-1]+d[i-1];
        d[i-1]=0;
    }

    g->adj=malloc(2*g->e*sizeof(unsigned long));

    for (i=0;i<g->e;i++) {
        u=g->edges[i].s;
        v=g->edges[i].t;
        g->adj[ g->cd[u] + d[u]++ ]=v;
        g->adj[ g->cd[v] + d[v]++ ]=u;
    }

    free(d);
    //free(g->edges);
};

//compute the maximum of three unsigned long
inline unsigned long max_of_3(unsigned long a,unsigned long b,unsigned long c){
    a=(a>b) ? a : b;
    return (a>c) ? a : c;
};

void free_adjlist(adjlist *g){
    free(g->edges);
    free(g->cd);
    free(g->adj);
    free(g);
};

NodeArray createNodeArray(){
    NodeArray array;
    array.nodes = malloc(0);
    array.size = 0;
    return array;
};

unsigned long Update(NodeArray* array){
    return array->nodes[array->size-1];
};

void addNode(NodeArray* array, unsigned long node){
    array->nodes = realloc(array->nodes,(array->size+1)*sizeof(unsigned long));
    array->nodes[array->size] = node;
    array->size++;
};

unsigned long popNode(NodeArray* array){
    unsigned long node = Update(array);
    array->size--;
    array->nodes = realloc(array->nodes,array->size*sizeof(unsigned long));
    return node;
};

void removeNode(NodeArray* array, unsigned long idx){
    array->nodes[idx] = Update(array);
    popNode(array);
};


#endif // DENSET_H_INCLUDED
