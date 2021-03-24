#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void generate_graph(int nodes, int clusters, double p, double q, char *output){
  FILE *fp = fopen(output,"w");
  fprintf (fp, "node1 node2\n");
  int *g = calloc(nodes, sizeof(int));

  for (int i =0; i < nodes; i++){
    g[i] = rand() % clusters;
  }

  for (int i = 0; i < nodes - 1; i++){
    for (int j = i + 1; j < nodes; j++){
      if (g[i] == g[j] && rand()/(double)RAND_MAX < p){
          fprintf (fp, "%u %u\n", i, j);
          fprintf (fp, "%u %u\n", j, i);
      }
      else if(rand()/(double)RAND_MAX < q){
          fprintf (fp, "%u %u\n", i, j);
          fprintf (fp, "%u %u\n", j, i);
      }
    }
  }
  fclose (fp);
}

int main(int argc,char** argv){
  int nodes = 400;
  double p = 0.1;
  double q = 0.1;
  int clusters = 4;
  char *output = "Graphs/random_graph.txt";

  generate_graph(nodes, clusters, p, q, output);
	return 0;
}