// Copyright 2022 Dragomir Andrei
#ifndef STRUCTS_H_
#define STRUCTS_H_

#pragma once

// Structs file
typedef struct info info;
struct info
{
    void *key;
    void *value;
};

typedef struct ll_node_t
{
    void *data;
    struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list_t
{
    ll_node_t *head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

typedef struct hashtable_t hashtable_t;
struct hashtable_t
{
    linked_list_t **buckets;
    unsigned int size;
    unsigned int hmax;
    unsigned int (*hash_function)(void *);
    long (*compare_function)(void *, void *);
};

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
          long (*compare_function)(void *, void *));
unsigned int
hash_function_string(void *a);
long compare_function_longs(void *a, void *b);
int ht_has_key(hashtable_t *ht, void *key);
void *
ht_get(hashtable_t *ht, void *key);

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
            void *value, unsigned int value_size);

void *ht_remove_entry(hashtable_t *ht, void *key);
void ht_print(hashtable_t *hashtable, FILE *out);
void ht_free(hashtable_t *ht);
#endif // STRUCTS_H_