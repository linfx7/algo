#include "tss.h"
#include <cstddef>
#include <iostream>
#include <tr1/unordered_map>
#include <map>

using std::make_pair;
using std::tr1::unordered_map;
using std::map;

//typedef unordered_map<uint64_t, uint8_t> cmap;
typedef map<uint64_t, uint8_t> cmap;

cmap rbfc;

void init_rbcache()
{
    rbfc.clear();
}

void free_rbcache()
{
    rbfc.clear();
}

void add_rbcache(uint32_t from, uint32_t to, uint8_t status)
{
    rbfc[get_key(from, to)] = status;

#ifdef DEBUG
    std::cout << "Red-black tree cache: " 
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

void del_rbcache(uint32_t from, uint32_t to)
{
    rbfc.erase(rbfc.find(get_key(from, to)));
}

uint8_t get_rbcache(uint32_t from, uint32_t to)
{
    // default value, return 255 if no entry found
    uint8_t result = 0xff;
    cmap::iterator it = rbfc.find(get_key(from, to));
    if (it != rbfc.end())
        result = it->second;
    return result;
}

void update_rbcache(struct entry *e)
{

}

void dump_rbfc()
{
    cmap::iterator it = rbfc.begin();
    for (;it != rbfc.end(); it++)
    {
#ifdef DEBUG
        std::cout << "Red-black tree cache: " 
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
    std::cout << "Total red-black tree cache: " << rbfc.size() << std::endl;
}

