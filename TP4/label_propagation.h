#include  "adjarray.h"
#include <memory.h>

unsigned long randint(unsigned long min, unsigned long max);
unsigned long new_label(const unsigned long arr[], unsigned long n, adjlist* g, unsigned long u, unsigned long adj_labels[]);
unsigned long* count_freq(const unsigned long arr[], unsigned long n);
void fisher_yates_shuffle(unsigned long *arr, unsigned long n);
void label_propagation(adjlist* g, char *output);
void generate_graph(int n, int nb_clusters, double p, double q, char *output);
