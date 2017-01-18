#include "tss.h"
#include <cstddef>
#include <iostream>
#include <tr1/unordered_map>
#include <map>

using std::make_pair;
using std::tr1::unordered_map;
using std::map;

typedef unordered_map<uint64_t, uint8_t> cmap;
//typedef map<uint64_t, uint8_t> cmap;

cmap fc;

void init_cache()
{
    free_tss();
}

void free_cache()
{
    fc.clear();
}

void add_cache(uint32_t from, uint32_t to, uint8_t status)
{
    fc[get_key(from, to)] = status;

#ifdef DEBUG
    std::cout << "Cache: " 
        << ((from >> 24) & 0x000000ff) << "." 
        << ((from >> 16) & 0x000000ff) << "." 
        << ((from >>  8) & 0x000000ff) << "." 
        << ( from        & 0x000000ff) << " "
        << ((to >> 24) & 0x000000ff) << "." 
        << ((to >> 16) & 0x000000ff) << "." 
        << ((to >>  8) & 0x000000ff) << "." 
        << ( to        & 0x000000ff) << " "
        << (uint32_t)status << std::endl;
#endif
}

void del_cache(uint32_t from, uint32_t to)
{
    fc.erase(fc.find(get_key(from, to)));
}

uint8_t get_cache(uint32_t from, uint32_t to)
{
    // default value, return 255 if no entry found
    uint8_t result = 0xff;
    cmap::iterator it = fc.find(get_key(from, to));
    if (it != fc.end())
        result = it->second;
    return result;
}

void update_cache(struct entry *e)
{

}

void dump_fc()
{
    cmap::iterator it = fc.begin();
    for (;it != fc.end(); it++)
    {
#ifdef DEBUG
        std::cout << "Cache: " 
                << ((it->first >> 56) & 0x000000ff) << "." 
                << ((it->first >> 48) & 0x000000ff) << "." 
                << ((it->first >> 40) & 0x000000ff) << "." 
                << ((it->first >> 32) & 0x000000ff) << " " 
                << ((it->first >> 24) & 0x000000ff) << "." 
                << ((it->first >> 16) & 0x000000ff) << "." 
                << ((it->first >>  8) & 0x000000ff) << "." 
                << ((it->first      ) & 0x000000ff) << " " 
                << (uint32_t)it->second
                << std::endl;
#endif
    }
    std::cout << "Total cache: " << fc.size() << std::endl;
}

