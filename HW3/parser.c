#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

// Global variables
instruction *code;
symbol *table;
int cIndex;
int tIndex;
int token;
int level;

// Given function declarations
void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

// Utility function declarations
void program(lexeme *list, int level);
void block(lexeme *list, int level);
void constDeclaration(lexeme *list, int level);
int varDeclaration(lexeme *list, int level);
void procedureDeclaration(lexeme *list, int level);
void statement(lexeme *list, int level);
void condition(lexeme *list, int level);
void expression(lexeme *list, int level);
void term(lexeme *list, int level);
void factor(lexeme *list, int level);
int multipleDeclarationCheck(lexeme *list, int level);
int findSymbol(lexeme *list, int kind);
void mark(int level);
void getNextToken();

// Function that parses the tokens.
instruction *parse(lexeme *list, int printTable, int printCode)
{
  // Initialize gloabal varibles to 0
	level = 0;
  token = 0;
	cIndex = 0;
  tIndex = 0;

  // Allocate memory for pointers instruction code and symbol table.
	code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
	table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);

  // Call program
	program(list, level);

  // Print symbol table as long as printTable is true.
	if (printTable)
		printsymboltable();

  // Print assembly code as long as printCode is true.
	if (printCode)
		printassemblycode();

  // marks the end of the code
  code[cIndex].opcode = -1;

	return code;
}

void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}

void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}

	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark);

	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}

// Function that the parse function calls to execute program.
void program(lexeme *list, int level)
{
	int line;

  // Emit JMP
	emit(7, 0, 0);

	addToSymbolTable(3, "main", 0, 0, 0, 0);
	level = -1;

  // Call block function
	block(list, level);

  // Error handling prints error message and exits the program.
	if (list[token].type != periodsym)
	{
		printparseerror(1);
    exit(0);
	}

  // Emit Halt.
	emit(9, 0, 3);

	for (line = 0; line < cIndex; line++)
		if (code[line].opcode == 5)
			code[line].m = table[code[line].m].addr;
	code[0].m = table[0].addr;
}

void block(lexeme *list, int level)
{
  // Increment level.
	level++;
	int procedure_idx = tIndex - 1;

  // Call the constDeclaration function.
	constDeclaration(list, level);
	int x = varDeclaration(list, level);

  // Call procedure Declaration.
	procedureDeclaration(list, level);
	table[procedure_idx].addr = cIndex * 3;

  // Emit Inc with x as mvalue paramater. Otherwise add 3 to x for mvalue.
	if (level == 0)
		emit(6, 0, x);
	else
		emit(6, 0, x + 3);

  // Call statement function.
	statement(list, level);

  // Call mark function.
	mark(level);

  // Decrement level.
	level--;
}

void constDeclaration(lexeme *list, int level)
{
  // Check if the type is equal to constsym
	if (list[token].type == constsym)
	{
		do
		{
			getNextToken();

      // Error handling prints error message and exits the program.
			if (list[token].type != identsym)
			{
				printparseerror(2);
        exit(0);
			}

      // Store return value of multipleDecCheck in variable.
			int symidx = multipleDeclarationCheck(list, level);

      // Error handling prints error message and exits the program if variable is not equal to -1.
			if (symidx != -1)
			{
				printparseerror(18);
        exit(0);
			}

      // Save indent name.
      char saveName[12];
      strcpy(saveName, list[token].name);

			getNextToken();

      // Error handling prints error message and exits the program.
			if (list[token].type != assignsym)
			{
				printparseerror(5);
        exit(0);
			}

			getNextToken();

      // Error handling prints error message and exits the program.
			if (list[token].type != numbersym)
			{
				printparseerror(2);
        exit(0);
			}

			addToSymbolTable(1, saveName, list[token].value, level, 0, 0);
			getNextToken();
		}
		while(list[token].type == commasym);

		if (list[token].type != semicolonsym)
		{
      // Error handling prints error message and exits the program.
			if (list[token].type != identsym)
			{
				printparseerror(14);
        exit(0);
			}
			else
			{
				printparseerror(13);
        exit(0);
			}
		}
    getNextToken();
	}
}

int varDeclaration(lexeme *list, int level)
{
	int numVars = 0;
	if (list[token].type == varsym)
	{
		do
		{
      // Increment numVars.
			numVars++;

			getNextToken();

      // Error handling prints error message and exits the program.
			if (list[token].type != identsym)
			{
				printparseerror(3);
        exit(0);
			}

      // Store return value of multipleDecCheck in variable.
			int symidx = multipleDeclarationCheck(list, level);

      // Error handling prints error message and exits the program if variable is not equal to -1.
			if (symidx != -1)
			{
				printparseerror(18);
        exit(0);
			}

			if (level == 0)
      {
        addToSymbolTable(2, list[token].name, 0, level, numVars - 1, 0);
      }
			else
      {
        addToSymbolTable(2, list[token].name, 0, level, numVars + 2, 0);
      }

      getNextToken();
		}
		while(list[token].type == commasym);

		if (list[token].type != semicolonsym)
		{
      // Error handling prints error message and exits the program.
			if (list[token].type != identsym)
			{
				printparseerror(14);
        exit(0);
			}
			else
			{
				printparseerror(13);
        exit(0);
			}
		}
    getNextToken();
	}
	return numVars;
}

void procedureDeclaration(lexeme *list, int level)
{
	while (list[token].type == procsym)
	{
		getNextToken();

    // Error handling prints error message and exits the program.
		if (list[token].type != identsym)
		{
			printparseerror(4);
      exit(0);
		}

    // Store return value of multipleDecCheck in variable.
		int symidx = multipleDeclarationCheck(list, level);

    // Error handling prints error message and exits the program if variable is not equal to -1.
		if (symidx != -1)
		{
			printparseerror(18);
      exit(0);
		}

		addToSymbolTable(3, list[token].name, 0, level, 0, 0);

		getNextToken();

    // Error handling prints error message and exits the program
		if (list[token].type != semicolonsym)
		{
			printparseerror(4);
      exit(0);
		}

		getNextToken();

    // Call block function.
		block(list, level);

    // Error handling prints error message and exits the program
		if (list[token].type != semicolonsym)
		{
			printparseerror(17);
      exit(0);
		}

		getNextToken();

    // Emit RTN
		emit(2, 0, 0);
	}
}

void statement(lexeme *list, int level)
{
	if(list[token].type == identsym)
  {
    // Store return value of findSymbol in symIdx
		int symIdx = findSymbol(list, 2);

		if(symIdx == -1)
    {
      // Error handling prints error message and exits the program
			if(findSymbol(list, 1) != findSymbol(list, 3))
      {
				printparseerror(6);
				exit(0);
			}
      else
      {
				printparseerror(17);
				exit(0);
			}
		}

		getNextToken();

    // Error handling prints error message and exits the program
		if(list[token].type != assignsym)
    {
			printparseerror(5);
      exit(0);
		}

		getNextToken();

    // Call expression function.
		expression(list, level);

		emit(4, level-table[symIdx].level, table[symIdx].addr);
		return;
	}

	if(list[token].type == beginsym)
  {
		do
    {
			getNextToken();

      // Call statement recursively.
			statement(list, level);
		}
    while(list[token].type == semicolonsym);

    // Error handling prints error message and exits the program
		if(list[token].type != endsym)
    {
			if(list[token].type == identsym || list[token].type == beginsym || list[token].type == ifsym || list[token].type == whilesym || list[token].type == readsym || list[token].type == writesym || list[token].type == callsym)
      {
				printparseerror(15);
				exit(0);
			}
      else
      {
				printparseerror(16);
				exit(0);
			}
		}

		getNextToken();
		return;
	}

	if(list[token].type == ifsym)
  {
		getNextToken();

    // Call condition function.
		condition(list, level);
		int jpcIdx = cIndex;

    // Emit JPC with jpcIdx as mvalue paramater.
		emit(8, 0, jpcIdx);

    // Error handling prints error message and exits the program
    if(list[token].type != thensym)
    {
			printparseerror(8);
			exit(0);
		}

		getNextToken();

    // Call statement recursively.
		statement(list, level);

		if(list[token].type == elsesym)
    {
      getNextToken(); // Not included in pseudocode, but neccessary to run and complete complex tipping test, upon strenuous trial and error
			int jmpIdx = cIndex;

      // Emit JMP with jpcIdx as mvalue paramater.
			emit(7, 0, jmpIdx);

			code[jpcIdx].m = cIndex * 3;

      // Call statement recursivley
			statement(list, level);

			code[jmpIdx].m = cIndex * 3;
		}
    else
    {
			code[jpcIdx].m = cIndex * 3;
		}

		return;
	}

	if(list[token].type == whilesym)
  {
		getNextToken();
		int loopIdx = cIndex;

    // Call condition function.
		condition(list, level);

    // Error handling prints error message and exits the program
		if(list[token].type != dosym)
    {
			printparseerror(9);
			exit(0);
		}

		getNextToken();
		int jpcIdx = cIndex;

    // Emit JPC
		emit(8, 0, 0);

    // Call statement recursively.
		statement(list, level);

    // Emit JMP with m value with loopIdx multiplied by 3.
		emit(7, 0, loopIdx * 3);
		code[jpcIdx].m = cIndex * 3;
		return;
	}

	if(list[token].type == readsym)
  {
		getNextToken();

    // Error handling prints error message and exits the program
		if(list[token].type != identsym)
    {
			printparseerror(6);
			exit(0);
		}

    // Store return value of findSymbol in symIdx
		int symIdx = findSymbol(list, 2);

		if(symIdx == -1)
    {
      // Error handling prints error message and exits the program
			if(findSymbol(list, 1) != findSymbol(list, 3))
      {
				printparseerror(17);
				exit(0);
			}
      else
      {
				printparseerror(19);
				exit(0);
			}
		}
		getNextToken();

    // Emit READ
		emit(9, 0, 2);
		emit(4, level-table[symIdx].level, table[symIdx].addr);
		return;
	}

	if(list[token].type == writesym)
  {
		getNextToken();

    // Call expression function
		expression(list, level);

    // Emit WRITE
		emit(9, 0, 1);
		return;
	}

	if(list[token].type == callsym)
  {
		getNextToken();

    // Store return value of findSymbol and store it in variable.
		int symIdx = findSymbol(list, 3);

		if(symIdx == -1)
    {
      // Error handling prints error message and exits the program
			if(findSymbol(list, 1) != findSymbol(list, 2))
      {
				printparseerror(7);
				exit(0);
			}
      else
      {
				printparseerror(19);
				exit(0);
			}
		}
		getNextToken();
		emit(5, level-table[symIdx].level, symIdx);
	}
}

void condition(lexeme *list, int level)
{
	if (list[token].type == oddsym)
	{
		getNextToken();

    // Call expression function.
		expression(list, level);

    // Emit ODD
		emit(2, 0, 6);
	}
	else
	{
    // Call expression function.
		expression(list, level);
		if (list[token].type == eqlsym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit EQL
			emit(2, 0, 8);
		}
		else if (list[token].type == neqsym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit NEQ
			emit(2, 0, 9);
		}
		else if (list[token].type == lsssym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit LSS
			emit(2, 0, 10);
		}
		else if (list[token].type == leqsym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit LEQ
			emit(2, 0, 11);
		}
		else if (list[token].type == gtrsym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit GTR
			emit(2, 0, 12);
		}
		else if (list[token].type == geqsym)
		{
			getNextToken();

      // Call expression function.
			expression(list, level);

      // Emit GEQ
			emit(2, 0, 13);
		}

    // Error handling prints error message and exits the program.
		else
		{
			printparseerror(10);
      exit(0);
		}
	}
}

void expression(lexeme *list, int level)
{
	if (list[token].type == subsym)
	{
		getNextToken();

    // Call term funciton.
		term(list, level);

    // Emit NEG
		emit(2, 0, 1);

		while (list[token].type == addsym || list[token].type == subsym)
		{
			if (list[token].type == addsym)
			{
				getNextToken();

        // Call term funciton.
				term(list, level);

        // Emit ADD
				emit(2, 0, 2);
			}
			else
			{
				getNextToken();

        // Call term funciton.
				term(list, level);

        // Emit SUB
				emit(2, 0, 3);
			}
		}
	}
	else
	{
		if (list[token].type == addsym)
			getNextToken();

    // Call term funciton.
		term(list, level);

		while (list[token].type == addsym || list[token].type == subsym)
		{
			if (list[token].type == addsym)
			{
				getNextToken();

        // Call term funciton.
				term(list, level);

        // Emit ADD
				emit(2, 0, 2);
			}
			else
			{
				getNextToken();

        // Call term funciton.
				term(list, level);

        // Emit SUB
				emit(2, 0, 3);
			}
		}
	}

  // Error handling prints error message and exits the program
	if (list[token].type == oddsym)
	{
		printparseerror(17);
    exit(0);
	}
}

void term(lexeme *list, int level)
{
  // Call factor function
	factor(list, level);
	while (list[token].type == multsym || list[token].type == divsym ||
				 list[token].type == modsym)
	{
		if (list[token].type == multsym)
		{
			getNextToken();

      // Call factor function
			factor(list, level);

      // Emit MUL
			emit(2, 0, 4);
		}
		else if (list[token].type == divsym)
		{
			getNextToken();

      // Call factor function.
			factor(list, level);

      // Emit DIV
			emit(2, 0, 5);
		}
		else
		{
			getNextToken();

      // Call factor function
			factor(list, level);

      // EMIT MOD
			emit(2, 0, 7);
		}
	}
}

void factor(lexeme *list, int level)
{
	if(list[token].type == identsym)
  {
    // Store return values from findSymbol in variables
		int symIdx_var = findSymbol(list, 2);
		int symIdx_const = findSymbol(list, 1);

		if(symIdx_var == -1 && symIdx_const == -1)
    {

      // Error handling prints error message and exits the program
			if(findSymbol(list, 3) != -1)
      {
				printparseerror(11);
				exit(0);
			}
      else
      {
				printparseerror(19);
				exit(0);
			}
		}

		if(symIdx_var == -1)
    	emit(1, 0, table[symIdx_const].val);
	  else if(symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level)
      emit(3, level - table[symIdx_var].level, table[symIdx_var].addr);
    else
			emit(1, 0, table[symIdx_const].val);

		getNextToken();

	}
  else if(list[token].type == numbersym)
  {
		emit(1, 0, list[token].value);
		getNextToken();
	}

  else if(list[token].type == lparensym)
  {
		getNextToken();

    // Call expression function.
		expression(list, level);

    // Error handling prints error message and exits the program
  	if(list[token].type != rparensym)
    {
			printparseerror(12);
			exit(0);
		}

		getNextToken();
	}

  // Error handling prints error message and exits the program
  else
  {
		printparseerror(11);
		exit(0);
	}
}

// Function that searches through the table Index and. marks table at that index to 1 once it is equal to the current level.
void mark(int level)
{
	int i;
	for (i = tIndex - 1; i >= 0; i--)
	{
		if (table[i].mark)
			continue;

		if (table[i].level == level)
			table[i].mark = 1;

		if (!table[i].mark && table[i].level < level)
			break;
	}
}

// Function that makes a linear pass through the symbol table and returns that index if it finds the name, check to see if its unmarked and if the level is equal to the current level
int multipleDeclarationCheck(lexeme *list, int level)
{
  int i;
  for (i = 0; i <= tIndex; i++)
    if(!strcmp(table[i].name, list[token].name) && table[i].level == level && !table[i].mark)
      return i;

  return -1;
}

// Function that makes a linear pass through the symbol index returns the index as long as the name is correct and kind value matches and it is unmarked.
int findSymbol(lexeme *list, int kind)
{
	int i;
	for (i = tIndex; i >= 0; i--)
		if (!strcmp(table[i].name, list[token].name) && table[i].kind == kind && !table[i].mark)
			return i;

	return -1;
}

// Simple utility function that increments token to get next token.
void getNextToken()
{
	token++;
}
