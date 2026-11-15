/* -DSHOW_UB enables the undefined-behavior expression (do NOT trust its output). */
#include <stdio.h>

int main(void)
{
    int a = 5, b = 3;
    int r1 = a+++b;               /* tokens: a ++ + b  ->  (a++) + b */
    printf("a+++b : r=%d a=%d b=%d\n", r1, a, b);

    a = 5; b = 3;
    int r2 = b---a;               /* tokens: b -- - a  ->  (b--) - a */
    printf("b---a : r=%d a=%d b=%d\n", r2, a, b);

#ifdef SHOW_UB
    a = 0; b = 5;
    a = b++ + ++b;                /* UB: b modified twice without sequencing */
    printf("a = b++ + ++b -> a=%d b=%d (UNDEFINED, not a correct answer)\n", a, b);
#endif

    printf("\n-- safe expressions --\n");
    printf("2 + 3 * 4        = %d\n", 2 + 3 * 4);
    printf("(2 + 3) * 4      = %d\n", (2 + 3) * 4);
    printf("1 << 3 + 1       = %d\n", 1 << 3 + 1);     /* + binds tighter than << */
    printf("(1 << 3) + 1     = %d\n", (1 << 3) + 1);
    printf("5 < 3 == 0       = %d\n", 5 < 3 == 0);
    printf("6 & 3 ^ 1        = %d\n", 6 & 3 ^ 1);       /* & before ^ */
    printf("6 & (3 ^ 1)      = %d\n", 6 & (3 ^ 1));
    printf("1 || 0 && 0      = %d\n", 1 || 0 && 0);     /* && before || */
    printf("(1 || 0) && 0    = %d\n", (1 || 0) && 0);
    return 0;
}
