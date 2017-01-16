#include <iostream>
#include "tss.h"

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

int main()
{
    init_tss();
    struct entry e;
    e.from = address(192,168,1,0);
    e.from_pre = 24;
    e.to = address(192,168,2,0);
    e.to_pre = 24;
    e.desc = 1;
    add_entry(&e);

    dump();
    cout << (uint32_t)get_entry(address(192,168,2,5), address(192,168,2,55)) << endl;

    free_tss();
    return 0;
}

