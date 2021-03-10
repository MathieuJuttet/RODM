#include "triangles.h"

void getIntersect(unsigned long u, unsigned long v, const unsigned long *adj, const unsigned long *cd, unsigned long *count, triangle *list_triangles) {
    unsigned long target1 = cd[v];
    unsigned long target2 = cd[u];
    unsigned long end1 = cd[v + 1];
    unsigned long end2 = cd[u + 1];
    while(target1 < end1 && target2 < end2) {
        if (adj[target1] == adj[target2]) {
            list_triangles[*count] = (triangle) { .node_1 = u, .node_2 = v, .node_3 = adj[target1] };
            *count = *count + 1;
            if (*count % NTRIANGLES == 0) {
                list_triangles=realloc(list_triangles,(*count + NTRIANGLES)*sizeof(triangle));
            }
            target1++;
            target2++;
        } else if (adj[target1] > adj[target2]){
            while (adj[target1] > adj[target2] && target2 < end2) {
                target2++;
            }
        } else {
            while (adj[target1] < adj[target2] && target1 < end1) {
                target1++;
            }
        }
    }
}

void triangles(adjlist* g, unsigned long *count, triangle *list_triangles) {
    unsigned long u, v;
    for (int e = 0; e < g->e; e++) {
        u = g->edges[e].s;
        v = g->edges[e].t;
        getIntersect(u, v, g->adj, g->cd, count, list_triangles);
    }
    list_triangles=realloc(list_triangles,*count*sizeof(triangle));
    *count = *count/3;
}

int main(int argc,char** argv){
    unsigned long count = 0;
    time_t t1,t2;
    adjlist* g;
    g=readedgelist(argv[1]);
    mkadjlist(g);

    t1 = time(NULL);
    triangle *list_triangles = malloc(NTRIANGLES*sizeof(triangle));
    triangles(g, &count, list_triangles);
    t2 = time(NULL);

    printf("Number of triangles: %lu\n", count);
    printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
    triangle t = list_triangles[0];
    printf("Triangle 0 : %lu %lu %lu \n", t.node_1, t.node_2, t.node_3);
    triangle tt = list_triangles[1];
    printf("Triangle 1 : %lu %lu %lu \n", tt.node_1, tt.node_2, tt.node_3);

    free(list_triangles);
    return 0;
}
