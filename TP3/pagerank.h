#include  "adjarray.h"
#define CONST_K 5

float* power_iteration(adjlist * G, float alpha, int t);
float* matvectprod(adjlist * g, const float* P);
void normalize2(float * p, unsigned long n, float norm_1);
float* power_iteration(adjlist * g, float alpha, int t);
void k_lowest_highest(const float* p, unsigned long n, unsigned long* highest_nodes, unsigned long* lowest_nodes);
void print_nodes(const unsigned long* highest_nodes, const unsigned long* lowest_nodes, adjlist* g);