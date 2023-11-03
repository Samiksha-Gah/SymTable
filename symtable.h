/* This file outlines an interface 
for a symbol table. A symbol table is
a data structure connecting distinct 
string keys and their respective values. 
The implementation facilitates the 
creation of an empty symbol table, 
memory deallocation, 
 size determination, addition of 
 key-value pairs, value updates for
  existing keys, key existence checks,
   value retrieval by key, removal
of key-value pairs, and application
     of a function to all key-value nodes. */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* SymTable_T is a pointer to an object of SymTable  */
typedef struct SymTable *SymTable_T;

/* SymTable_new: Initializes a new symbol table, returning
 a new SymTable object that contains no nodes, 
 or NULL if insufficient memory is available.
The table is initially empty. */
SymTable_T SymTable_new(void);

/* SymTable_free: Takes SymTable_T oSymTable and
Frees all memory occupied by oSymTable 
after given a symbol table pointer input. Void return */
void SymTable_free(SymTable_T oSymTable);

/* SymTable_getLength: Returns size_t the number of nodes in oSymTable with
a symbol table pointer input oSymTable . */
size_t SymTable_getLength(SymTable_T oSymTable);

/* SymTable_put:  client is required to provide
a valid symbol table pointer and a non-NULL
 key. function adds a new entry to the symbol table, 
 associating provided key with  given value. 
 If successful, it returns 1; otherwise, if the
  key already exists/memory allocation
  issues, returns 0. Note that the key is 
  duplicated, and the copy is managed by the symbol table. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue);

/* SymTable_replace: Requires a 
valid symbol table pointer and a 
non-NULL key from the client. 
Updates the value of an existing node
 with the specified key and returns 
 the old value. If no such node is found,
  returns NULL.  */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue);

/* SymTable_contains: Requires a valid symbol 
table pointer and a non-NULL key from the client.
 Verifies the presence of a node with the 
 specified key in the symbol table. Returns 
 1 if the node exists, or 0 otherwise. */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* SymTable_get: client must provide a valid symbol
 table pointer and a non-NULL key. Retrieves the 
 value associated with the specified key if a 
 node exists  otherwise, returns NULL.  */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* SymTable_remove: Requires a valid symbol
 table pointer and a non-NULL key from the
  client. Deletes the node with the 
  specified key from the symbol table, 
  deallocates the key's memory, and returns
   the associated value if it exists; otherwise, 
   returns NULL.  */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* SymTable_map: takes in valid symbol table pointer 
and a non-NULL function pointer. Executes the 
provided function for every node in the symbol 
table. The pvExtra parameter is passed along to the 
function. */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra);

#endif 
