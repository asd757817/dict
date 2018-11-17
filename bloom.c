#include "bloom.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
unsigned int murmur3(const void *_str, unsigned int seed)
{
    size_t len = strlen(_str);
    const uint8_t *key = _str;
    uint32_t h = seed;

    if (len > 3) {
        const uint32_t *key_x4 = (const uint32_t *) key;
        size_t i = len >> 2;
        do {
            uint32_t k = *key_x4++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = (h * 5) + 0xe6546b64;
        } while (--i);
        key = (const uint8_t *) key_x4;
    }
    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        key = &key[i - 1];
        do {
            k <<= 8;
            k |= *key--;
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

bloom_t bloom_create(size_t size, unsigned int num_h)
{
    bloom_t res = calloc(1, sizeof(struct bloom_filter));
    res->size = size;
    res->bits = calloc(size, 1);

    for (int i = 0; i < num_h; i++)
        bloom_add_hash(res, i);

    return res;
}

void bloom_free(bloom_t filter)
{
    if (filter) {
        while (filter->func) {
            struct bloom_hash *h = filter->func;
            filter->func = h->next;
            free(h);
        }
        free(filter->bits);
        free(filter);
    }
}

void bloom_add_hash(bloom_t filter, unsigned int seed)
{
    struct bloom_hash *h = calloc(1, sizeof(struct bloom_hash));
    h->func = murmur3;
    h->seed = seed;
    struct bloom_hash *last = filter->func;
    while (last && last->next) {
        last = last->next;
    }
    if (last) {
        last->next = h;
    } else {
        filter->func = h;
    }
}

void bloom_add(bloom_t filter, const void *item)
{
    struct bloom_hash *h = filter->func;
    uint8_t *bits = filter->bits;
    while (h) {
        unsigned int hash = h->func(item, h->seed);
        hash %= filter->size;
        bits[hash] = 1;
        h = h->next;
    }
}

bool bloom_test(bloom_t filter, const void *item)
{
    struct bloom_hash *h = filter->func;
    uint8_t *bits = filter->bits;
    while (h) {
        unsigned int hash = h->func(item, h->seed);
        hash %= filter->size;
        if (!(bits[hash])) {
            return false;
        }
        h = h->next;
    }
    return true;
}
