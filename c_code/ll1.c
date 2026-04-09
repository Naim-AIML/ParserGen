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

// ----------------------------------
// Check if production matches terminal
// ----------------------------------
bool matches(int prod_index, int terminal) {
    bool can_derive = false;
    bool nullable_rhs = true;

    for (int j = 0; j < rhs_len[prod_index]; j++) {
        int X = prods_rhs[prod_index][j];

        if (first[X][terminal]) {
            can_derive = true;
        }

        if (!nullable[X]) {
            nullable_rhs = false;
            break;
        }
    }

    if (can_derive) return true;

    return nullable_rhs && follow[prods_lhs[prod_index]][terminal];
}

// ----------------------------------
// Build LL(1) table
// ----------------------------------
void build_ll1_table(int num_symbols) {
    bool is_ll1 = true;

    for (int A = 0; A < num_symbols; A++) {
        for (int t = 0; t < num_symbols; t++) {

            int chosen = -1;

            for (int p = 0; p < num_prods; p++) {
                if (prods_lhs[p] == A && matches(p, t)) {

                    if (chosen == -1) {
                        chosen = p;
                    } else {
                        // Conflict!
                        is_ll1 = false;
                    }
                }
            }

            if (chosen != -1) {
                printf("M[%d, %d] = production %d\n", A, t, chosen);
            }
        }
    }

    if (is_ll1)
        printf("Grammar is LL(1)\n");
    else
        printf("Grammar is NOT LL(1)\n");
}


// ------------------- Main Function ---------------------- //
int main() {
    int n;
    char lhs_str[10], arrow[5], rhs_str[MAX];

    printf("Enter number of productions: ");
    scanf("%d", &num_prods);

    // Initialize terminals in FIRST set (base case: FIRST(c) = {c})
    for (int i = 0; i < MAX; i++) {
        first[i][i] = true;
    }

    printf("Enter productions (e.g., 'S -> aB' or 'B -> #'):\n");
    for (int i = 0; i < num_prods; i++) {
        // Scans LHS, then the arrow "->", then the RHS string
        scanf("%s %s %s", lhs_str, arrow, rhs_str);

        prods_lhs[i] = lhs_str[0];

        if (rhs_str[0] == '#') {
            rhs_len[i] = 0;
            nullable[prods_lhs[i]] = true;
        } else {
            rhs_len[i] = strlen(rhs_str);
            for (int j = 0; j < rhs_len[i]; j++) {
                prods_rhs[i][j] = rhs_str[j];
            }
        }
    }

    // Perform computations
    compute_first(MAX);
    compute_follow(prods_lhs[0], MAX);

    // --- DISPLAY FIRST SETS ---
    printf("\n--- FIRST SETS ---\n");
    for (int i = 0; i < MAX; i++) {
        bool has_first = false;
        // Check if 'i' is a non-terminal that appeared in the LHS
        for (int p = 0; p < num_prods; p++) if (prods_lhs[p] == i) has_first = true;

        if (has_first) {
            printf("FIRST(%c) = { ", i);
            if (nullable[i]) printf("# ");
            for (int t = 0; t < MAX; t++) {
                // Only print terminals (non-uppercase or specific terminal chars)
                if (first[i][t] && i != t) {
                    printf("%c ", t);
                }
            }
            printf("}\n");
        }
    }

    // --- DISPLAY FOLLOW SETS ---
    printf("\n--- FOLLOW SETS ---\n");
    for (int i = 0; i < MAX; i++) {
        bool has_follow = false;
        for (int p = 0; p < num_prods; p++) if (prods_lhs[p] == i) has_follow = true;

        if (has_follow) {
            printf("FOLLOW(%c) = { ", i);
            for (int t = 0; t < MAX; t++) {
                if (follow[i][t]) {
                    printf("%c ", t);
                }
            }
            printf("}\n");
        }
    }

    printf("\n--- LL(1) Table Construction ---\n");
    build_ll1_table(MAX);

    return 0;
}