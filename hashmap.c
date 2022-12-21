// Copyright 2022 Dragomir Andrei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include <stdint.h>
linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t *new_list = malloc(sizeof(linked_list_t));

    if (!new_list)
        return NULL;

    new_list->data_size = data_size;
    new_list->size = 0;
    new_list->head = NULL;

    return new_list;
}

void ll_add_nth_node(linked_list_t *list, unsigned int n, void *new_data)
{
    if (!list)
        return;

    ll_node_t *new_node = malloc(sizeof(ll_node_t));

    if (!new_node)
        return;

    new_node->next = NULL;
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    list->size++;

    if (!list->head || n == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
        return;
    }

    ll_node_t *curr_node = list->head;
    ll_node_t *prev_node = NULL;

    for (size_t i = 0; i < n && i < list->size; i++)
    {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    prev_node->next = new_node;
    new_node->next = curr_node;
}

ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list)
        return NULL;

    if (!list->head)
        return NULL;

    if (n > list->size - 1)
        n = list->size - 1;

    curr = list->head;
    prev = NULL;
    while (n > 0)
    {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (!prev)
        list->head = curr->next;
    else
        prev->next = curr->next;

    list->size--;

    return curr;
}

unsigned int
ll_get_size(linked_list_t *list)
{
    if (!list)
        return -1;

    return list->size;
}

void ll_free(linked_list_t **pp_list, void (*free_func)(void *))
{
    if (pp_list == NULL || *pp_list == NULL)
        return;

    while (ll_get_size(*pp_list) > 0)
    {
        ll_node_t *currNode = ll_remove_nth_node(*pp_list, 0);
        free_func(currNode);
    }

    free(*pp_list);
    *pp_list = NULL;
}
void ll_free_direct(linked_list_t **pp_list)
{
    ll_node_t *currNode;

    if (!pp_list || !*pp_list)
        return;

    while (ll_get_size(*pp_list) > 0)
    {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

unsigned int hash_function(void *a)
{
    unsigned int x = *(unsigned int *)a;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void ll_concat(linked_list_t *dest, linked_list_t *src)
{
    if (!src || !dest)
        return;

    if (!src->head)
    {
        free(src);
        return;
    }

    if (!dest->head)
    {
        dest->head = src->head;
        dest->size += src->size;
        free(src);
        return;
    }

    ll_node_t *node = dest->head;
    for (; node->next; node = node->next)
        ;

    node->next = src->head;
    dest->size += src->size;

    free(src);
}

void ll_free_node(void *node)
{
    free(((ll_node_t *)node)->data);
    free(node);
}
int is_prime(int x)
{
    if (x % 2)
        return 0;
    for (int d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return 0;
    return 1;
}
int nearest_prime(int x)
{
    for (int res = x; res > 0; res++)
        if (is_prime(res))
            return res;
    return 0;
}

void ht_print(hashtable_t *hashtable, FILE *out)
{
    unsigned int cnt = 0;

    // For each bucket
    while (cnt < hashtable->hmax)
    {
        if (hashtable->buckets[cnt])
        {
            // fprintf(out, "\n----------BUCKET NR ::: %d\n", cnt);
            ll_node_t *curr = hashtable->buckets[cnt]->head;
            while (curr != NULL)
            {
                fprintf(out, "%d ", *(int *)(((info *)curr->data)->key));
                curr = curr->next;
            }
        }
        cnt++;
    }

    fprintf(out, "\n");
}

unsigned int
compare_function_ints(void *a, void *b)
{
    unsigned int *str_a = (unsigned int *)a;
    unsigned int *str_b = (unsigned int *)b;

    return str_a - str_b;
}

void allocate_buckets(hashtable_t *ht)
{
    for (unsigned int i = 0; i < ht->hmax; i++)
        ht->buckets[i] = ll_create(sizeof(info));
}

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
          unsigned int (*compare_function)(void *, void *))
{
    hashtable_t *new_table = malloc(sizeof(hashtable_t));

    if (!new_table)
        return NULL;

    new_table->buckets = malloc(hmax * sizeof(linked_list_t *));

    new_table->hmax = hmax;
    new_table->hash_function = hash_function;
    new_table->compare_function = compare_function;
    new_table->size = 0;

    allocate_buckets(new_table);

    return new_table;
}

int32_t
ht_has_key(hashtable_t *ht, void *key)
{
    if (!ht)
        return 0;

    unsigned int hash = ht->hash_function(key) % ht->hmax;

    ll_node_t *node = ht->buckets[hash]->head;

    while (node)
    {
        if (!ht->compare_function(key, ((info *)node->data)->key))
            return 1;

        node = node->next;
    }

    return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
    if (!ht)
        return NULL;

    unsigned int hash = ht->hash_function(key) % ht->hmax;

    ll_node_t *node = ht->buckets[hash]->head;

    while (node)
    {
        if (!ht->compare_function(key, ((info *)node->data)->key))
            return ((info *)node->data)->value;

        node = node->next;
    }

    return NULL;
}

linked_list_t *
ht_convert_to_list(hashtable_t *ht)
{
    linked_list_t *merged_list = ll_create(ht->buckets[0]->data_size);

    for (unsigned int i = 0; i < ht->hmax; i++)
        ll_concat(merged_list, ht->buckets[i]);

    return merged_list;
}

void ht_rehash(hashtable_t *ht, info *entry)
{
    if (!ht)
        return;

    unsigned int hash = ht->hash_function(entry->key) % ht->hmax;
    linked_list_t *list = ht->buckets[hash];

    ll_add_nth_node(list, 0, entry);
}

void ht_resize(hashtable_t *ht)
{
    linked_list_t *ht_content = ht_convert_to_list(ht);
    ht->hmax = 2 * ht->hmax;

    ht->buckets = realloc(ht->buckets, ht->hmax * sizeof(linked_list_t *));
    allocate_buckets(ht);

    for (ll_node_t *node = ht_content->head; node; node = node->next)
        ht_rehash(ht, node->data);

    ll_free(&ht_content, ll_free_node);
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
            void *value, unsigned int value_size)
{
    if (!ht)
        return;

    double load_factor = (double)ht->size / ht->hmax;
    if (load_factor > 1)
    {
        printf("///////////////////////////////////RESIZING NOW------ at HMAX %d\n", ht->hmax);
        ht_resize(ht);
    }

    unsigned int hash = ht->hash_function(key) % ht->hmax;

    linked_list_t *list = ht->buckets[hash];
    ll_node_t *node = list->head;

    while (node)
    {
        if (!ht->compare_function(key, ((info *)node->data)->key))
        {
            ((info *)node->data)->value =
                realloc(((info *)node->data)->value, value_size);
            memcpy(((info *)node->data)->value, value, value_size);
            return;
        }

        node = node->next;
    }

    info *new_info = malloc(sizeof(info));

    new_info->key = malloc(key_size);
    memcpy(new_info->key, key, key_size);

    new_info->value = malloc(value_size);
    memcpy(new_info->value, value, value_size);

    ht->size++;

    ll_add_nth_node(list, 0, new_info);
    free(new_info);
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
    if (!ht)
        return;

    unsigned int hash = ht->hash_function(key) % ht->hmax;

    size_t i = 0;
    linked_list_t *list = ht->buckets[hash];
    ll_node_t *node = list->head;

    while (node)
    {
        if (!ht->compare_function(key, ((info *)node->data)->key))
        {
            node = ll_remove_nth_node(list, i);
            free(((info *)node->data)->value);
            free(((info *)node->data)->key);
            free(node->data);
            free(node);
            break;
        }
        i++;
        node = node->next;
    }

    ht->size--;
}

void ht_free(hashtable_t *ht)
{
    ll_node_t *node = NULL;

    for (size_t i = 0; i < ht->hmax; i++)
    {
        while (ll_get_size(ht->buckets[i]) > 0)
        {
            node = ll_remove_nth_node(ht->buckets[i], 0);

            free(((info *)node->data)->key);
            free(((info *)node->data)->value);
            free(node->data);
            free(node);
        }
        free(ht->buckets[i]);
    }

    free(ht->buckets);
}

void ll_free_node_with_hashtables(void *node)
{
    ll_node_t *__node = node;

    ht_free(((info *)__node->data)->value);
    free(((info *)__node->data)->value);
    free(((info *)__node->data)->key);
    free(((ll_node_t *)__node)->data);
    free(__node);
}

void free_ll_of_ht(linked_list_t *hashtables)
{
    ll_free(&hashtables, ll_free_node_with_hashtables);
}
