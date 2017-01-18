#include <iostream>
#include <cstdio>
#include "tss.h"
#include "fc.h"
#include <sys/time.h>

#define NUM 64

using std::cin;
using std::cout;
using std::endl;

struct ip
{
    uint32_t from, to;
};

struct net
{
    uint32_t from, to;
    uint8_t from_pre, to_pre;
};

uint32_t address(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return ((uint32_t)a << 24)
        + ((uint32_t)b << 16)
        + ((uint32_t)c << 8)
        + ((uint32_t)d);
}

void get_ips(struct ip *ips)
{
    FILE *f = fopen("flows.csv", "r");
    for (int i = 0; i < 1024 * NUM; ++i)
    {
        fscanf(f, "%d %d", &ips[i].from, &ips[i].to);
    }
    fclose(f);
}

void get_inputs(struct ip *ips)
{
    FILE *f = fopen("inputs.csv", "r");
    for (int i = 0; i < 65536; ++i)
    {
        fscanf(f, "%d %d", &ips[i].from, &ips[i].to);
    }
    fclose(f);
}

void get_nets(struct net *nets)
{
    uint32_t fp, tp;
    FILE *f = fopen("rules.csv", "r");
    for (int i = 0; i < 1024 * NUM; ++i)
    {
        fscanf(f, "%d %d %d %d", &nets[i].from, &fp, &nets[i].to, &tp);
        nets[i].from_pre = (uint8_t)fp;
        nets[i].to_pre = (uint8_t)tp;
    }
    fclose(f);
}

int main()
{
    // load test data
    struct ip ips[1024 * NUM];
    struct net nets[1024 * NUM];
    get_ips(ips);
    get_nets(nets);

    init_tss();
    init_cache();
    
    // insert into spd
    for (int i = 0; i < NUM * 1024; ++i)
    {
        struct entry e;
        e.from = nets[i].from;
        e.from_pre = nets[i].from_pre;
        e.to = nets[i].to;
        e.to_pre = nets[i].to_pre;
        e.desc = 1;
        add_entry(&e);
    }

    // insert into cache
    for (int i = 0; i < NUM * 1024; ++i)
    {
        add_cache(ips[i].from, ips[i].to, (uint8_t)1);
    }
    
    dump_tss();
    dump_fc();

    // load inputs
    struct ip inputs[1024 * 64];
    get_inputs(inputs);

    //time
    struct timeval start, end, diff;

    gettimeofday(&start, 0);
    // spd lookup
    for (int i = 0; i < 65536; ++i)
    {
        get_entry(inputs[i].from, inputs[i].to);       
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "SPD lookup takes: " << diff.tv_usec << "us." << endl;

    gettimeofday(&start, 0);
    // cache lookup
    for (int i = 0; i < 65536; ++i)
    {
        get_cache(inputs[i].from, inputs[i].to);       
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "Cache lookup takes: " << diff.tv_usec << "us." << endl;

    free_tss();
    free_cache();
    return 0;
}

