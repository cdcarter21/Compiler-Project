//-------------------------------------------------------------------------
// Developer---Cole Carter
// Course------CS4223-01
// Project-----Checkpoint #8
// Due---------April 23, 2025

// This is the implementation file for the symbol table methods.  
//-------------------------------------------------------------------------

#include "symbol.h"
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------
// initialize()
//
// this function initializes a given symbol table.  
//-------------------------------------------------------------------------
void initialize(struct symbolTable *table) {
    table->count = 0; 
    table->nextAddress = 0; 
}

//-------------------------------------------------------------------------
// insert()
//
// this function inserts a symbol entry into the symbol table.  
//-------------------------------------------------------------------------
void insert(struct symbolTable *table, char *n, char dt, char vt, int s) {
    struct symbolTableEntry symbol; 
    symbol.name = n; 
    symbol.dataType = dt; 
    symbol.variableType = vt; 
    symbol.size = s; 
    symbol.address = table->nextAddress; 
    table->nextAddress += s; 
    table->entries[table->count++] = symbol; 
    return; 
}

//-------------------------------------------------------------------------
// lookup()
//
// this function searches the symbol table to see if a symbol already
// exists within the table and if so, returns the entry.  
//-------------------------------------------------------------------------
struct symbolTableEntry* lookup(struct symbolTable *table, char *n) {
    
    for (int i = 0; i < table->count; i++) {
        if (!strcmp(table->entries[i].name, n))
            return &table->entries[i]; 
    }
    return NULL; 
}

//-------------------------------------------------------------------------
// printTable()
//
// this function prints the symbol table in a neat format.   
//-------------------------------------------------------------------------
void printTable(struct symbolTable *table) {

    printf("---------------------------------------------------------------------------------\n");
    printf("%-15s | %-15s | %-15s | %-15s | %-15s\n", "Name", "Data Type", "Variable Type", "Size", "Address");
    printf("---------------------------------------------------------------------------------\n");
    char *dt;
    char *vt; 
    for (int i = 0; i < table->count; i++) {
        if (table->entries[i].dataType == DT_INTEGER)
            dt = "Integer";
        else
            dt = "Real";

        if (table->entries[i].variableType == VT_SCALAR)
            vt = "Scalar";
        else
            vt = "Array";

        printf("%-15s | %-15s | %-15s | %-15d | %-15d\n", table->entries[i].name, dt, vt, table->entries[i].size, table->entries[i].address);
    }
}
