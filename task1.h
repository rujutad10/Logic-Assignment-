/**
 * @file task1.h
 * @brief Header for infix-to-prefix conversion functions.
 */

#ifndef TASK1_H
#define TASK1_H

/**
 * @brief Converts a fully parenthesized infix logical expression to prefix notation.
 *
 * Supported operators:
 * - '~' : NOT
 * - '*' : AND
 * - '+' : OR
 * - '>' : IMPLIES
 *
 * Example:
 * Input:  (A+B)*C
 * Output: * + A B C
 *
 * @param inputInfix  Input infix expression (as a null-terminated string)
 * @param outputPrefix  Output buffer for the resulting prefix expression
 */
void inFixToPreFix(char *inputInfix, char *outputPrefix);

#endif
