#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int hash(const char *key)
{
    int hash = 2166136261;
    for (int i = 0; key[i] != '\0'; i++) {
        hash ^= key[i];
        hash *= 16777619;
    }
    return hash;
}

HashTable *new_hash_table(int initial_size)
{
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table_size = initial_size;
    ht->entries = malloc(sizeof(void *) * initial_size);
    for (int i = 0; i < initial_size; i++) ht->entries[i] = NULL;
    return ht;
}

static HashTableEntry *new_hash_table_entry(const char *key, const void *value)
{
    HashTableEntry *hte = malloc(sizeof(HashTableEntry));
    hte->key = malloc(sizeof(char) * strlen(key) + 1);
    hte->value = malloc(sizeof(void *));

    strcpy(hte->key, key);
    memcpy(hte->value, value, sizeof(void *));

    hte->next = NULL;

    return hte;
}

static void free_hash_table_entry(HashTableEntry *entry)
{
    if (entry == NULL) return;
    free(entry->key);
    free(entry->value);
    free_hash_table_entry(entry->next);
    free(entry);
}

void free_hash_table(HashTable *hash_table)
{
    for (int i = 0; i < hash_table->table_size; i++)
        free_hash_table_entry(hash_table->entries[i]);
    free(hash_table->entries);
    free(hash_table);
}

void *hash_table_lookup(HashTable *hash_table, const char *key)
{
    HashTableEntry *entry = hash_table->entries[hash(key) %
                                                hash_table->table_size];

    if (entry == NULL) return NULL;

    while (entry != NULL) {
        // Does the key match
        if (strcmp(entry->key, key) == 0) return entry->value;
        entry = entry->next;
    }
    return NULL;
}

void hash_table_insert(HashTable *hash_table, const char *key,
                       const void *value)
{
    unsigned int index = hash(key) % hash_table->table_size;

    // New entry
    if (hash_table->entries[index] == NULL) {
        hash_table->entries[index] = new_hash_table_entry(key, value);
        return;
    }

    HashTableEntry *entry = hash_table->entries[index];
    while (entry->next != NULL) {
        // Update value if the key already exists
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(sizeof(void *));
            memcpy(entry->value, value, sizeof(void *));
            return;
        }
        entry = entry->next;
    }

    // Handle collision
    entry->next = new_hash_table_entry(key, value);
}

void hash_table_print(HashTable *hash_table)
{
    printf("hash table size = %d\n", hash_table->table_size);

    for (int i = 0; i < hash_table->table_size; i++) {
        HashTableEntry *entry = hash_table->entries[i];

        if (entry == NULL) continue;

        printf("bucket[%d] = ", i);
        while (1) {
            printf("(key = \"%s\", value = %p) ", entry->key, entry->value);
            if (entry->next == NULL) break;
            entry = entry->next;
        }
        printf("\n");
    }
}
