#include <memory.h>
#include  "pagerank.h"

float * matvectprod(adjlist * g, const float* P) {
    float* TP = calloc(g->n, sizeof(float));
    for (int i = 0; i < g->e; i++) {
        edge e = g->edges[i];
        unsigned  long s = e.s;
        unsigned  long t = e.t;
        TP[t] += P[s]/(float)g->cd[s];
    }
    return TP;
}

void normalize2(float * p, unsigned long n, float norm_1) {
    float normalizer = (1 - norm_1) / (float)n;
    for (int k = 0; k < n; k++){
        p[k] += normalizer;
    }
}

float* power_iteration(adjlist * g, float alpha, int t) {
    float* p = calloc(g->n, sizeof(float));
    for (int i = 0; i < g->n; i++) {
        p[i] = (float)1. / (float)g->n;
    }
    float alpha_n = alpha/(float)g->n;
    float comp_alpha = (1-alpha);
    for (int i = 0; i < t; i++) {
        p = matvectprod(g, p);
        float norm_1 = 0;
        for (int k = 0; k < g->n; k++){
            p[k] = comp_alpha*p[k] + alpha_n;
            norm_1 += p[k];
        }
        normalize2(p, g->n, norm_1);
    }
    return p;
}

void k_lowest_highest(const float* p, unsigned long n, unsigned long* highest_nodes, unsigned long* lowest_nodes) {
    float highest_values[CONST_K];
    float lowest_values[CONST_K] = {[0 ... CONST_K - 1] = 1.};
    for (int i = 0; i <= n; i++) {
        if (lowest_values[0] > p[i]) {
            lowest_values[0] = p[i];
            lowest_nodes[0] = i;
            for (int j = 0; j < CONST_K - 1; j++) {
                if (lowest_values[j] <= lowest_values[j + 1]) {
                    float temp_value = lowest_values[j];
                    lowest_values[j] = lowest_values[j + 1];
                    lowest_values[j + 1] = temp_value;
                    unsigned long temp_node = lowest_nodes[j];
                    lowest_nodes[j] = lowest_nodes[j + 1];
                    lowest_nodes[j + 1] = temp_node;
                } else {
                    break;
                }

            }
        }

        if (highest_values[0] <= p[i]) {
            highest_values[0] = p[i];
            highest_nodes[0] = i;
            for (int j = 0; j < CONST_K - 1; j++) {
                if (highest_values[j] >= highest_values[j + 1]) {
                    float temp_value = highest_values[j];
                    highest_values[j] = highest_values[j + 1];
                    highest_values[j + 1] = temp_value;
                    unsigned long temp_node = highest_nodes[j];
                    highest_nodes[j] = highest_nodes[j + 1];
                    highest_nodes[j + 1] = temp_node;
                } else {
                    break;
                }
            }
        }


    }
}

void print_nodes(const unsigned long* highest_nodes, const unsigned long* lowest_nodes, adjlist* g) {
    unsigned long node;
    char name[1000];
    for (int i = 0; i < CONST_K; i++) {
        FILE *file=fopen(CONST_NAME_NODES,"r");
        while (fscanf(file,"%lu %1000[^\n]", &node, name) == 2) {
            if(node == g->pros_to_orig[highest_nodes[CONST_K - i - 1]]) {
                printf("%i hightest ranked page : %s (node %lu)\n", CONST_K - i, name, g->pros_to_orig[highest_nodes[CONST_K - i - 1]]);
                break;
            }
        }
    }
    for (int i = 0; i < CONST_K + 1; i++) {
        FILE *file=fopen(CONST_NAME_NODES,"r");
        while (fscanf(file,"%lu %1000[^\n]", &node, name)==2) {
            if(node == g->pros_to_orig[lowest_nodes[i]]) {
                printf("%i lowest ranked page : %s (node %lu)\n", i, name, g->pros_to_orig[lowest_nodes[i]]);
                break;
            }
        }
    }
    //printf(g->pros_to_orig[lowest_nodes[CONST_K - 5 - 1]]);
}

int search_deg_int(adjlist* g, int node_number){
    int deg_int_node = 0;
    for(int i =0; i<g->e; i++){
        if(g->edges[i].t == node_number){
            deg_int_node = deg_int_node + 1;
        }
    }
    return deg_int_node;
}


void save_deg_out(adjlist* g, float* p){
    FILE *fp = fopen("file_test.txt","w");
    for (int i = 0; i < g->n; i++){
        fprintf (fp, "%e %d\n", p[i], g->cd[i]);
    }
    fclose (fp);
}

void save_double_page_rank(adjlist* g,float* p1, float* p2){
    FILE *fp = fopen("double_page_rank.txt","w");
    for (int i = 0; i < g->n; i++){
        fprintf (fp, "%e %e\n", p1[i], p2[i]);
    }
    fclose (fp);
}

void save_deg_in_out(adjlist* g, float* p){
    FILE *fp = fopen("file_test.txt","w");
    for (int i = 0; i < g->n; i++){
        fprintf (fp, "%e %d\n", p[i], search_deg_int(g, i));
    }
    fclose (fp);
}

int main(int argc,char** argv){
    float alpha = (float) 0.99;
    time_t t1,t2;
    int t = 5;
    adjlist* g = readedgelist(argv[1]);
    mkadjlist(g);
    printf("Number of nodes: %lu\n", g->n);
    t1 = time(NULL);
    float* p = power_iteration(g, alpha, t);
    unsigned long highest_nodes[CONST_K] = {0};
    unsigned long lowest_nodes[CONST_K] = {0};
    k_lowest_highest(p, g->n, highest_nodes, lowest_nodes);
    print_nodes(highest_nodes, lowest_nodes, g);

    float alpha = (float) 0.15;
    float alpha2 = (float) 0.9;
    int t = 5;
    adjlist* g = readedgelist("./Graphs/com-amazon.ungraph.txt");
    mkadjlist(g);
    printf("Number of nodes: %lu\n", g->n);
    printf("Number of edges: %lu\n", g->e);
    float* p = power_iteration(g, alpha, t);
    float* p2 = power_iteration(g, alpha2, t);

    save_double_page_rank(g, p, p2);

    //save_deg_in_out(g, p);
    printf("%lu\n", g->cd);
    printf("%lu\n", g->n);
    printf("%lu\n", g->e);
    printf("taille du tableau : %lu\n", g->e);

    t2 = time(NULL);
    printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
    return 0;
}