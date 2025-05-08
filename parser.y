%{

//-------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the bison file for our SLIC parser. 
//-------------------------------------------------------------------------


#include <stdio.h>
#include "symbol.h"
#include "expression.h"

int yyerror();
int yylex();

char data_type; 
extern struct symbolTable table; 
struct listnode *list;

%}

%union {
   char *name;
   int ival;
   float rval;  
   char *sval; 
   struct listnode *listpoint;
   struct expression *exppoint;
   struct printNode *printpoint; 
   struct printItem *printItemPoint; 
}

%{ 
   //Tokens used in grammar 
%}
%token MAIN
%token END
%token DATA 
%token ALGORITHM
%token COLON 
%token SEMICOLON
%token REAL 
%token INTEGER 
%token <ival> INTEGER_CONSTANT
%token COMMA 
%token OPEN_BRACKET 
%token CLOSED_BRACKET 
%token <name> VARIABLE_NAME 
%token PRINT 
%token MINUS
%token ADD 
%token MULTIPLY 
%token DIVIDE
%token ASSIGNMENT 
%token OPEN_PARENTHESIS
%token CLOSED_PARENTHESIS
%token CARRIAGE_RETURN
%token <rval> REAL_CONSTANT 
%token <sval> CHARACTER_STRING
%token LESS_THAN 
%token LESS_THAN_OR_EQUAL
%token GREATER_THAN
%token GREATER_THAN_OR_EQUAL
%token EQUAL_TO 
%token NOT_EQUAL_TO 
%token AND 
%token OR 
%token NOT 
%token IF
%token ELSE 
%token WHILE 
%token EXIT 
%token COUNTING 
%token UPWARD
%token DOWNWARD
%token TO
%token READ
%token MODULUS 
%token CATCH_ALL

%type <listpoint> statements
%type <printpoint> printList
%type <printItemPoint> printItem
%type <exppoint> expr
%type <exppoint> comp 
%type <exppoint> exp
%type <exppoint> term
%type <exppoint> factor
%type <exppoint> unit

%%

program: MAIN SEMICOLON data_section algorithm_section END MAIN SEMICOLON;

data_section: DATA COLON
			   | DATA COLON declarations;

declarations: declaration SEMICOLON
            | declarations declaration SEMICOLON;

declaration: type COLON variables;

type: REAL { data_type = DT_REAL; }
    | INTEGER { data_type = DT_INTEGER; };

variables: VARIABLE_NAME       {  if (lookup(&table, $1) == NULL) 
                                    insert(&table, $1, data_type, VT_SCALAR, 1); 
                                 else {
                                    yyerror("ERROR: Redeclaration of an existing variable.");
                                    YYERROR; 
                                 }
                              }
        | VARIABLE_NAME       {  if (lookup(&table, $1) == NULL)
                                    insert(&table, $1, data_type, VT_SCALAR, 1); 
                                 else {
                                    yyerror("ERROR: Redeclaration of an existing variable.");
                                    YYERROR;
                                 }
                              } 
          COMMA variables
        | VARIABLE_NAME OPEN_BRACKET INTEGER_CONSTANT CLOSED_BRACKET {  if (lookup(&table, $1) == NULL) 
                                                                           insert(&table, $1, data_type, VT_ARRAY, $3); 
                                                                        else {
                                                                           yyerror("ERROR: Redeclaration of an existing variable.");
                                                                           YYERROR;
                                                                        }
                                                                     }
		  | VARIABLE_NAME OPEN_BRACKET INTEGER_CONSTANT CLOSED_BRACKET {  if (lookup(&table, $1) == NULL) 
                                                                           insert(&table, $1, data_type, VT_ARRAY, $3); 
                                                                        else {
                                                                           yyerror("ERROR: Redeclaration of an existing variable.");
                                                                           YYERROR; 
                                                                        }
                                                                     } 
          COMMA variables; 

algorithm_section: ALGORITHM COLON
                 | ALGORITHM COLON statements { list = $3; };

statements: VARIABLE_NAME ASSIGNMENT expr SEMICOLON 
                  {  
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL;
                     $$->exp = $3;
                     $$->kind = ST_ASSIGN; 
                     $$->varName = $1; 
                  }
          | VARIABLE_NAME ASSIGNMENT expr SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $5;
                     $$->exp = $3;
                     $$->kind = ST_ASSIGN;
                     $$->varName = $1; 
                  }
          | VARIABLE_NAME OPEN_BRACKET expr CLOSED_BRACKET ASSIGNMENT expr SEMICOLON 
                  {  
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL;
                     $$->exp = $6;
                     $$->kind = ST_ASSIGN; 
                     $$->varName = $1; 
                     $$->arrayIndex = $3;
                  }
          | VARIABLE_NAME OPEN_BRACKET expr CLOSED_BRACKET ASSIGNMENT expr SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $8;
                     $$->exp = $6;
                     $$->kind = ST_ASSIGN;
                     $$->varName = $1; 
                     $$->arrayIndex = $3;
                  }
          | PRINT printList SEMICOLON 
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->print = $2; 
                     $$->kind = ST_PRINT; 
                  }
          | PRINT printList SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $4; 
                     $$->print = $2; 
                     $$->kind = ST_PRINT; 
                  }
          | READ VARIABLE_NAME SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->kind = ST_READ; 
                     $$->varName = $2; 
                  }
          | READ VARIABLE_NAME SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $4; 
                     $$->kind = ST_READ; 
                     $$->varName = $2; 
                  }
          | READ VARIABLE_NAME OPEN_BRACKET expr CLOSED_BRACKET SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->kind = ST_READ; 
                     $$->varName = $2;
                     $$->arrayIndex = $4; 
                  }
          | READ VARIABLE_NAME OPEN_BRACKET expr CLOSED_BRACKET SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $7; 
                     $$->kind = ST_READ; 
                     $$->varName = $2;
                     $$->arrayIndex = $4; 
                  }
          | IF expr SEMICOLON statements END IF SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->ifBody = $4; 
                     $$->kind = ST_IF; 
                     $$->exp = $2; 
                  }
          | IF expr SEMICOLON statements END IF SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $8;
                     $$->ifBody = $4;  
                     $$->kind = ST_IF; 
                     $$->exp = $2; 
                  }
          | IF expr SEMICOLON statements ELSE SEMICOLON statements END IF SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->ifBody = $4; 
                     $$->elseBody = $7; 
                     $$->kind = ST_IF_ELSE; 
                     $$->exp = $2; 
                  }
          | IF expr SEMICOLON statements ELSE SEMICOLON statements END IF SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $11; 
                     $$->ifBody = $4; 
                     $$->elseBody = $7; 
                     $$->kind = ST_IF_ELSE; 
                     $$->exp = $2; 
                  }
          | WHILE expr SEMICOLON statements END WHILE SEMICOLON 
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL;
                     $$->exp = $2;
                     $$->whileBody = $4;
                     $$->kind = ST_WHILE; 
                  }
          | WHILE expr SEMICOLON statements END WHILE SEMICOLON statements 
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $8;
                     $$->exp = $2;
                     $$->whileBody = $4;
                     $$->kind = ST_WHILE; 
                  }
          | COUNTING VARIABLE_NAME UPWARD expr TO expr SEMICOLON statements END COUNTING SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL;
                     $$->startExp = $4;
                     $$->endExp = $6;
                     $$->kind = ST_COUNTING;
                     $$->count = CT_UP;  
                     $$->varName = $2; 
                     $$->countingBody = $8;
                  }
          | COUNTING VARIABLE_NAME UPWARD expr TO expr SEMICOLON statements END COUNTING SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $12;
                     $$->startExp = $4;
                     $$->endExp = $6;
                     $$->kind = ST_COUNTING; 
                     $$->count = CT_UP; 
                     $$->varName = $2; 
                     $$->countingBody = $8;
                  }
          | COUNTING VARIABLE_NAME DOWNWARD expr TO expr SEMICOLON statements END COUNTING SEMICOLON
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL;
                     $$->startExp = $4;
                     $$->endExp = $6;
                     $$->kind = ST_COUNTING; 
                     $$->count = CT_DOWN; 
                     $$->varName = $2; 
                     $$->countingBody = $8;
                  }
          | COUNTING VARIABLE_NAME DOWNWARD expr TO expr SEMICOLON statements END COUNTING SEMICOLON statements
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $12;
                     $$->startExp = $4;
                     $$->endExp = $6;
                     $$->kind = ST_COUNTING; 
                     $$->count = CT_DOWN; 
                     $$->varName = $2; 
                     $$->countingBody = $8;
                  }
          | EXIT SEMICOLON 
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = NULL; 
                     $$->kind = ST_EXIT; 
                  }
          | EXIT SEMICOLON statements 
                  {
                     $$ = malloc(sizeof(struct listnode));
                     $$->link = $3; 
                     $$->kind = ST_EXIT; 
                  }
          ;

printList: printItem COMMA printList
               {
                  $$ = malloc(sizeof(struct printNode));
                  $$->item = $1; 
                  $$->link = $3; 
               }
         | printItem
               {
                  $$ = malloc(sizeof(struct printNode));
                  $$->item = $1; 
                  $$->link = NULL; 
               }
         ;

printItem: CHARACTER_STRING
               {
                  $$ = malloc(sizeof(struct printItem));
                  $$->charString = $1; 
                  $$->kind = TYPE_STRING; 
               }
         | CARRIAGE_RETURN
               {
                  $$ = malloc(sizeof(struct printItem));
                  $$->kind = EK_OP; 
               }
         | expr
               {
                  $$ = malloc(sizeof(struct printItem));
                  $$->kind = EK_EXP; 
                  $$->exp = $1; 
               }
         ;

expr: expr AND comp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_AND;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | expr OR comp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_OR;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | NOT comp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_NOT;
            $$->r_operand = $2;
            if($$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | comp 
         {
            $$ = $1; 
         }
    ;

comp: comp LESS_THAN exp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_LESS;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | comp LESS_THAN_OR_EQUAL exp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_LOEQ;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT; 
         }
    | comp GREATER_THAN exp
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_GREAT;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | comp GREATER_THAN_OR_EQUAL exp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_GOEQ;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | comp EQUAL_TO exp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_EQ;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | comp NOT_EQUAL_TO exp 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_NEQ;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT; 
         }
    | exp 
         {
            $$ = $1; 
         }
    ;

exp: exp ADD term 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_ADD;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
   | exp MINUS term 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_SUB;
            $$->l_operand = $1;
            $$->r_operand = $3;  
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
   | term
         {
            $$ = $1;
         }
   ; 

term: term MULTIPLY factor 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_MUL;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | term DIVIDE factor
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_DIV;
            $$->l_operand = $1;
            $$->r_operand = $3;
            if($$->l_operand->type == TYPE_REAL || $$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
    | term MODULUS factor 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_MOD;
            $$->l_operand = $1;
            $$->r_operand = $3;
            $$->type = TYPE_INT;
         }
    | factor
         {
            $$ = $1;
         }
   ; 

factor: MINUS unit 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_OP;
            $$->operator = OP_UMIN;
            $$->r_operand = $2;
            if($$->r_operand->type == TYPE_REAL)
               $$->type = TYPE_REAL;
            else 
               $$->type = TYPE_INT;
         }
      | unit
         {
            $$ = $1; 
         }
      ; 

unit: OPEN_PARENTHESIS expr CLOSED_PARENTHESIS
         {
            $$ = $2; 
         }
    | INTEGER_CONSTANT
         {
            $$ = malloc(sizeof(struct expression));
            $$->type = TYPE_INT;
            $$->kind = EK_VAL;
            $$->ivalue = $1;
         }
    | REAL_CONSTANT
         {
            $$ = malloc(sizeof(struct expression));
            $$->type = TYPE_REAL;
            $$->kind = EK_VAL; 
            $$->rvalue = $1;
         }
    | VARIABLE_NAME
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_VAR;
            $$->name = $1;
            struct symbolTableEntry* entry = lookup(&table, $1);
            if (entry->dataType == DT_INTEGER)
               $$->type = TYPE_INT;
            else if (entry->dataType == DT_REAL)
               $$->type = TYPE_REAL; 
         }
    | VARIABLE_NAME OPEN_BRACKET expr CLOSED_BRACKET 
         {
            $$ = malloc(sizeof(struct expression));
            $$->kind = EK_ARR;
            $$->name = $1;
            $$->arrayIndex = $3; 
            struct symbolTableEntry* entry = lookup(&table, $1);
            if (entry->dataType == DT_INTEGER)
               $$->type = TYPE_INT;
            else if (entry->dataType == DT_REAL)
               $$->type = TYPE_REAL; 
         }
    ; 

%%

int yyerror(char *message) {
   printf("%s\n", message);
   return  0;
}
