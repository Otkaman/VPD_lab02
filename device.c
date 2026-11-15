#include "device.h"

unsigned long global_runs = 0;       /* external linkage */
static unsigned long internal = 0;   /* file scope, visible only in device.c */

void device_tick(void)
{
    static unsigned long calls = 0;  /* lives whole program, local scope */

    ++global_runs;
    ++internal;
    ++calls;
}

unsigned long device_internal_calls(void)
{
    return internal;
}
