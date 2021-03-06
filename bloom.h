#ifndef _BLOOM_H
#define _BLOOM_H

#include <stdbool.h>
#include <stddef.h>

typedef unsigned int (*hash_function)(const void *data, unsigned int seed);
typedef struct bloom_filter *bloom_t;

struct bloom_hash {
    hash_function func;
    unsigned int seed;
    struct bloom_hash *next;
};

struct bloom_filter {
    struct bloom_hash *func;
    void *bits;
    size_t size;
};

/* Creates a new bloom filter with no hash functions and size * 8 bits. */
bloom_t bloom_create(size_t size, unsigned int num_h);

/* Frees a bloom filter. */
void bloom_free(bloom_t filter);

/* Adds a hashing function to the bloom filter. You should add all of the
 * functions you intend to use before you add any items. */
void bloom_add_hash(bloom_t filter, unsigned int seed);

/* Adds an item to the bloom filter. */
void bloom_add(bloom_t filter, const void *item);

/* Tests if an item is in the bloom filter.
 *
 * Returns false if the item has definitely not been added before. Returns true
 * if the item was probably added before. */
bool bloom_test(bloom_t filter, const void *item);

/* hash functions */
/*
unsigned int djb2(const void *);
unsigned int jenkins(const void *);
*/
unsigned int murmur3(const void *, unsigned int);

#endif
