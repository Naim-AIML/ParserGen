#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

// FIRST and FOLLOW sets
bool first[MAX][MAX];
bool follow[MAX][MAX];
bool nullable[MAX];

// Grammar
int prods_lhs[MAX];
int prods_rhs[MAX][MAX];
int rhs_len[MAX];
int num_prods;

// ----------------------------------
// Compute FIRST sets
// ----------------------------------
void compute_first(int num_symbols) {
    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i < num_prods; i++) {
            int A = prods_lhs[i];
            bool all_nullable = true;

            for (int j = 0; j < rhs_len[i]; j++) {
                int X = prods_rhs[i][j];

                // Add FIRST(X) to FIRST(A)
                for (int t = 0; t < num_symbols; t++) {
                    if (first[X][t] && !first[A][t]) {
                        first[A][t] = true;
                        changed = true;
                    }
                }

                if (!nullable[X]) {
                    all_nullable = false;
                    break;
                }
            }

            if (all_nullable && !nullable[A]) {
                nullable[A] = true;
                changed = true;
            }
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

// FIRST and FOLLOW sets
bool first[MAX][MAX];
bool follow[MAX][MAX];
bool nullable[MAX];

// Grammar
int prods_lhs[MAX];
int prods_rhs[MAX][MAX];
int rhs_len[MAX];
int num_prods;

// ----------------------------------
// Compute FIRST sets
// ----------------------------------
void compute_first(int num_symbols) {
    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i < num_prods; i++) {
            int A = prods_lhs[i];
            bool all_nullable = true;

            for (int j = 0; j < rhs_len[i]; j++) {
                int X = prods_rhs[i][j];

                // Add FIRST(X) to FIRST(A)
                for (int t = 0; t < num_symbols; t++) {
                    if (first[X][t] && !first[A][t]) {
                        first[A][t] = true;
                        changed = true;
                    }
                }

                if (!nullable[X]) {
                    all_nullable = false;
                    break;
                }
            }

            if (all_nullable && !nullable[A]) {
                nullable[A] = true;
                changed = true;
            }
        }
    }
}

// ----------------------------------
// Compute FOLLOW sets
// ----------------------------------
void compute_follow(int start_symbol, int num_symbols) {
    follow[start_symbol]['$'] = true;

    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i < num_prods; i++) {
            int A = prods_lhs[i];

            for (int j = 0; j < rhs_len[i]; j++) {
                int B = prods_rhs[i][j];

                bool all_nullable = true;

                // Look ahead symbols β
                for (int k = j + 1; k < rhs_len[i]; k++) {
                    int beta = prods_rhs[i][k];

                    for (int t = 0; t < num_symbols; t++) {
                        if (first[beta][t] && !follow[B][t]) {
                            follow[B][t] = true;
                            changed = true;
                        }
                    }

                    if (!nullable[beta]) {
                        all_nullable = false;
                        break;
                    }
                }

                // If β -> ε
                if (all_nullable) {
                    for (int t = 0; t < num_symbols; t++) {
                        if (follow[A][t] && !follow[B][t]) {
                            follow[B][t] = true;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}


