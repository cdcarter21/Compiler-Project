//----------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the header file containing the expression tree and printList tree.
//----------------------------------------------------------------------------
//kinds of statements
#define ST_ASSIGN       0
#define ST_PRINT        1
#define ST_READ         2
#define ST_IF           3
#define ST_IF_ELSE      4
#define ST_WHILE        5
#define ST_COUNTING     6
#define ST_EXIT         8

//kind of counting statement
#define CT_UP     0
#define CT_DOWN   1

//entry kind
#define EK_OP   0
#define EK_VAL  1
#define EK_VAR  2
#define EK_EXP  3
#define EK_ARR  4

//data type
#define TYPE_INT    0
#define TYPE_REAL   1
#define TYPE_STRING 2

//operators
#define OP_ADD   0
#define OP_SUB   1
#define OP_MUL   2
#define OP_DIV   3
#define OP_UMIN  4
#define OP_AND   5
#define OP_OR    6
#define OP_NOT   7
#define OP_LESS  8
#define OP_LOEQ  9
#define OP_GREAT 10
#define OP_GOEQ  11
#define OP_EQ    12
#define OP_NEQ   13
#define OP_MOD   14

struct listnode {
   struct listnode *link;
   struct expression *exp;
   struct expression *arrayIndex; 
   struct printNode *print; 
   struct listnode *ifBody; 
   struct listnode *elseBody; 
   struct listnode *whileBody; 
   struct listnode *countingBody; 
   struct expression *startExp;
   struct expression *endExp; 
   char kind; 
   char count; 
   char* varName; 

};

//a printNode will point to a printItem and to the next printNode in a printList
struct printNode {
   struct printItem *item; 
   struct printNode *link;  
};

//a printItem will tell us what to print 
struct printItem {
   char kind; 
   char* charString; 
   struct expression *exp; 
};

struct expression {
   char kind;
   char type; 
   char operator;
   struct expression *l_operand;
   struct expression *r_operand;
   struct expression *arrayIndex; 
   int ivalue;
   float rvalue; 
   char * name; 
};

extern struct listnode *list;
