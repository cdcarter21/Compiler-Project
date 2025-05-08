//-------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the header file for the symbol table.  
//-------------------------------------------------------------------------

#define DT_INTEGER  0
#define DT_REAL     1

#define VT_SCALAR   0
#define VT_ARRAY    1

//an entry in the symbol table
struct symbolTableEntry {
    char *name; 
    char dataType; 
    char variableType;
    int size; 
    int address; 
};

//the symbol table 
struct symbolTable {
    int count; 
    int nextAddress; 
    struct symbolTableEntry entries[9999]; 
};

//symbol table operations
void initialize(struct symbolTable *table);
void insert(struct symbolTable *table, char *n, char dt, char vt, int s);
struct symbolTableEntry* lookup(struct symbolTable *table, char *n);
void printTable(struct symbolTable *table);
