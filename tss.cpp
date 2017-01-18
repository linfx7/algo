#include "tss.h"
#include <cstddef>
#include <list>
#include <iostream>

using std::list;
using std::make_pair;
using std::tr1::unordered_map;

uint32_t masks[] = {0x00000000,                         
    0x80000000, 0xc0000000, 0xe0000000, 0xf0000000,
    0xf8000000, 0xfc000000, 0xfe000000, 0xff000000,
    0xff800000, 0xffc00000, 0xffe00000, 0xfff00000,
    0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000,
    0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000,
    0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00,
    0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0,
    0xfffffff8, 0xfffffffc, 0xfffffffe, 0xffffffff,
    }; 

list<struct tuple> tp;

void init_tss()
{
    free_tss();
}

void free_tss()
{
    tp.clear();
}

void add_tuple(uint8_t from_pre, uint8_t to_pre)
{
    struct tuple n;
    n.from_pre = from_pre;
    n.to_pre = to_pre;
    n.ht_size = 0;

    list<struct tuple>::iterator it = tp.begin();
    for (; it != tp.end(); it++)
    {
        if (it->from_pre + it->to_pre < from_pre + to_pre)
            break;
    }
    tp.insert(it, n);

    //std::cout << "Add tuple: " << (uint32_t)from_pre << ", " << (uint32_t)to_pre << std::endl;
}

void del_tuple(uint8_t from_pre, uint8_t to_pre)
{
    list<struct tuple>::iterator it = tp.begin();
    for (; it != tp.end(); it++)
    {
        if (it->from_pre == from_pre && it->to_pre == to_pre)
            break;
    }
    if (it != tp.end())
    {
        tp.erase(it);
    }
}

int tmpc = 0;

void add_entry(struct entry *e)
{
    list<struct tuple>::iterator it = tp.begin();
    for (; it != tp.end(); it++)
    {
        if (it->from_pre == e->from_pre && it->to_pre == e->to_pre)
            break;
    }

    // tuple not found
    if (it == tp.end())
    {
        // add tuple
        add_tuple(e->from_pre, e->to_pre);
        // add entry again
        add_entry(e);
    }
    else
    {
        if (it->ht.find(get_key(e->from & masks[e->from_pre],e->to & masks[e->to_pre])) != it->ht.end())
            tmpc++;
        // add to the found tuple
        it->ht[get_key(e->from & masks[e->from_pre],
                       e->to & masks[e->to_pre])] = e->desc;

#ifdef DEBUG
        std::cout << "Entry: " 
            << ((e->from >> 24) & 0x000000ff) << "." 
            << ((e->from >> 16) & 0x000000ff) << "." 
            << ((e->from >>  8) & 0x000000ff) << "." 
            << ( e->from        & 0x000000ff) << " "
            << ((e->to >> 24) & 0x000000ff) << "." 
            << ((e->to >> 16) & 0x000000ff) << "." 
            << ((e->to >>  8) & 0x000000ff) << "." 
            << ( e->to        & 0x000000ff) << " ==> "
            << "Tuple: " << (uint32_t)e->from_pre << ", " << (uint32_t)e->to_pre
            << std::endl;
#endif
    }
}

void del_entry(struct entry *e)
{
    // find tuple first
    list<struct tuple>::iterator it = tp.begin();
    for (; it != tp.end(); it++)
    {
        if (it->from_pre == e->from_pre && it->to_pre == e->to_pre)
            break;
    }
    if (it != tp.end())
    {
        // then get the entry
        unordered_map<uint64_t, uint8_t>::iterator itt =
            it->ht.find(get_key(e->from & masks[e->from_pre], e->to & masks[e->to_pre]));
        // delete the found entry
        if (itt != it->ht.end())
            it->ht.erase(itt);
    }
}

uint8_t get_entry(uint32_t from, uint32_t to)
{
    // default value, return 255 if no entry found
    uint8_t result = 0xff;
    // find tuple first
    list<struct tuple>::iterator it = tp.begin();
    unordered_map<uint64_t, uint8_t>::iterator itm;
    for (; it != tp.end(); it++)
    {
        itm = it->ht.find(get_key(from & masks[it->from_pre], to & masks[it->to_pre]));
        if (itm != it->ht.end())
        {
            result = itm->second;
            //break;
        }
    }

#ifdef DEBUG
    std::cout << "Packet: " 
            << ((from >> 24) & 0x000000ff) << "." 
            << ((from >> 16) & 0x000000ff) << "." 
            << ((from >>  8) & 0x000000ff) << "." 
            << ( from        & 0x000000ff) << " "
            << ((to >> 24) & 0x000000ff) << "." 
            << ((to >> 16) & 0x000000ff) << "." 
            << ((to >>  8) & 0x000000ff) << "." 
            << ( to        & 0x000000ff) << " ==> "
            << "Action: " << (uint32_t)result
            << std::endl;
#endif

    return result;
}

uint64_t get_key(uint32_t from, uint32_t to)
{
    return ((0x00000000ffffffff & (uint64_t)from) << 32) 
        | (0x00000000ffffffff & (uint64_t)to);
}

void dump_tss()
{
    uint32_t total = 0;
    list<struct tuple>::iterator it = tp.begin();
    for (; it != tp.end(); it++)
    {
        total += it->ht.size();
        unordered_map<uint64_t, uint8_t>::iterator itm;
        for (itm = it->ht.begin(); itm != it->ht.end(); itm++)
        {
#ifdef DEBUG
            std::cout << "Entry: " 
                    << ((itm->first >> 56) & 0x000000ff) << "." 
                    << ((itm->first >> 48) & 0x000000ff) << "." 
                    << ((itm->first >> 40) & 0x000000ff) << "." 
                    << ((itm->first >> 32) & 0x000000ff) << "/" 
                    << (uint32_t)it->from_pre << "  "
                    << ((itm->first >> 24) & 0x000000ff) << "." 
                    << ((itm->first >> 16) & 0x000000ff) << "." 
                    << ((itm->first >>  8) & 0x000000ff) << "." 
                    << ((itm->first      ) & 0x000000ff) << "/" 
                    << (uint32_t)it->to_pre << "  "
                    << (uint32_t)itm->second
                    << std::endl;
#endif
        }
    }
    std::cout << "Total SPs: " << total << "+" << tmpc << std::endl;
}

