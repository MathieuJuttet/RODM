#include  "label_propagation.h"

unsigned long randint(unsigned long min, unsigned long max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

unsigned long new_label(const unsigned long arr[], unsigned long n, adjlist* g, unsigned long u, unsigned long adj_labels[]) {
    unsigned long max = 0;
    int size_argmax = 0;
    unsigned long* argmaxs = malloc(n*sizeof(unsigned long));
    unsigned long* maxs = malloc(n*sizeof(unsigned long));
    //Compute argmaxs
    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
            size_argmax = 1;
            argmaxs[0] = i;
            maxs[0] = arr[i];
        } else if (arr[i] == max) {
            argmaxs[size_argmax] = i;
            maxs[size_argmax] = arr[i];
            size_argmax++;
        }
    }
    int same_label = 0;
    for(int i =0; i < size_argmax; i++) {
        if(adj_labels[argmaxs[i]] == g->labels[u]) {
            same_label = 1;
        }
    }
    if (same_label == 0) {
        if (size_argmax == 1) {
            return adj_labels[argmaxs[0]];
        } else {
            return adj_labels[argmaxs[randint(0, size_argmax)]];
        }
    }
    return g->labels[u];
}

unsigned long* count_freq(const unsigned long arr[], unsigned long n) {
    int* visited = calloc(n, sizeof(int));
    unsigned long* freqs = calloc(n, sizeof(unsigned long));

    for (int i = 0; i < n; i++) {
        if (visited[i] == 1)
            continue;
        int count = 1;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                visited[j] = 1;
                count++;
            }
        }
        freqs[i] = count;
    }
    free(visited);
    return freqs;
}

void fisher_yates_shuffle(unsigned long *arr, unsigned long n) {
    unsigned long j, temp;
    for (int i= 0 ; i < n - 2; i++){
        j = randint(i, n);
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void label_propagation(adjlist* g, char *output) {
    unsigned long i, u;
    uint8_t communities_changed = 0;
    unsigned long* order = malloc(g->n*sizeof(unsigned long));
    memcpy(order, g->labels, g->n*sizeof(unsigned long));
    do {
        communities_changed = 0;
        fisher_yates_shuffle(order, g->n);
        for (i = 0; i < g->n; i++) {
            u = order[i];
            unsigned long n_adj = g->cd[u + 1] - g->cd[u];
            unsigned long *adj_labels = malloc(n_adj*sizeof(unsigned long));
            for (unsigned long k = 0; k < n_adj; k++) {
                adj_labels[k] = g->labels[g->adj[g->cd[u] + k]];
            }
            unsigned long* freqs = count_freq(adj_labels, n_adj);
            unsigned long new_lab = new_label(freqs, n_adj, g, u, adj_labels);
            if (g->labels[u] != new_lab) {
                communities_changed = 1;
            }
            g->labels[u] = new_lab;
        }
    } while (communities_changed);

    FILE *fp = fopen(output,"w");
    for(i = 0; i < g->n; i++) {
        fprintf (fp, "%lu\n", g->labels[i]);
    }
    fclose (fp);
}

int main(int argc,char** argv) {
    srand(time(0));
    time_t t1,t2;

    t1 = time(NULL);

    int n = 400;
    int nb_clusters = 4;
    double p = 0.9;
    double q = 0.1;

    char *output = "../Graphs/random_graph.txt";
    generate_graph(n, nb_clusters, p, q, output);
    adjlist* g = readedgelist(output);
    mkadjlist(g);
    printf("Number of nodes: %lu\n", g->n);
    output = "../Graphs/labels.txt";
    label_propagation(g, output);
    t2 = time(NULL);
    printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
    return 0;
}
