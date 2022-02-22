/*
    This is the lex.c file for the UCF Fall 2021 Systems Software Project.
    For HW2, you must implement the function lexeme *lexanalyzer(char *input).
    You may add as many constants, global variables, and support functions
    as you desire.

    If you choose to alter the printing functions or delete list or lex_index,
    you MUST make a note of that in you readme file, otherwise you will lose
    5 points.
*/
// TEAM: Anthony Galbo, Connor Cabrera, Eduardo Bourget, Luis Curtiellas
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;		//list of lexemes with their .name .value and .type
int lex_index = 0;	//total number of lexemes found

void printlexerror(int type);
void printtokens();

lexeme *lexanalyzer(char *input)
{
    list = malloc(sizeof(struct lexeme) * MAX_NUMBER_TOKENS);
    int inputLength = strlen(input);

    if (inputLength == 0)
    {
        return NULL;
    }

    int startIndex = 0;

    for (int endIndex = 0; endIndex < inputLength; endIndex++)
    {
        startIndex = endIndex;
        if (isalpha(input[endIndex]))
        {
            while ((isalpha(input[endIndex]) || isdigit(input[endIndex])) && (endIndex < inputLength - 1))
            {
                endIndex++;
            }
            if (endIndex - startIndex > MAX_IDENT_LEN)
            {
                printlexerror(4);
                exit(0);
            }
            else
            {
                strncpy(list[lex_index].name, input + startIndex, endIndex - startIndex);

                // Assigns the correct tokentype to the list to be returned
                // If its not a reserved word, it will be assigned and returned as 'identsym'
                if (strcmp(list[lex_index].name, "const") == 0)
                {
                    list[lex_index].type = constsym;
                }
                else if (strcmp(list[lex_index].name, "var") == 0)
                {
                    list[lex_index].type = varsym;
                }
                else if (strcmp(list[lex_index].name, "procedure") == 0)
                {
                    list[lex_index].type = procsym;
                }
                else if (strcmp(list[lex_index].name, "begin") == 0)
                {
                    list[lex_index].type = beginsym;
                }
                else if (strcmp(list[lex_index].name, "end") == 0)
                {
                    list[lex_index].type = endsym;
                }
                else if (strcmp(list[lex_index].name, "while") == 0)
                {
                    list[lex_index].type = whilesym;
                }
                else if (strcmp(list[lex_index].name, "do") == 0)
                {
                    list[lex_index].type = dosym;
                }
                else if (strcmp(list[lex_index].name, "if") == 0)
                {
                    list[lex_index].type = ifsym;
                }
                else if (strcmp(list[lex_index].name, "then") == 0)
                {
                    list[lex_index].type = thensym;
                }
                else if (strcmp(list[lex_index].name, "else") == 0)
                {
                    list[lex_index].type = elsesym;
                }
                else if (strcmp(list[lex_index].name, "call") == 0)
                {
                    list[lex_index].type = callsym;
                }
                else if (strcmp(list[lex_index].name, "write") == 0)
                {
                    list[lex_index].type = writesym;
                }
                else if (strcmp(list[lex_index].name, "read") == 0)
                {
                    list[lex_index].type = readsym;
                }
                else if (strcmp(list[lex_index].name, "odd") == 0)
                {
                    list[lex_index].type = oddsym;
                }
                else
                {
                    list[lex_index].type = identsym;
                }

                lex_index++;
            }
        }
        if (isdigit(input[endIndex]))
        {
            while (isdigit(input[endIndex]) && (endIndex < inputLength - 1))
            {
                endIndex++;
            }
            if (endIndex - startIndex > MAX_NUMBER_LEN)
            {
                printlexerror(3);
                exit(0);
            }
            else if (endIndex < inputLength - 1)
            {
                if (isalpha(input[endIndex + 1]))
                {
                    printlexerror(2);
                    exit(0);
                }
            }
            strncpy(list[lex_index].name, input + startIndex, endIndex - startIndex);
            list[lex_index].type = numbersym;
            list[lex_index].value = atoi(list[lex_index].name);
            lex_index++;
        }

        // Assigns/returns a token reulting from the parsing of a special character
        // Raises an exception error if an invalid symbol is encountered
        switch (input[endIndex])
        {
        // Differentiates between a division symbol and an in line comment
        case '/':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '/')
            {
              while ((input[endIndex] != '\n') && (endIndex < inputLength - 1))
              {
                endIndex++;
              }
            }
            else
            {
              list[lex_index].name[0] = '/';
              list[lex_index].type = divsym;
              lex_index++;
            }
            break;

        case '+':
            list[lex_index].name[0] = '+';
            list[lex_index].type = addsym;
            lex_index++;
            break;

        case '-':
            list[lex_index].name[0] = '-';
            list[lex_index].type = subsym;
            lex_index++;
            break;

        case '*':
            list[lex_index].name[0] = '*';
            list[lex_index].type = multsym;
            lex_index++;
            break;

        case '%':
            list[lex_index].name[0] = '%';
            list[lex_index].type = modsym;
            lex_index++;
            break;

        case '(':
            list[lex_index].name[0] = '(';
            list[lex_index].type = lparensym;
            lex_index++;
            break;

        case ')':
            list[lex_index].name[0] = ')';
            list[lex_index].type = rparensym;
            lex_index++;
            break;

        case ',':
            list[lex_index].name[0] = ',';
            list[lex_index].type = commasym;
            lex_index++;
            break;

        case '.':
            list[lex_index].name[0] = '.';
            list[lex_index].type = periodsym;
            lex_index++;
            break;

        case ';':
            list[lex_index].name[0] = ';';
            list[lex_index].type = semicolonsym;
            lex_index++;
            break;

        // We can have a single '<' or a '<='
        // Making appropiate cases for each of them
        case '<':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '=')
            {
                list[lex_index].name[0] = '<';
                list[lex_index].name[1] = '=';
                list[lex_index].type = leqsym;
                lex_index++;
                endIndex++;
            }
            else
            {
                list[lex_index].name[0] = '<';
                list[lex_index].type = lsssym;
                lex_index++;
            }
            break;

        // We could have a single '>' or a '>='
        // Make appropiate cases for both of them
        case '>':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '=')
            {
                list[lex_index].name[0] = '>';
                list[lex_index].name[1] = '=';
                list[lex_index].type = geqsym;
                lex_index++;
                endIndex++;
            }
            else
            {
                list[lex_index].name[0] = '>';
                list[lex_index].type = gtrsym;
                lex_index++;
            }
            break;

        case ':':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '=')
            {
                list[lex_index].name[0] = ':';
                list[lex_index].name[1] = '=';
                list[lex_index].type = assignsym;
                lex_index++;
                endIndex++;
            }
            else
            {
                printlexerror(1);
                exit(0);
            }
            break;

        case '=':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '=')
            {
                list[lex_index].name[0] = '=';
                list[lex_index].name[1] = '=';
                list[lex_index].type = eqlsym;
                lex_index++;
                endIndex++;
            }
            else
            {
                printlexerror(1);
                exit(0);
            }
            break;

        case '!':
            if (endIndex < inputLength - 1 && input[endIndex + 1] == '=')
            {
                list[lex_index].name[0] = '!';
                list[lex_index].name[1] = '=';
                list[lex_index].type = neqsym;
                lex_index++;
                endIndex++;
            }
            else
            {
                printlexerror(1);
                exit(0);
            }
            break;
        }
    }
    printtokens();
    return list;
}

void printtokens()
{
    int i;
    printf("Lexeme Table:\n");
    printf("lexeme\t\ttoken type\n");
    for (i = 0; i < lex_index; i++)
    {
        switch (list[i].type)
        {
        case oddsym:
            printf("%11s\t%d", "odd", oddsym);
            break;
        case eqlsym:
            printf("%11s\t%d", "==", eqlsym);
            break;
        case neqsym:
            printf("%11s\t%d", "!=", neqsym);
            break;
        case lsssym:
            printf("%11s\t%d", "<", lsssym);
            break;
        case leqsym:
            printf("%11s\t%d", "<=", leqsym);
            break;
        case gtrsym:
            printf("%11s\t%d", ">", gtrsym);
            break;
        case geqsym:
            printf("%11s\t%d", ">=", geqsym);
            break;
        case modsym:
            printf("%11s\t%d", "%", modsym);
            break;
        case multsym:
            printf("%11s\t%d", "*", multsym);
            break;
        case divsym:
            printf("%11s\t%d", "/", divsym);
            break;
        case addsym:
            printf("%11s\t%d", "+", addsym);
            break;
        case subsym:
            printf("%11s\t%d", "-", subsym);
            break;
        case lparensym:
            printf("%11s\t%d", "(", lparensym);
            break;
        case rparensym:
            printf("%11s\t%d", ")", rparensym);
            break;
        case commasym:
            printf("%11s\t%d", ",", commasym);
            break;
        case periodsym:
            printf("%11s\t%d", ".", periodsym);
            break;
        case semicolonsym:
            printf("%11s\t%d", ";", semicolonsym);
            break;
        case assignsym:
            printf("%11s\t%d", ":=", assignsym);
            break;
        case beginsym:
            printf("%11s\t%d", "begin", beginsym);
            break;
        case endsym:
            printf("%11s\t%d", "end", endsym);
            break;
        case ifsym:
            printf("%11s\t%d", "if", ifsym);
            break;
        case thensym:
            printf("%11s\t%d", "then", thensym);
            break;
        case elsesym:
            printf("%11s\t%d", "else", elsesym);
            break;
        case whilesym:
            printf("%11s\t%d", "while", whilesym);
            break;
        case dosym:
            printf("%11s\t%d", "do", dosym);
            break;
        case callsym:
            printf("%11s\t%d", "call", callsym);
            break;
        case writesym:
            printf("%11s\t%d", "write", writesym);
            break;
        case readsym:
            printf("%11s\t%d", "read", readsym);
            break;
        case constsym:
            printf("%11s\t%d", "const", constsym);
            break;
        case varsym:
            printf("%11s\t%d", "var", varsym);
            break;
        case procsym:
            printf("%11s\t%d", "procedure", procsym);
            break;
        case identsym:
            printf("%11s\t%d", list[i].name, identsym);
            break;
        case numbersym:
            printf("%11d\t%d", list[i].value, numbersym);
            break;
        }
        printf("\n");
    }
    printf("\n");
    printf("Token List:\n");
    for (i = 0; i < lex_index; i++)
    {
        if (list[i].type == numbersym)
            printf("%d %d ", numbersym, list[i].value);
        else if (list[i].type == identsym)
            printf("%d %s ", identsym, list[i].name);
        else
            printf("%d ", list[i].type);
    }
    printf("\n");
    list[lex_index++].type = -1;
}

void printlexerror(int type)
{
    if (type == 1)
        printf("Lexical Analyzer Error: Invalid Symbol\n");
    else if (type == 2)
        printf("Lexical Analyzer Error: Invalid Identifier\n");
    else if (type == 3)
        printf("Lexical Analyzer Error: Excessive Number Length\n");
    else if (type == 4)
        printf("Lexical Analyzer Error: Excessive Identifier Length\n");
    else
        printf("Implementation Error: Unrecognized Error Type\n");

    free(list);
    return;
}