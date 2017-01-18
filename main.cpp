#include <iostream>
#include <cstdio>
#include <sys/time.h>
#include "tss.h"
#include "fc.h"
#include "rbfc.h"

#define NUM 64

using std::cin;
using std::cout;
using std::endl;

uint32_t address(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return ((uint32_t)a << 24)
        + ((uint32_t)b << 16)
        + ((uint32_t)c << 8)
        + ((uint32_t)d);
}

void get_flows(struct entry *ips)
{
    FILE *f = fopen("flows.csv", "r");
    for (int i = 0; i < 1024 * NUM; ++i)
    {
        fscanf(f, "%d %d", &ips[i].from, &ips[i].to);
        ips[i].from_pre = 0;
        ips[i].to_pre = 0;
        ips[i].desc = 1;
    }
    fclose(f);
}

void get_inputs(struct entry *ips)
{
    FILE *f = fopen("inputs.csv", "r");

    /*
    // for lookup
    for (int i = 0; i < 65536; ++i)
    {
        fscanf(f, "%d %d", &ips[i].from, &ips[i].to);
        ips[i].from_pre = 0;
        ips[i].to_pre = 0;
        ips[i].desc = 1;
    }
    */

    // for update
    uint32_t fp, tp;
    for (int i = 0; i < 65536; ++i)
    {
        fscanf(f, "%d %d %d %d", &ips[i].from, &fp, &ips[i].to, &tp);
        ips[i].from_pre = (uint8_t)fp;
        ips[i].to_pre = (uint8_t)tp;
        ips[i].desc = 1;
    }

    fclose(f);
}

void get_rules(struct entry *nets)
{
    uint32_t fp, tp;
    FILE *f = fopen("rules.csv", "r");
    for (int i = 0; i < 1024 * NUM; ++i)
    {
        fscanf(f, "%d %d %d %d", &nets[i].from, &fp, &nets[i].to, &tp);
        nets[i].from_pre = (uint8_t)fp;
        nets[i].to_pre = (uint8_t)tp;
        nets[i].desc = 1;
    }
    fclose(f);
}

/* lookup test */
void lookup_main()
{
    // load test data
    struct entry ips[1024 * NUM];
    struct entry nets[1024 * NUM];
    get_flows(ips);
    get_rules(nets);

    init_tss();
    init_cache();
    init_rbcache();
    
    // insert into spd
    for (int i = 0; i < NUM * 1024; ++i)
    {
        add_entry(&nets[i]);
    }

    // insert into cache
    for (int i = 0; i < NUM * 1024; ++i)
    {
        add_cache(ips[i].from, ips[i].to, (uint8_t)1);
        add_rbcache(ips[i].from, ips[i].to, (uint8_t)1);
    }
    
    dump_tss();
    dump_fc();
    dump_rbfc();

    // load inputs
    struct entry inputs[1024 * 64];
    get_inputs(inputs);

    //time
    struct timeval start, end, diff;

    for (int c = 0; c < 3; ++c)
    {
    //-----------------
    gettimeofday(&start, 0);
    // spd lookup
    for (int i = 0; i < 65536; ++i)
    {
        get_entry(inputs[i].from, inputs[i].to);       
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "SPD lookup takes: \033[31m" << diff.tv_usec << "\033[0mus." << endl;

    gettimeofday(&start, 0);
    // cache lookup
    for (int i = 0; i < 65536; ++i)
    {
        get_cache(inputs[i].from, inputs[i].to);       
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "Hashtable cache lookup takes: \033[31m" << diff.tv_usec << "\033[0mus." << endl;

    gettimeofday(&start, 0);
    // cache lookup
    for (int i = 0; i < 65536; ++i)
    {
        get_rbcache(inputs[i].from, inputs[i].to);       
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "Red-black tree cache lookup takes: \033[31m" << diff.tv_usec << "\033[0mus." << endl;
    //-----------------
    }//end for

    free_tss();
    free_cache();
    free_rbcache();
}

int main()
{
    init_cache();
    init_rbcache();

    struct entry ips[1024 * NUM];
    get_flows(ips);

    struct entry inputs[65536];
    get_inputs(inputs);

    for (int i = 0; i < 1024 * NUM; ++i)
    {
        add_cache(ips[i].from, ips[i].to, (uint8_t)1);
        add_rbcache(ips[i].from, ips[i].to, (uint8_t)1);
    }
    dump_fc();
    dump_rbfc();

    //time
    struct timeval start, end, diff;

    for (int c = 0; c < 3; ++c)
    {
    //-----------------
    gettimeofday(&start, 0);
    for (int i = 0; i < 1024; ++i)
    {
        update_cache(&inputs[i]);
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "Hashtable cache update takes: \033[31m" << diff.tv_usec << "\033[0mus." << endl;

    gettimeofday(&start, 0);
    for (int i = 0; i < 1024; ++i)
    {
        update_rbcache(&inputs[i]);
    }
    gettimeofday(&end, 0);
    timersub(&end, &start, &diff);
    cout << "Red-black tree cache update takes: \033[31m" << diff.tv_usec << "\033[0mus." << endl;
    //-----------------
    } //for

    free_cache();
    free_rbcache();

    return 0;
}

