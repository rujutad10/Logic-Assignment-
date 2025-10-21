#ifndef CNF_READER_H
#define CNF_READER_H

/**
 * @brief Converts a CNF file to a string in infix notation.
 *
 * Reads a DIMACS CNF file and produces a string representing the formula
 * in infix form. Literals are represented as xN for positive literals
 * and ~xN for negated literals. Clauses are combined using `*` (AND),
 * and literals within a clause are combined using `+` (OR).
 *
 * @param filename Path to the CNF file.
 * @return Malloc'ed string containing the infix formula. Caller must free.
 *         Returns NULL on error (file not found or malloc failure).
 */
char *cnfToInfix(const char *filename);

#endif
