//15.2

/* -std=c90 -Wpedantic: error/warning for // and for (int i...)  */
#include <stdio.h>
int main(void)
{
    // C99 comment
    for (int i = 0; i < 3; ++i) printf("%d\n", i);
    return 0;
}
