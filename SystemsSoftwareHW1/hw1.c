#include <stdio.h>

#define MAX_PAS_LENGTH 500

// Struct definition for Instruction Register
typedef struct IR
{
    int OP;
    int L;
    int M;
} IR;

// Global Struct Variable
IR ir;

// Global Variables
int GP = 0;
int IC = 0;
int DP = 0;
int FREE = 0;
int BP = 0;
int PC = 0;
int SP = 0;
int pas[MAX_PAS_LENGTH];
int line = 0;
int haltFlag = 1;
char *opname = "";

// This function will be helpful to find a variable in a different Activation
// Record some Llevels down:
int base(int L)
{
    int arb = BP; // arb = activation record base
    while (L > 0) //find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}

// Function for printing:
void print_execution(int line, char *opname, IR ir, int PC, int BP, int SP,
                     int DP, int *pas, int GP)
{
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, ir.L, ir.M , PC, BP, SP, DP);

    // print data section
    for (i = GP; i <= DP; i++)
        printf("%d ", pas[i]);
    printf("\n");

    // print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
        printf("%d ", pas[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    // Initalize all indexes to zero.
    for (int i = 0; i < MAX_PAS_LENGTH; i++)
        pas[i] = 0;

    // If User did not input enough arguments.
    if (argc < 2)
    {
      printf("***Error*** Proper syntax: ./a.out <file.txt>\n");
      return -1;
    }

    // User has input enough arguments so open the file.
    FILE *in = fopen(argv[1], "r");

    // Read in the input file
    while (!feof(in))
      fscanf(in, "%d", &pas[IC++]);

    // Close the file when done.
    fclose(in);

    // Setup registers based on location of instruction counter.
    GP = IC;
    DP = IC - 1;
    FREE = IC + 10;
    BP = IC;
    SP = MAX_PAS_LENGTH;

    // Print column headers to console
    printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");

    // Print inital values for each register to console.
    printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);

    while (haltFlag)
    {
      //---------------------------- FETCH CYCLE -----------------------------//

      // Place instruction at location 0 to 2 in IR
      ir.OP = pas[PC];
      ir.L = pas[PC + 1];
      ir.M = pas[PC + 2];
      line = PC / 3;

      // Increment PC by 3 so it points at next instruction.
      PC += 3;

      //-------------------------- EXECUTION CYCLE ---------------------------//
      // LIT
      if (ir.OP == 1)
      {
          opname = "LIT";
          if (BP == GP)
          {
              DP++;
              pas[DP] = ir.M;
          }
          else
          {
              SP--;
              pas[SP] = ir.M;
          }
      }

      // OPR
      else if (ir.OP == 2)
      {
          opname = "OPR";

          // RTN
          if (ir.M == 0)
          {
              opname = "RTN";
              SP = BP + 1;
              BP = pas[SP - 2];
              PC = pas[SP - 3];
          }

          // NEG
          else if (ir.M == 1)
          {
              opname = "NEG";
              if (BP == GP)
                  pas[DP] = -1 * pas[DP];
              else
                  pas[SP] = -1 * pas[SP];
          }

          // ADD
          else if (ir.M == 2)
          {
              opname = "ADD";
              if (BP == GP)
              {
                  DP--;
                  pas[DP] += pas[DP + 1];
              }
              else
              {
                  SP++;
                  pas[SP] += pas[SP - 1];
              }
          }

          // SUB
          else if (ir.M == 3)
          {
              opname = "SUB";
              if (BP == GP)
              {
                  DP--;
                  pas[DP] -= pas[DP + 1];
              }
              else
              {
                  SP++;
                  pas[SP] -= pas[SP - 1];
              }
          }

          // MUL
          else if (ir.M == 4)
          {
              opname = "MUL";
              if (BP == GP)
              {
                  DP--;
                  pas[DP] *= pas[DP + 1];
              }
              else
              {
                  SP++;
                  pas[SP] *= pas[SP - 1];
              }
          }

          // DIV
          else if (ir.M == 5)
          {
              opname = "DIV";
              if (BP == GP)
              {
                  DP--;
                  pas[DP] /= pas[DP + 1];
              }
              else
              {
                  SP++;
                  pas[SP] /= pas[SP - 1];
              }
          }

          // ODD
          else if (ir.M == 6)
          {
              opname = "ODD";
              if (BP == GP)
                  pas[DP] %= 2;
              else
                  pas[SP] %= 2;
          }

          // MOD
          else if (ir.M == 7)
          {
              opname = "MOD";
              if (BP == GP)
              {
                  DP--;
                  pas[DP] %= pas[DP + 1];
              }
              else
              {
                  SP++;
                  pas[SP] %= pas[SP - 1];
              }
          }

          // EQL
          else if (ir.M == 8)
          {
              opname = "EQL";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] == pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] == pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }

          // NEQ
          else if (ir.M == 9)
          {
              opname = "NEQ";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] != pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] != pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }

          // LSS
          else if (ir.M == 10)
          {
              opname = "LSS";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] < pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] < pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }

          // LEQ
          else if (ir.M == 11)
          {
              opname = "LEQ";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] <= pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] <= pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }

          // GTR
          else if (ir.M == 12)
          {
              opname = "GTR";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] > pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] > pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }

          // GEQ
          else if (ir.M == 13)
          {
              opname = "GEQ";
              if (BP == GP)
              {
                  DP--;
                  if (pas[DP] >= pas[DP + 1])
                      pas[DP] = 1;
                  else
                      pas[DP] = 0;
              }
              else
              {
                  SP++;
                  if (pas[SP] >= pas[SP - 1])
                      pas[SP] = 1;
                  else
                      pas[SP] = 0;
              }
          }
      }

      // LOD
      else if (ir.OP == 3)
      {
          opname = "LOD";
          if (BP == GP)
          {
              DP++;
              pas[DP] = pas[GP + ir.M];
          }
          else
          {
              if (base(ir.L) == GP)
              {
                  SP--;
                  pas[SP] = pas[GP + ir.M];
              }
              else
              {
                  SP--;
                  pas[SP] = pas[base(ir.L) - ir.M];
              }
          }
      }

      // STO
      else if (ir.OP == 4)
      {
          opname = "STO";
          if (BP == GP)
          {
              pas[GP + ir.M] = pas[DP];
              DP--;
          }
          else
          {
              if (base(ir.L) == GP)
              {
                  pas[GP + ir.M] = pas[SP];
                  SP++;
              }
              else
              {
                  pas[base(ir.L) - ir.M] = pas[SP];
                  SP--;
              }
          }
      }

      // CAL
      else if (ir.OP == 5)
      {
          opname = "CAL";
          pas[SP - 1] = base(ir.L);
          pas[SP - 2] = BP;
          pas[SP - 3] = PC;
          BP = SP - 1;
          PC = ir.M;
      }

      // INC
      else if (ir.OP == 6)
      {
          opname = "INC";
          if (BP == GP)
              DP = DP + ir.M;
          else
              SP = SP - ir.M;
      }

      // JMP
      else if (ir.OP == 7)
      {
        opname = "JMP";
        PC = ir.M;
      }

      // JPC
      else if (ir.OP == 8)
      {
          opname = "JPC";
          if (BP == GP)
          {
              if (pas[DP] == 0)
                  PC = ir.M;
              DP--;
          }
          else
          {
              if (pas[SP] == 0)
                  PC = ir.M;
              SP++;
          }
      }

      // SYS
      else if (ir.OP == 9)
      {
          opname = "SYS";
          if (ir.M == 1)
          {
              printf("\nTop of Stack Value: ");
              if (BP == GP)
              {
                  printf("%d\n", pas[DP]);
                  DP--;
              }
              else
              {
                  printf("%d\n", pas[SP]);
                  SP++;
              }
          }
          else if (ir.M == 2)
          {
              printf("\nPlease Enter an Integer: ");
              if (BP == GP)
              {
                  DP++;
                  scanf("%d", &pas[DP]);
              }
              else
              {
                  SP--;
                  scanf("%d", &pas[SP]);
              }
          }
          else if (ir.M == 3)
              haltFlag = 0;
      }

      print_execution(line, opname, ir, PC, BP, SP, DP, pas, GP);
    }

    return 0;
}
