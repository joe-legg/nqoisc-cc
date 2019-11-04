#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashTableEntry {
    char *key;
    void *value;
    struct HashTableEntry *next;
} HashTableEntry;

typedef struct HashTable {
    unsigned int table_size;
    HashTableEntry **entries;
} HashTable;

HashTable *new_hash_table(int initial_size);
void free_hash_table(HashTable *hash_table);

void *hash_table_lookup(HashTable *hash_table, const char *key);
void hash_table_insert(HashTable *hash_table, const char *key,
                       const void *value);

void hash_table_print(HashTable *hash_table);

#endif
