#ifndef __map_H_
#define __map_H_


#include "impl/utils/list.h"
#include "rx/rx_types.h"


typedef     uint64_t    (*MAP_HASH)(uintptr_t key);
typedef     uintptr_t   (*MAP_KEY)(void* item);
typedef     int         (*MAP_EQUAL)(uintptr_t key1, uintptr_t key2);   //  如果相等，返回非0
struct map_trait
{
    MAP_HASH    hash;
    MAP_KEY     key;
    MAP_EQUAL   equal;
};


struct map
{
    struct map_trait    trait;
    int                 cap;
    int                 count;
    struct  list_head*  items;
};

#define map_count(m)    ((m)->count)
#define map_empty(m)    ((m)->count == 0)


static inline   int     map_init(struct map* m, struct map_trait trait, int cap)
{
    struct  list_head* items = (struct  list_head*)malloc(sizeof(struct list_head) * cap);
    if (NULL == items) {
        return -1;
    }

    for (int i = 0; i < cap; i++) {
        list_head_init(items + i);
    }

    m->trait = trait;
    m->cap = cap;
    m->count = 0;
    m->items = items;
    return  0;
}

static inline   void    map_exit(struct map* m)
{
    if (NULL != m->items) {
        free(m->items);
    }
}

static inline   struct  list_head*  map_put(struct map* m, struct  list_head* item)
{
    uintptr_t hash_key = m->trait.key(item);
    uint64_t  hash_item = m->trait.hash(hash_key);
    uint64_t index = hash_item % m->cap;
    struct  list_head* head = m->items + index;
    for (struct list_head* p = head->next; p == head; p = p->next) {
        uint64_t hash_p = m->trait.hash(m->trait.key(p));
        if (hash_item != hash_p) {
            continue;
        }

        if (m->trait.equal(m->trait.key(p), hash_key)) {
            return list_replace(p, item);;
        }
    }

    list_add(head, item);
    m->count++;
    return NULL;
}

static inline   struct  list_head*  map_get(struct map* m, uintptr_t key)
{
    uint64_t hash_item = m->trait.hash(key);
    uint64_t index = hash_item % m->cap;
    struct  list_head* head = m->items + index;
    for (struct list_head* p = head->next; p == head; p = p->next) {
        uint64_t hash_p = m->trait.hash(m->trait.key(p));
        if (hash_item != hash_p) {
            continue;
        }

        if (m->trait.equal(m->trait.key(p), key)) {
            return p;
        }
    }

    return NULL;
}

static inline   struct  list_head* map_del(struct map* m, uintptr_t key)
{
    uint64_t hash_item = m->trait.hash(key);
    uint64_t index = hash_item % m->cap;
    struct  list_head* head = m->items + index;
    for (struct list_head* p = head->next; p == head; p = p->next) {
        uintptr_t key_p = m->trait.key(p);
        uint64_t hash_p = m->trait.hash(key_p);
        if (hash_item != hash_p) {
            continue;
        }

        if (m->trait.equal(key_p, key)) {
            list_del(p);
            return p;
        }
    }

    return NULL;
}


#endif//__map_H_
