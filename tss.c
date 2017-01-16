#include "tss.h"
#include "hashtable.h"
#include "entry.h"
#include <stdlib.h>

struct tuple tuples[32][32];
uint32_t total;

void init_tss()
{
    int i, j;
    for (i = 0; i < 32; ++i)
        for (j = 0; j < 32; ++j) {
            tuples[i][j].ht = NULL;
        }
    total = 0;
}

void free_tss()
{
    int i, j;
    for (i = 0; i < 32; ++i)
        for (j = 0; j < 32; ++j)
            if (tuples[i][j].ht != NULL)
                free_tuple(&tuples[i][j]);
    total = 0;
}

void init_tuple(struct tuple *t)
{
    t->ht = (struct t_hashtable *) malloc(struct t_hashtable);
    t->ht_size = 0;
}

void free_tuple(struct tuple *t)
{
    int tmp_i;
    struct hlist_node *tmp_hn;
    struct tuple_entry *tmp_te;

    hash_for_each_safe(t->ht->hhead, tmp_i, tmp_hn, tmp_te, hn)
    {
        // delete entry from hashtable
        hash_del(&tmp_te->hn);
        // free the entry
        free(tmp_te->e);
        free(tmp_te);
    }

    free(t->ht);
}

void add_entry(struct entry *e)
{
    // construct the tuple_entry
    struct tuple_entry *te = (struct tuple_entry *) malloc(sizeof(struct tuple_entry));
    te->e = e;

    add_tuple_entry(te);
}

void add_tuple_entry(struct tuple_entry *te)
{
    struct tuple *t = &tuples[te->e->from_pre][te->e->to_pre];

    // if not inited
    if (t->ht == NULL)
        init_tuple(t);

    hash_add(t->ht->hhead, &te->hn, get_key(te->e->from & masks[te->e->from_pre], te->e->to & masks[te->e->to_pre]));
}

void del_entry(struct entry *e)
{
    // TODO
}

void del_tuple_entry(struct tuple_entry *e)
{
    hash_del(&e->hn);
    free(e);
}

struct entry *get_entry(uint32_t from, uint32_t to)
{
    return get_tuple_entry(from, to)->e;
};

struct tuple_entry *get_tuple_entry(uint32_t from, uint32_t to)
{
    struct tuple_entry *result = NULL;
    uint8_t i, j;
    for (i = 64; i >= 0; --i)
        for (j = 32; j >= 0; --j)
            if (i-j >= 0 && i-j <= 32)
            {
                struct tuple *t = &tuples[j][i-j];
                if (t->ht != NULL)
                {
                    struct tuple_entry *tmp_te;
                    hash_for_each_possible(t->ht->hhead, tmp_te, hn, get_key(from & masks[j], to & masks[i-j]))
                    {
                        if(tmp_te->e, from, to)
                            result = tmp_te;
                    }
                }
            }
    return result;
}

void update_cache(struct entry *e)
{
    // no use
    uint8_t i, j;
    struct tuple *t = &tuples[32][32];
    if (t->ht != NULL)
    {
        struct tuple_entry *tmp_te;
        int tmp_i;
        hash_for_each(t->ht->hhead, tmp_i, tmp_te, hn)
        {
            if (match(e, tmp_te->e->from, tmp_te->e->to))
                tmp_te->e->desc = e->desc;
        }
    }
}

uint64_t get_key(uint32_t from, uint32_t to)
{
    return ((0x00000000ffffffff & from) << 32)
          | (0x00000000ffffffff & to);
}

int match(struct entry *e, uint32_t from, uint32_t to)
{
    if (((e->from & masks[e->from_pre]) == from & masks[e->from_pre]) && ((e->to & masks[e->to_pre]) == to & masks[e->to_pre]))
        return 1;
    else
        return 0;
}
