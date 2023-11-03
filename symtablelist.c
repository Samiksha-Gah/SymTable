#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* symbol table struct that is  a linked list
It contains the first node and the length */
struct SymTable {
    /* points to the first node */
  struct Node *first; 
  /* store length of list */
  size_t length;
};

/* Node in the linked list  with a 
key-value pair and pointer
to the next one */
struct Node {
  /* string, store defensive key copy */
  const char *key;
  /* void pointer, pointer to value */
  const void *value;
  /* points to next node in the linked list */
  struct Node *next;
};

/* void, takes node pointer and freed node
and defensive key */
static void freeBoth(struct Node *node) {
    /* frees node */
    free(node);
    /* frees key */
    free((char*)node->key);

}

SymTable_T SymTable_new(void) {
  SymTable_T tableOne = (SymTable_T)malloc(sizeof(struct SymTable));
  /* returning null if the memory allocation fails */
  if (!tableOne) {
    return NULL;
  }

  /* Initialize the symbol table. */
  tableOne->first = NULL;
  tableOne->length = 0;
  /* Returns symbol table. */
  return tableOne;
}
/* SymTable_free: Takes SymTable_T oSymTable and
Frees all memory occupied by oSymTable 
after given a symbol table pointer input. Void return */
void SymTable_free(SymTable_T oSymTable) {
  struct Node *curr =  oSymTable->first;
  /*return if null */
  if (!oSymTable) return;
  /* free all the nodes by iteration */
  while (curr != NULL) {
    /*store next*/
    oSymTable->first = curr->next;    
    freeBoth(curr);
    curr = oSymTable->first;
  }
  free(oSymTable);
}

/* SymTable_getLength: Returns size_t the 
number of Nodes in oSymTable with
a symbol table pointer input oSymTable . */

size_t SymTable_getLength(SymTable_T oSymTable) {
  /*check for null */
  assert(oSymTable != NULL);
  return oSymTable->length;
}

/* SymTable_put:  client is required to provide
a valid symbol table pointer and a non-NULL
 key. function adds a new entry to the symbol table, 
 associating provided key with  given value. 
 If successful, it returns 1; otherwise, if the
  key already exists/memory allocation
  issues, returns 0. Note that  key is 
  duplicated, and the copy is managed by the symbol table. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {
struct Node *currentNode;
         /* If  NULL, return. */
        assert(oSymTable != NULL && pcKey != NULL);
        char *copyK;
        size_t lenK;
        /* If exists, return. */
        if (SymTable_contains(oSymTable, pcKey)) {
            return 0;
        }
        /* Memory allocation */
        lenK = strlen(pcKey) + 1;
        copyK = (char *)malloc(lenK);
        if (copyK == NULL) {
            return 0; 
        }   
        strcpy(copyK, pcKey);
        /* Memory allocation */
       currentNode = (struct Node *)malloc(sizeof(struct Node));
        /* free the node if key can't be copied */
        if (currentNode == NULL) {
            free(copyK);  
            return 0; 
        }
        currentNode->key = copyK;
        currentNode->value = (void *)pvValue;
        currentNode->next = oSymTable->first;
        oSymTable->first = currentNode;
        oSymTable->length++;
        /* Return 1 for success. */
        return 1;
}

/* SymTable_replace: Requires a 
valid symbol table pointer and a 
non-NULL key from the client. 
Updates the value of an existing Node
 with the specified key and returns 
 the old value. If no such Node is found,
  just returns NULL.  */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {
  struct Node *curr;
  void *oldValue;
  /* ensuring key or table is not NULL */
  assert(oSymTable != NULL && pcKey != NULL);

  curr = oSymTable->first;
  while (curr != NULL) {
    if (strcmp(pcKey, curr->key) == 0) {
      oldValue = (void*)curr->value;
      curr->value = pvValue;
      /*returns the old value */
      return oldValue;
    }
    curr = curr->next;
  }
  /* no such node is found */
  return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct Node *curr;
    curr = oSymTable->first;
        assert(pcKey != NULL);
        assert(oSymTable != NULL);
/* while key is not found */ 
    while (curr != NULL) {

        if (strcmp(curr->key, pcKey) == 0) {
            return 1;
        /* key found confimation */
        }
        curr = curr->next;
    }
    return 0;
    /* key not found confimation */
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
  assert (pcKey != NULL);
  struct Node *curr = oSymTable->first;
  /* until value is found */
  while (curr != NULL) {
    if (strcmp(pcKey, curr->key) == 0) {
      return (void*)curr->value;
      /* return the value of key if found */
    }
    curr = curr->next;
  }
  return NULL;
  /* returns NULL if the key is not found */
}

/* SymTable_remove: Requires a valid symbol
 table pointer and a non-NULL key from the
  client. Deletes the node with the 
  specified key from the symbol table, 
  deallocates the key's memory, and returns
   the associated value if it exists; otherwise, 
   returns NULL.  */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
  assert(oSymTable != NULL && pcKey != NULL);

  struct Node *prev = NULL;
  struct Node *curr = oSymTable->first;
  void *value = NULL;

  while (curr != NULL) {
    if (strcmp(pcKey, curr->key) == 0) {
      value = (void*)curr->value;
      if (prev == NULL) {
        /* First item is being removed */
        oSymTable->first = curr->next;
      } else {
        /* Middle or last item is being removed */
        prev->next = curr->next;
      }
      oSymTable->length--;
      freeBoth(curr);
      break; /*Exit the loop after removing the node. */
    }
    /* Move prev and curr pointers forward */
    prev = curr;
    curr = curr->next;
  }

  return value;
}


void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra) {
  struct Node *curr = oSymTable->first;
  assert(oSymTable != NULL && pfApply != NULL);
  /* Iterates through and applies the function. */
  while (curr != NULL) {
    (*pfApply)(curr->key, (void*)curr->value, (void*)pvExtra);
    curr = curr->next;
  }
}