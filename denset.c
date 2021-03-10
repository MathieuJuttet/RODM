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
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

#include "denset.h"


// This function, given a graph, returns (res[0]) the core value of the graph, as well as (res[1]) the average degree density, (res[2]) the edge density and (res[3]) the size of a densest core ordering prefix.

 void densest_subgraph(adjlist* graph, long double * output){
    
    // initialization of all the variables :
    
    unsigned long n = graph->n; // number of nodes within the graph
    unsigned long edges = graph->e; // Nomber of edges of the given graph
     
    unsigned long degree; // iterable wich will represent the degree
    unsigned long node; // iterable wich will represent the nodes
    unsigned long neighbor; // iterable wich will represent the neighbors of a given node
    unsigned long update; // useful for the update
    
    // intilization of outputs :
    unsigned long core_max = 0; // the core of the graph initialised on 0
    long double average_degree_density = 0.0; // average degree density
    long double edge_density = 0.0; // edge density
    long double size_densest_core_prefix = 0.0; // size of densest core ordering prefix

    // Our indexs :
    unsigned long i;
    unsigned long j;
    
     // tab of n nodes that will contains the codre of each nodes
    unsigned long* c = malloc(n * sizeof(unsigned long));
     
    // tab of n nodes that will represent the index of each nodes
    unsigned long* o = malloc(n * sizeof(unsigned long));
          
    // tab with n cases that store the degree for each nodes :
    unsigned long* deg = malloc(graph->n*sizeof(unsigned long));
    
    // will store the maximum degree of the nodes in the graph
    unsigned long degree_max = 0;
    
    // We iterate through each nodes to get the
    for (node = 0; node < graph->n; node++){
        deg[node] = graph->cd[node+1]-graph->cd[node]; // degree of the node
        if (deg[node] > degree_max) degree_max = deg[node]; // update maximal degree
    }


    NodeArray nodesOfDegree[degree_max+1]; // Initialization of a node
    unsigned long* nodeIndex = malloc(graph->n*sizeof(unsigned long)); // tab of each index
    bool* in_graph = malloc(graph->n*sizeof(bool)); // tab of bool to tell us whether a node is still in the current graph

    // We make a tab of nodes for each degree
    for (degree = 0; degree <= degree_max; degree++){
        nodesOfDegree[degree] = createNodeArray();
    }

    
     for (node = 0; node < graph->n; node++){
        
        // we intilialiaze our tab full of True since all of the nodes are in the starting given graph :
        in_graph[node] = true;
        
        // We get the degree of our nodes :
        degree = deg[node];
        
        // The index of the node in its degree tab is its rank of apparition
        nodeIndex[node] = nodesOfDegree[degree].size;
        
        // we add the node
        addNode(&nodesOfDegree[degree],node);
    }

    // We iterate untill the graph is empty
    while (n > 0) {
        degree = 0; // start with degree 0
        while (nodesOfDegree[degree].size == 0) degree++; // we get the first degree with nodes that indeed has this degree
        
        node = popNode(&nodesOfDegree[degree]); // we get the node with the corresponding degree

        in_graph[node] = false; // Now we get the degree out of the graph

        if (degree > core_max) core_max = degree; // we update the maximal core of the graph
        c[node] = core_max; // we take its k-core
        o[node] = n; // we take its index of apparition

        // now we update its neigbhors since they lost a neighbor
        for (i = graph->cd[node]; i < graph->cd[node+1]; i++) {
            neighbor = graph->adj[i]; // we visit each point of our node
            if (in_graph[neighbor]) {
                degree = deg[neighbor]; // the degree of the current neighbor
                j = nodeIndex[neighbor]; // index the neighbor
                update = Update(&nodesOfDegree[degree]);
                nodeIndex[update] = j;

                removeNode(&nodesOfDegree[degree],j); // The neighbor lost a degree since the node is not in the graph anymore

                nodeIndex[neighbor] = nodesOfDegree[degree-1].size; // Then its index change
                
                addNode(&nodesOfDegree[degree-1],neighbor); // Now neighbor has degree-1 neighbors, it goes to the tab of nodes with degree-1 with degree
                
                deg[neighbor]--;// neigbor losts node as neighbor
                edges--;// then the graph losts an edge
            }
        }

        // We update the number of nodes in our graph :
        n--;
        
        // We update our densest core prefix :
        if (n>2 && average_degree_density <= ((double)(edges)) / ((double)(n))) {
            average_degree_density = edges / ((double)n) ;
            edge_density = (2. * edges) / ((n-1)*n);
            size_densest_core_prefix = n;
        }
        
    }
    
    // Outputs :
    output[0] = core_max; // maximal core of graph
    output[1] = average_degree_density; // average degree density
    output[2] = edge_density; // edge density
    output[3] = size_densest_core_prefix; // size of the densest core prefix

    return ;
}

int main(int argc,char** argv){
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    // Choose your graph :
    char* path_amazon = "com-amazon.ungraph.txt";
    char* path_orkut = "com-orkut.ungraph.txt";
    char* path_lj = "com-lj.ungraph.txt";
    char* path_friendster = "com-friendster.ungraph.txt";
    
    char* filename = path_friendster;
    
    adjlist* g = readedgelist(filename);
    printf("Number of nodes: %lu\n",g->n);
    printf("Number of edges: %lu\n",g->e);
    mkadjlist(g);

    // create the outputs :
    long double* output = malloc(4*sizeof(long double));
    densest_subgraph(g,output);
    
    // Print the outputs :
    printf("Core of the graph : %Lg\n",output[0]);
    printf("average degree density : %.4Lf\n",output[1]);
    printf("edge density : %.4Lf\n",output[2]);
    printf("size of the densest core ordering prefix: %Lg\n",output[3]);

    free(output);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time spend : %f seconds\n",cpu_time_used);
    return 0;
}
