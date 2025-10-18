
// cnf_reader.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cnfReader.h"

#define BUF_SZ 2048
#define OUT_MAX 2000000

char *cnfToInfix(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    char *out = (char*) malloc(OUT_MAX);
    if (!out) { fclose(f); return NULL; }
    out[0] = '\0';

    char line[BUF_SZ];
    int firstClause = 1;

    while (fgets(line, sizeof(line), f) != NULL) {
        // Skip comments and problem line
        char *ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (*ptr == 'c' || *ptr == 'p' || *ptr == '\n' || *ptr == '\0') continue;

        // Process clause line(s). CNF may break clause across lines (rare). We'll tokenise ints.
        // Use strtok on the line for integers
        char *tok = strtok(line, " \t\r\n");
        if (!tok) continue;
        int lit;
        // Start building the clause text
        char clause[BUF_SZ];
        clause[0] = '\0';
        int firstLit = 1;

        while (tok) {
            lit = atoi(tok);
            if (lit == 0) break; // clause ended
            char litstr[64];
            if (lit > 0) snprintf(litstr, sizeof(litstr), "x%d", lit);
            else snprintf(litstr, sizeof(litstr), "~x%d", -lit);
            if (!firstLit) strncat(clause, " + ", sizeof(clause)-strlen(clause)-1);
            strncat(clause, litstr, sizeof(clause)-strlen(clause)-1);
            firstLit = 0;
            tok = strtok(NULL, " \t\r\n");
        }

        if (clause[0] != '\0') {
            if (!firstClause) strncat(out, " * ", OUT_MAX - strlen(out) - 1);
            strncat(out, "(", OUT_MAX - strlen(out) - 1);
            strncat(out, clause, OUT_MAX - strlen(out) - 1);
            strncat(out, ")", OUT_MAX - strlen(out) - 1);
            firstClause = 0;
        }
    }

    fclose(f);
    // possible trim: if out length large, still valid; caller must be aware
    return out;
}
