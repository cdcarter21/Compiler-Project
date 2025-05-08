%{ 

//-------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the flex file for our SLIC scanner. 
//-------------------------------------------------------------------------

#define  DEBUG 0
#include "y.tab.h"
#include <stdio.h>
#include <string.h>

%}

%%

%{ 
    //Reserved Words
%}
[mM][aA][iI][nN]                                        { 
                                                            if (DEBUG) printf("MAIN: %s\n", yytext);
                                                            return(MAIN);
                                                        } 
[eE][nN][dD]                                            { 
                                                            if (DEBUG) printf("END: %s\n", yytext);           
                                                            return(END);
                                                        } 
[iI][fF]                                                { 
                                                            if (DEBUG) printf("IF: %s\n", yytext); 
                                                            return(IF); 
                                                        }                               
[eE][lL][sS][eE]                                        { 
                                                            if (DEBUG) printf("ELSE: %s\n", yytext);
                                                            return(ELSE);
                                                        }
[wW][hH][iI][lL][eE]                                    { 
                                                            if (DEBUG) printf("WHILE: %s\n", yytext);
                                                            return(WHILE); 
                                                        }
[dD][aA][tT][aA]                                        { 
                                                            if (DEBUG) printf("DATA: %s\n", yytext);
                                                            return(DATA);
                                                        }
[aA][lL][gG][oO][rR][iI][tT][hH][mM]                    { 
                                                            if (DEBUG) printf("ALGORITHM: %s\n", yytext);
                                                            return(ALGORITHM);
                                                        }
[eE][xX][iI][tT]                                        { 
                                                            if (DEBUG) printf("EXIT: %s\n", yytext); 
                                                            return(EXIT);
                                                        }
[rR][eE][aA][lL]                                        {   if (DEBUG) printf("REAL: %s\n", yytext);
                                                            return(REAL);
                                                        }
[iI][nN][tT][eE][gG][eE][rR]                            { 
                                                            if (DEBUG) printf("INTEGER: %s\n", yytext);
                                                            return(INTEGER);
                                                        }
[cC][oO][uU][nN][tT][iI][nN][gG]                        { 
                                                            if (DEBUG) printf("COUNTING: %s\n", yytext);
                                                            return(COUNTING); 
                                                        }
[uU][pP][wW][aA][rR][dD]                                { 
                                                            if (DEBUG) printf("UPWARD: %s\n", yytext); 
                                                            return(UPWARD);
                                                        }
[dD][oO][wW][nN][wW][aA][rR][dD]                        { 
                                                            if (DEBUG) printf("DOWNWARD: %s\n", yytext); 
                                                            return(DOWNWARD);
                                                        }
[tT][oO]                                                { 
                                                            if (DEBUG) printf("TO: %s\n", yytext); 
                                                            return(TO);
                                                        }
[rR][eE][aA][dD]                                        { 
                                                            if (DEBUG) printf("READ: %s\n", yytext);
                                                            return(READ);
                                                        }
[pP][rR][iI][nN][tT]                                    { 
                                                            if (DEBUG) printf("PRINT: %s\n", yytext); 
                                                            return(PRINT);
                                                        }


%{ 
    //Operators and Symbols 
%}
";"                                                     { 
                                                            if (DEBUG) printf("SEMICOLON: %s\n", yytext);
                                                            return(SEMICOLON);
                                                        }
":"                                                     { 
                                                            if (DEBUG) printf("COLON: %s\n", yytext);
                                                            return(COLON);
                                                        }
","                                                     { 
                                                            if (DEBUG) printf("COMMA: %s\n", yytext); 
                                                            return(COMMA);
                                                        }
"["                                                     { 
                                                            if (DEBUG) printf("OPEN_BRACKET: %s\n", yytext);
                                                            return(OPEN_BRACKET);
                                                        }
"]"                                                     { 
                                                            if (DEBUG) printf("CLOSED_BRACKET: %s\n", yytext);
                                                            return(CLOSED_BRACKET);
                                                        }
"-"                                                     { 
                                                            if (DEBUG) printf("MINUS: %s\n", yytext); 
                                                            return(MINUS);
                                                        }
"+"                                                     { 
                                                            if (DEBUG) printf("ADD: %s\n", yytext); 
                                                            return(ADD);
                                                        }
"*"                                                     { 
                                                            if (DEBUG) printf("MULTIPLY: %s\n", yytext); 
                                                            return(MULTIPLY);
                                                        }
"/"                                                     { 
                                                            if (DEBUG) printf("DIVIDE: %s\n", yytext); 
                                                            return(DIVIDE);
                                                        }
"%"                                                     { 
                                                            if (DEBUG) printf("MODULUS: %s\n", yytext); 
                                                            return(MODULUS);
                                                        }
"<"                                                     { 
                                                            if (DEBUG) printf("LESS_THAN: %s\n", yytext);
                                                            return(LESS_THAN);
                                                        }
"<="                                                    { 
                                                            if (DEBUG) printf("LESS_THAN_OR_EQUAL: %s\n", yytext); 
                                                            return(LESS_THAN_OR_EQUAL);
                                                        }
">"                                                     { 
                                                            if (DEBUG) printf("GREATER_THAN: %s\n", yytext); 
                                                            return(GREATER_THAN);
                                                        }
">="                                                    { 
                                                            if (DEBUG) printf("GREATER_THAN_OR_EQUAL: %s\n", yytext); 
                                                            return(GREATER_THAN_OR_EQUAL);
                                                        }
"="                                                     { 
                                                            if (DEBUG) printf("EQUAL_TO: %s\n", yytext); 
                                                            return(EQUAL_TO);
                                                        }
"<>"                                                    { 
                                                            if (DEBUG) printf("NOT_EQUAL_TO: %s\n", yytext); 
                                                            return(NOT_EQUAL_TO);
                                                        }
"&"                                                     { 
                                                            if (DEBUG) printf("AND: %s\n", yytext); 
                                                            return(AND);
                                                        }
"|"                                                     { 
                                                            if (DEBUG) printf("OR: %s\n", yytext); 
                                                            return(OR);
                                                        }
"~"                                                     { 
                                                            if (DEBUG) printf("NOT: %s\n", yytext); 
                                                            return(NOT);
                                                        }
":="                                                    { 
                                                            if (DEBUG) printf("ASSIGNMENT: %s\n", yytext); 
                                                            return(ASSIGNMENT);
                                                        }
"("                                                     { 
                                                            if (DEBUG) printf("OPEN_PARENTHESIS: %s\n", yytext); 
                                                            return(OPEN_PARENTHESIS);
                                                        }
")"                                                     { 
                                                            if (DEBUG) printf("CLOSED_PARENTHESIS: %s\n", yytext); 
                                                            return(CLOSED_PARENTHESIS);
                                                        }
"!"                                                     { 
                                                            if (DEBUG) printf("CARRIAGE_RETURN: %s\n", yytext); 
                                                            return(CARRIAGE_RETURN); 
                                                        }


%{ 
    //Variable Names and Data Values 
%}
[a-zA-Z][a-zA-Z0-9]*                                    { 
                                                            if (DEBUG) printf("VARIABLE_NAME: %s\n", yytext);
                                                            yylval.name = strdup(yytext);
                                                            return(VARIABLE_NAME);
                                                        }
([0-9]+\.[0-9]+)|([0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+)     { 
                                                            if (DEBUG) printf("REAL_CONSTANT: %s\n", yytext);
                                                            yylval.rval = atof(yytext);
                                                            return(REAL_CONSTANT);
                                                        }
[0-9]+                                                  { 
                                                            if (DEBUG) printf("INTEGER_CONSTANT: %s\n", yytext);  
                                                            yylval.ival = atoi(yytext);
                                                            return(INTEGER_CONSTANT);
                                                        }
\"([^"\n]|\"\")*\"                                      { 
                                                            if (DEBUG) printf("CHARACTER_STRING: %s\n", yytext); 
                                                            yylval.sval = strdup(yytext); 
                                                            return(CHARACTER_STRING);
                                                        }


%{ 
    //Tokens that are not passed to parser 
%}
#[^\n]*                                                 { 
                                                            if (DEBUG) printf("COMMENT: %s\n", yytext);      
                                                        }
[\t ]*                                                  { 
                                                            if (DEBUG) printf("WHITESPACE\n"); 
                                                        }
"\n"                                                    { 
                                                            if (DEBUG) printf("----NEWLINE----\n\n"); 
                                                        }
.                                                       { 
                                                            if (DEBUG) printf("CATCH_ALL: %s\n", yytext); 
                                                            return(CATCH_ALL);
                                                        } 
