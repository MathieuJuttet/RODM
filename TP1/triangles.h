#include  "adjarray.h"

#define NTRIANGLES 30000000

typedef struct {
    unsigned long node_1;
    unsigned long node_2;
    unsigned long node_3;
} triangle;

void getIntersect(unsigned long u, unsigned long v, const unsigned long *adj, const unsigned long *cd, unsigned long *count, triangle *list_triangles);
void triangles(adjlist* g, unsigned long *count, triangle *list_triangles);
