#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"


int main()
{
    int i, j;
    for (i = 64; i >= 0; --i)
        for (j = 32; j >= 0; --j)
            if (i-j >= 0 && i-j <= 32)
                printf("%d %d\n", j, i-j);
    return 0;
}
