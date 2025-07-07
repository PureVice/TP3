// A union-find algorithm to identify islands
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <getopt.h> 

typedef struct subset {
    int parent;
    int rank;
} subset;

// A utility function to find set of an element i (with path compression)
int Find(subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = Find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// A function that does union of two sets of x and y (uses union by rank)
void Union(subset subsets[], int x, int y) {
    int xroot = Find(subsets, x);
    int yroot = Find(subsets, y);

    if (xroot == yroot) return;

    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int rndnode(int nodes) {
    return (int)(drand48() * nodes);
}

void print_usage(char *prog_name) {
    fprintf(stderr, "Uso: %s [-s seed] [-n nós] [-p pacotes] [-c clientes] [-t tempo]\n", prog_name);
}

int main(int argc, char *argv[]) {
    // Default values
    long seed = 1;
    int nodes = 10;
    int numpackets = 100;
    int numclients = 10;
    long rtime = 10;
    int verbose = 0;

    int opt;
    while ((opt = getopt(argc, argv, "s:n:p:c:t:v")) != -1) {
        switch (opt) {
            case 's': seed = atol(optarg); break;
            case 'n': nodes = atoi(optarg); break;
            case 'p': numpackets = atoi(optarg); break;
            case 'c': numclients = atoi(optarg); break;
            case 't': rtime = atol(optarg); break;
            case 'v': verbose = 1; break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    if(verbose) fprintf(stderr, "[DEBUG] genwkl3_extra iniciado com seed=%ld, nós=%d, pacotes=%d, clientes=%d\n", seed, nodes, numpackets, numclients);

    if (nodes <= 1) {
        fprintf(stderr, "Erro: O número de nós deve ser maior que 1.\n");
        exit(EXIT_FAILURE);
    }

    long atime = 1;
    int src, dst, sender, receiver;
    long qtime, qtime_end;
    
    FILE *out = fopen("tp3.in", "wt");
    if (!out) { perror("Erro ao abrir tp3.in"); exit(EXIT_FAILURE); }

    srand48(seed);

    if(verbose) fprintf(stderr, "[DEBUG] Gerando %d pacotes...\n", numpackets);
    for (int i = 0; i < numpackets; i++) {
        do { src = rndnode(nodes); dst = rndnode(nodes); } while (src == dst);
        atime += (long)(drand48() * rtime) + 1;

        do { sender = rndnode(numclients); receiver = rndnode(numclients); } while (sender == receiver && numclients > 1);
        fprintf(out, "%.7ld EV RG %.3d n%.5d n%.5d %.3d %.3d\n",
                atime, i, sender, receiver, src, dst);
    }

    int numcl = (numclients > 0) ? 10 : 0;
    int numpc = (numpackets > 0) ? 10 : 0;
    int num_ma = 5; // Número de consultas de Movimentação em Armazém
    int num_rc = 5; // Número de consultas de Rotas Congestionadas

    if(verbose) fprintf(stderr, "[DEBUG] Gerando consultas...\n");

    for (int i = 0; i < numcl; i++) {
        qtime = (long)(drand48() * atime);
        sender = rndnode(numclients);
        fprintf(out, "%.7ld CL n%.5d\n", qtime, sender);
    }
    
    for (int i = 0; i < numpc; i++) {
        qtime = (long)(drand48() * atime);
        sender = rndnode(numpackets);
        fprintf(out, "%.7ld PC %.3d\n", qtime, sender);
    }

    // Gerar consultas MA (Movimentação em Armazém)
    for (int i = 0; i < num_ma; i++) {
        qtime = (long)(drand48() * atime);
        qtime_end = qtime + (long)(drand48() * (atime - qtime));
        int armazem = rndnode(nodes);
        fprintf(out, "%.7ld MA %.7ld %.7ld %.3d\n", qtime, qtime, qtime_end, armazem);
    }

    // Gerar consultas RC (Rotas Congestionadas)
    for (int i = 0; i < num_rc; i++) {
        qtime = (long)(drand48() * atime);
        fprintf(out, "%.7ld RC\n", qtime);
    }


    if(verbose) fprintf(stderr, "[DEBUG] Geração de dados concluída. Fechando arquivo.\n");
    fclose(out);

    return 0;
}