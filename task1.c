#include <stdio.h>
#include <string.h>
#include <ctype.h> // for isalpha()

#define max 100

// ---------------- Stack Implementation ----------------
char stack[max];
int top = -1;

// Function to push onto stack
void stackPush(char i)
{
    stack[++top] = i;
}

// Function to pop from the stack
char stackPop()
{
    if (top == -1)
        return '\0'; // Return null character if stack empty
    else
        return stack[top--];
}

// Check if stack is empty
int stackIsEmpty()
{
    return top == -1;
}

// ---------------- Precedence Function ----------------
int priority(char operation)
{
    if (operation == '(')
        return 1;
    if (operation == '~')
        return 2;
    if (operation == '*')
        return 3;
    if (operation == '+')
        return 4;
    if (operation == '>')
        return 5;
    return 0;
}

// ---------------- Infix to Prefix Conversion ----------------
/**
 * @brief Key Rule: 
 * We cannot have a lower precedence operator on top of a higher precedence operator.
 * Also, we cannot have 2 same precedence operators together.
 * So we keep popping until it’s valid to push.
 * If we have a closing bracket, we pop until an opening bracket is found.
 * 
 * Steps:
 *  1. Swap '(' with ')' and vice versa.
 *  2. Reverse the input.
 *  3. Convert using infix→postfix logic.
 *  4. Reverse the output to get prefix.
 */
void inFixToPreFix(char *input, char *outputToChar)
{
    char output[max];
    int outputCounter = 0;

    // Step 1: Swap brackets
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '(')
            input[i] = ')';
        else if (input[i] == ')')
            input[i] = '(';
    }

    // Step 2: Reverse the string
    int len = strlen(input);
    for (int i = 0; i < len / 2; i++)
    {
        char tempChar = input[i];
        input[i] = input[len - i - 1];
        input[len - i - 1] = tempChar;
    }

    // Step 3: Process the reversed infix
    char temp;
    int t = 0;
    while ((temp = input[t++]) != '\0')
    {
        if (isalpha(temp)) // Operand
        {
            output[outputCounter++] = temp;
        }
        else // Operator
        {
            if (temp == '(')
            {
                stackPush(temp);
            }
            else if (temp == ')')
            {
                // Pop until '('
                while (!stackIsEmpty() && stack[top] != '(')
                {
                    output[outputCounter++] = stackPop();
                }
                stackPop(); // Pop '('
            }
            else
            {
                while (!stackIsEmpty() && priority(stack[top]) <= priority(temp))
                {
                    output[outputCounter++] = stackPop();
                }
                stackPush(temp);
            }
        }
    }

    // Pop remaining operators
    while (!stackIsEmpty())
    {
        output[outputCounter++] = stackPop();
    }

    output[outputCounter] = '\0';

    // Step 4: Reverse output to get prefix
    int k = 0;
    printf("Output in PreOrder (Prefix) Notation: ");
    for (int i = outputCounter - 1; i >= 0; i--)
    {
        printf("%c", output[i]);
        outputToChar[k++] = output[i];
    }
    outputToChar[k] = '\0';
    printf("\n");
}

// ---------------- Main Function ----------------
/*int main()
{
    char input[max];
    char output[max];

    printf("Enter Infix Expression: ");
    scanf("%s", input);

    inFixToPreFix(input, output);

    printf("Prefix Expression Stored: %s\n", output);

    return 0;
}*/
