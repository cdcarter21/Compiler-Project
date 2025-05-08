//-------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the main program for checkpoint #8.  
//-------------------------------------------------------------------------

#include "y.tab.h"
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "expression.h"

struct symbolTable table; 
void codegen(struct listnode *);
char exprgen(struct expression *);
void outputProgram(); 
char output[9999][50];
int outputCounter = 0; 

int main()
{
   initialize(&table); 
   if (yyparse())
      printf("Syntax error\n");
   else {
      snprintf(output[outputCounter++], sizeof(output[0]), "ISP %d\n", table.nextAddress);
      codegen(list); 
      snprintf(output[outputCounter++], sizeof(output[0]), "HLT\n");
      outputProgram(); 
   }
   return 0;
}

//-------------------------------------------------------------------------
// outputProgram()
//
// this function prints all the instructions that represnt an entire GSTAL   
// program using the output array. 
//-------------------------------------------------------------------------
void outputProgram() {
   for (int i=0; i < outputCounter; i++) {
      printf("%s", output[i]);
   }
   return; 
}

//-------------------------------------------------------------------------
// codegen()
//
// this function handles generating the correct GSTAL translation for 
// each statement within the algorithm section. 
//-------------------------------------------------------------------------
void codegen(struct listnode *list)
{      
   struct listnode *p = list;
   while (p != NULL) {
      struct symbolTableEntry* entry;
      struct printNode *print; 
      struct printItem *item; 
      //if p is an assignment statement 
      if (p->kind == ST_ASSIGN) {
         entry = lookup(&table, p->varName);
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         
         //array index functionality 
         if (p->arrayIndex != NULL) {
            char index = exprgen(p->arrayIndex);
            //if expression resulted in a float, change to int
            if (index == TYPE_REAL)
               snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");
            //add the index to the address
            snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
         }

         char result = exprgen(p->exp);

         /* if the resulting expression was a different data type than the variable being
            assigned to, we want to output a data type conversion. */
         if (result == TYPE_REAL && entry->dataType == DT_INTEGER) 
            snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");
         else if (result == TYPE_INT && entry->dataType == DT_REAL) 
            snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");

         snprintf(output[outputCounter++], sizeof(output[0]), "STO\n");
      }
      //if p is a print statement 
      else if (p->kind == ST_PRINT) {
         print = p->print; 
         while (print != NULL) {
            item = print->item; 
            //if print item is a string
            if (item->kind == TYPE_STRING) {
               for (int i=1; i<strlen(item->charString)-1; i++) {                     
                  snprintf(output[outputCounter++], sizeof(output[0]), "LLI %d\n", item->charString[i]);
                  snprintf(output[outputCounter++], sizeof(output[0]), "PTC\n");
                  //this takes care of double quotes inside string
                  if (item->charString[i] == '"')
                     i++; 
               }
            }
            //if print item is a carriage return 
            else if (item->kind == EK_OP) {
               snprintf(output[outputCounter++], sizeof(output[0]), "PTL\n");
            }
            //if print item is an expression 
            else if (item->kind == EK_EXP) {
               exprgen(item->exp);
               char op = item->exp->operator; 
               //if expression is a variable
               if (item->exp->kind == EK_VAR || item->exp->kind == EK_ARR) {
                  struct symbolTableEntry* entry = lookup(&table, item->exp->name);
                  if (entry->dataType == DT_INTEGER)
                     snprintf(output[outputCounter++], sizeof(output[0]), "PTI\n");
                  else 
                     snprintf(output[outputCounter++], sizeof(output[0]), "PTF\n");
               } 
               //expression result is an integer
               else if (item->exp->type == TYPE_INT) {
                  snprintf(output[outputCounter++], sizeof(output[0]), "PTI\n");
               }
               //expression result is a real
               else if (item->exp->type == TYPE_REAL) {
                  snprintf(output[outputCounter++], sizeof(output[0]), "PTF\n"); 
               }
            }
            print = print->link; 
         }
      }
      //if p is a read statement 
      else if (p->kind == ST_READ) {
         entry = lookup(&table, p->varName);
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         
         //array index functionality 
         if (p->arrayIndex != NULL) {
            char index = exprgen(p->arrayIndex);
            //if expression resulted in a float, change to int
            if (index == TYPE_REAL)
               snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");
            //add the index to the address
            snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
         }

         if (entry->dataType == DT_INTEGER)
            snprintf(output[outputCounter++], sizeof(output[0]), "INI\n");
         else 
            snprintf(output[outputCounter++], sizeof(output[0]), "INF\n");

         snprintf(output[outputCounter++], sizeof(output[0]), "STO\n");
      }
      //if p is an if statement 
      else if (p->kind == ST_IF) {
         //evaluate expression
         exprgen(p->exp);

         //go ahead and add jump instruction, but will be fixed later
         int ogCounter = outputCounter;
         snprintf(output[outputCounter++], sizeof(output[0]), "JPF\n");
         
         //evaluate the body of the if statement through recursion 
         codegen(p->ifBody); 

         //backpatch the jump instruction 
         snprintf(output[ogCounter], sizeof(output[0]), "JPF %d\n", outputCounter);
      }
      //if p is an if-else statement 
      else if (p->kind == ST_IF_ELSE) {
         //evaluate expression
         exprgen(p->exp);

         //go ahead and add jump instruction to jump over if body, but will be fixed later
         int ogCounter = outputCounter;
         snprintf(output[outputCounter++], sizeof(output[0]), "JPF\n");
         
         //evaluate the body of the if statement through recursion 
         codegen(p->ifBody); 

         //go ahead and add jump instruction to jump over else body, will be fixed later
         int ogCounter2 = outputCounter;
         snprintf(output[outputCounter++], sizeof(output[0]), "JMP\n");

         //backpatch the first jump instruction 
         snprintf(output[ogCounter], sizeof(output[0]), "JPF %d\n", outputCounter);

         //evaluate the body of the else statement through recursion 
         codegen(p->elseBody);

         //backpatch the second jump instruction 
         snprintf(output[ogCounter2], sizeof(output[0]), "JMP %d\n", outputCounter);
      }
      //if p is a while statement 
      else if (p->kind == ST_WHILE) {
         //save the GSTAL address of the top of loop 
         int topOfLoop = outputCounter; 
         
         //evaluate expression
         exprgen(p->exp);

         //go ahead and add jump instruction to jump over while body, but will be fixed later
         int ogCounter = outputCounter;
         snprintf(output[outputCounter++], sizeof(output[0]), "JPF\n");

         //evaluate the body of the while loop through recursion 
         codegen(p->whileBody); 

         //the jump instruction to jump to the top of the loop
         snprintf(output[outputCounter++], sizeof(output[0]), "JMP %d\n", topOfLoop);

         //backpatch the jump instruction that jumps over the while body
         snprintf(output[ogCounter], sizeof(output[0]), "JPF %d\n", outputCounter);
      }
      //if p is a counting statement 
      else if (p->kind == ST_COUNTING) {
         //set variable equal to startexpr
         entry = lookup(&table, p->varName);
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         exprgen(p->startExp);
         snprintf(output[outputCounter++], sizeof(output[0]), "STO\n");
         
         //save the GSTAL address of the top of loop 
         int topOfLoop = outputCounter; 

         //check if we should do loop or not
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         snprintf(output[outputCounter++], sizeof(output[0]), "LOD\n");
         exprgen(p->endExp);
         //if counting upward
         if (p->count == CT_UP)
            snprintf(output[outputCounter++], sizeof(output[0]), "LEI\n");
         //if counting downward
         else 
            snprintf(output[outputCounter++], sizeof(output[0]), "GEI\n");

         //go ahead and add jump instruction to jump over while body, but will be fixed later
         int ogCounter = outputCounter;
         snprintf(output[outputCounter++], sizeof(output[0]), "JPF\n");

         //evaluate body
         codegen(p->countingBody);

         //increment/decrement variable by 1
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
         snprintf(output[outputCounter++], sizeof(output[0]), "LOD\n");
         snprintf(output[outputCounter++], sizeof(output[0]), "LLI 1\n");
         //if counting upward
         if (p->count == CT_UP)
            snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
         //if counting downward
         else 
            snprintf(output[outputCounter++], sizeof(output[0]), "SBI\n");
         snprintf(output[outputCounter++], sizeof(output[0]), "STO\n");

         //the jump instruction to jump to the top of the loop
         snprintf(output[outputCounter++], sizeof(output[0]), "JMP %d\n", topOfLoop);

         //backpatch the jump instruction that jumps over the counting body
         snprintf(output[ogCounter], sizeof(output[0]), "JPF %d\n", outputCounter);
      }
      else if (p->kind == ST_EXIT) {
         snprintf(output[outputCounter++], sizeof(output[0]), "HLT\n");
      }
      p = p->link;
   }
   return;
}

//-------------------------------------------------------------------------
// exprgen()
//
// this function handles generating the correct GSTAL translation for 
// integer and real expressions.    
//-------------------------------------------------------------------------
char exprgen(struct expression *p)
{
   char leftResult;
   char rightResult; 
   //if expression is an integer
   if (p->kind == EK_VAL && p->type == TYPE_INT) {
      snprintf(output[outputCounter++], sizeof(output[0]), "LLI %d\n", p->ivalue);
   }
   //if expression is a real
   else if (p->kind == EK_VAL && p->type == TYPE_REAL) {
      snprintf(output[outputCounter++], sizeof(output[0]), "LLF %f\n", p->rvalue);
   }
   //if expression is a variable
   else if (p->kind == EK_VAR) {
      struct symbolTableEntry* entry = lookup(&table, p->name);
      snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
      snprintf(output[outputCounter++], sizeof(output[0]), "LOD\n");
   }
   //if expression is an array reference 
   else if (p->kind == EK_ARR) {
      struct symbolTableEntry* entry = lookup(&table, p->name);
      snprintf(output[outputCounter++], sizeof(output[0]), "LAA %d\n", entry->address);
      
      //evaluate the index's expression
      char index = exprgen(p->arrayIndex);
      
      //if expression is a float, convert to int
      if (index == TYPE_REAL) {
         snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");
      }
      
      //add the index to the address and load the variable 
      snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
      snprintf(output[outputCounter++], sizeof(output[0]), "LOD\n");
   }
   //if expression is an operator
   else {
      switch (p->operator) {
         case OP_ADD:   leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADF\n");
                        
                        break;

         case OP_SUB:   leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "SBI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "SBF\n");
                        
                        break;

         case OP_MUL:   leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "MLI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "MLF\n");
                        
                        break;

         case OP_DIV:   leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "DVI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "DVF\n");
                        
                        break;

         case OP_UMIN:  exprgen(p->r_operand);
                        if (p->r_operand->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "NGI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "NGF\n");
                       break;

         case OP_AND:   exprgen(p->l_operand);
                        if (p->l_operand->type == TYPE_INT) {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                        }
                        else {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLF 0.0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                        }

                        exprgen(p->r_operand);
                        if (p->r_operand->type == TYPE_INT) {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 2\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "EQI\n");
                        }
                        else {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLF 0.0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 2\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "EQI\n");
                        }
                        
                        return TYPE_INT; 
         
         case OP_OR:    exprgen(p->l_operand);
                        if (p->l_operand->type == TYPE_INT) {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                        }
                        else {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLF 0.0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                        }

                        exprgen(p->r_operand);
                        if (p->r_operand->type == TYPE_INT) {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADI\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                        }
                        else {
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLF 0.0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "ADF\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "LLF 0.0\n");
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                        }
                        return TYPE_INT; 

         case OP_NOT:   exprgen(p->r_operand);
                        snprintf(output[outputCounter++], sizeof(output[0]), "LLI 0\n");
                        snprintf(output[outputCounter++], sizeof(output[0]), "LEI\n");
                        return TYPE_INT; 

         case OP_LESS:  leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "LTI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "LTF\n");
                        
                        return TYPE_INT; 

         case OP_LOEQ:  leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "LEI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "LEF\n");
                        
                        return TYPE_INT; 
               
         case OP_GREAT: leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "GTI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "GTF\n");
                        
                        return TYPE_INT; 

         case OP_GOEQ:  leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");

                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "GEI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "GEF\n");
                        
                        return TYPE_INT; 

         case OP_EQ:    leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "EQI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "EQF\n");
                        
                        return TYPE_INT; 

         case OP_NEQ:   leftResult = exprgen(p->l_operand);
                        if (p->type == TYPE_REAL && leftResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                        
                        rightResult = exprgen(p->r_operand);
                        if (p->type == TYPE_REAL && rightResult == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "ITF\n");
                           
                        if (p->type == TYPE_INT)
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEI\n");
                        else 
                           snprintf(output[outputCounter++], sizeof(output[0]), "NEF\n");
                        
                        return TYPE_INT; 

         case OP_MOD:   leftResult = exprgen(p->l_operand);
                        if (leftResult == TYPE_REAL)
                           snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");

                        leftResult = exprgen(p->l_operand);
                        if (leftResult == TYPE_REAL)
                           snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");

                        rightResult = exprgen(p->r_operand);
                        if (rightResult == TYPE_REAL)
                           snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");

                        snprintf(output[outputCounter++], sizeof(output[0]), "DVI\n");

                        rightResult = exprgen(p->r_operand);
                        if (rightResult == TYPE_REAL)
                           snprintf(output[outputCounter++], sizeof(output[0]), "FTI\n");

                        snprintf(output[outputCounter++], sizeof(output[0]), "MLI\n");
                        snprintf(output[outputCounter++], sizeof(output[0]), "SBI\n");
      }
   }
   return p->type;
}
