#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Array of numbers for the number of buckets */
static size_t bucketCounts[] = { 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521 };

/* Number of bucket counts */
static const size_t numBucketCounts = sizeof(bucketCounts) / sizeof(bucketCounts[0]);

/* Structure to represent the symbol table */
struct SymTable {
    /* Pointer to the bucket array */
    struct Node **buckets;
    /* Symbole table length/count of entries */
    size_t length;
    /* Index for the current number of buckets */
    size_t bucketIndex;
};

/* Structure for node */
struct Node {
    /* Key in the pair */
    const char *key;
    /* Value with the key */
    const void *value;
    /* Pointer to next node */
    struct Node *next;
};

/* Given key and bucket count, function to return hash value for the given key */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount) {
    const size_t HASH_MULTIPLIER = 65599;
    size_t u;
    size_t uHash = 0;
    assert(pcKey != NULL);

    for (u = 0; pcKey[u] != '\0'; u++)
        uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

    return uHash % uBucketCount;
}

/* Helper for free node and its key, given Node */
static void freeNode(struct Node *node) {
    free((char*)node->key);
    free(node);
}

/* Function to expand symbol table oSymTable if required given a oSymTable */
static void expand(SymTable_T oSymTable) {
    size_t newIndex = oSymTable->bucketIndex + 1;
    size_t newBucketCount;
    struct Node **newBuckets;
    size_t i;
    size_t keyHash;
    struct Node *curr;
    struct Node *next;

    if (newIndex > numBucketCounts - 1) {
        return;
    }

    /*  new buckets */
    newBucketCount = bucketCounts[newIndex];
    newBuckets = (struct Node**)calloc(newBucketCount, sizeof(struct Node*));
    if (newBuckets == NULL) {
        return;
    }

    /* Rehash inside symbol table */
    for (i = 0; i < bucketCounts[newIndex - 1]; i++) {
        if (oSymTable->buckets[i] == NULL) {
            continue;
        }

        /* Rehash all nodes in list through iteration */
        curr = oSymTable->buckets[i];
        while (curr != NULL) {
            keyHash = SymTable_hash(curr->key, newBucketCount);
            next = curr->next;
            curr->next = newBuckets[keyHash];
            newBuckets[keyHash] = curr;
            curr = next;
        }
    }

    free(oSymTable->buckets);

    oSymTable->buckets = newBuckets;
    oSymTable->bucketIndex = newIndex;
}

/* Function to create new symbol table that is empty, returns symtable */
SymTable_T SymTable_new(void) {
    SymTable_T table;

    table = (SymTable_T)malloc(sizeof(struct SymTable));
    if (table == NULL) {
        return NULL;
    }

    table->buckets = (struct Node**)calloc(bucketCounts[0], sizeof(struct Node*));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    table->length = 0;
    table->bucketIndex = 0;
    return table;
}

/* Function to free the memory allocated for  symbol table, given a SymTable_T oSymTable */
void SymTable_free(SymTable_T oSymTable) {
    size_t i;
    struct Node *curr;
    assert(oSymTable != NULL);

    for (i = 0; i < bucketCounts[oSymTable->bucketIndex]; i++) {
        if (oSymTable->buckets[i] == NULL) {
            continue;
        }

        curr = oSymTable->buckets[i];
        while (curr != NULL) {
            oSymTable->buckets[i] = curr->next;
            freeNode(curr);
            curr = oSymTable->buckets[i];
        }
    }
    free(oSymTable->buckets);
    free(oSymTable);
}

/* Function to get he number of entries in symbol table, gives size_t given SymTable_T */
size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->length;
}

/* Function to add a new key-value pair in the symbol table, returns 1 if successful/0 if not */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    size_t keyHash;
    struct Node *curr;
    assert(oSymTable != NULL && pcKey != NULL);

    if (oSymTable->length == bucketCounts[oSymTable->bucketIndex]) {
        expand(oSymTable);
    }

    /* Check if already exists */
    keyHash = SymTable_hash(pcKey, bucketCounts[oSymTable->bucketIndex]);
    curr = oSymTable->buckets[keyHash];
    while (curr != NULL) {
        if (strcmp(pcKey, curr->key) == 0) {
            return 0;
        }
        curr = curr->next;
    }

    curr = (struct Node*)malloc(sizeof(struct Node));
    if (curr == NULL) {
        return 0;
    }
    curr->key = (const char*)malloc(strlen(pcKey) + 1);
    if (curr->key == NULL) {
        free(curr);
        return 0;
    }
    strcpy((char*)curr->key, pcKey);

    curr->value = pvValue;
    curr->next = oSymTable->buckets[keyHash];
    oSymTable->buckets[keyHash] = curr;

    oSymTable->length++;

    return 1;
}

/* Function to replace value of key in symbol table, given the sym table, key, and value*/
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    size_t keyHash;
    struct Node *curr;
    void *oldValue;
    assert(oSymTable != NULL && pcKey != NULL);

    keyHash = SymTable_hash(pcKey, bucketCounts[oSymTable->bucketIndex]);
    curr = oSymTable->buckets[keyHash];
    while (curr != NULL) {
        if (strcmp(pcKey, curr->key) == 0) {
            oldValue = (void*)curr->value;
            curr->value = pvValue;
            return oldValue;
        }
        curr = curr->next;
    }
    return NULL;
}

/* Function to check if a key is present in the symbol table, given key and table; 1 if yes, 0 if no */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    size_t keyHash;
    struct Node *curr;
    assert(oSymTable != NULL && pcKey != NULL);

    keyHash = SymTable_hash(pcKey, bucketCounts[oSymTable->bucketIndex]);
    curr = oSymTable->buckets[keyHash];
    while (curr != NULL) {
        if (strcmp(pcKey, curr->key) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/* Function to retrieve value associated with a key given the key and the sym table */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    size_t keyHash;
    struct Node *curr;
    assert(oSymTable != NULL && pcKey != NULL);

    keyHash = SymTable_hash(pcKey, bucketCounts[oSymTable->bucketIndex]);
    curr = oSymTable->buckets[keyHash];
    while (curr != NULL) {
        if (strcmp(pcKey, curr->key) == 0) {
            return (void*)curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

/* Function to remove a key-value pair from the symbol table, given table and key */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    size_t keyHash;
    struct Node *curr;
    struct Node *prev;
    void *value;
    assert(oSymTable != NULL && pcKey != NULL);

    keyHash = SymTable_hash(pcKey, bucketCounts[oSymTable->bucketIndex]);
    prev = oSymTable->buckets[keyHash];
    if (prev == NULL) {
        return NULL;
    }

    if (strcmp(pcKey, prev->key) == 0) {
        value = (void*)prev->value;
        oSymTable->buckets[keyHash] = prev->next;
        oSymTable->length--;
        freeNode(prev);
        return value;
    }
    curr = prev->next;
    while (curr != NULL) {
        if (strcmp(pcKey, curr->key) == 0) {
            value = (void*)curr->value;
            prev->next = curr->next;
            oSymTable->length--;
            freeNode(curr);
            return value;
        }

        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

/* Function to apply provided function to all the key-value pairs in the given sym table */
void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    size_t i;
    struct Node *curr;
    assert(oSymTable != NULL && pfApply != NULL);

    for (i = 0; i < bucketCounts[oSymTable->bucketIndex]; i++) {
        if (oSymTable->buckets[i] == NULL) {
            continue;
        }

        curr = oSymTable->buckets[i];
        while (curr != NULL) {
            (*pfApply)(curr->key, (void*)curr->value, (void*)pvExtra);
            curr = curr->next;
        }
    }
}
