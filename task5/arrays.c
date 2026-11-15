/* Build:
 *   gcc -Wall -Wextra -Wpedantic -O0 -DUNINIT arrays.c -o arrays_O0_uninit
 *   gcc -Wall -Wextra -Wpedantic -O2 -DUNINIT arrays.c -o arrays_O2_uninit
 *   gcc -Wall -Wextra -Wpedantic -O0 arrays.c -o arrays_O0
 * With -DUNINIT the arrays are NOT initialized: reading them is indeterminate
 * (possibly UB). Printed values are NOT a correct answer, only an observation.
 */
#include <stdio.h>

int main(void)
{
#ifdef UNINIT
    char chars[304];
    int integers[25];
    double reals[25];
    printf("UNINITIALIZED (indeterminate values!)\n");
#else
    char chars[304] = {0};
    int integers[25] = {0};
    double reals[25] = {0};

    for (size_t i = 0; i < sizeof(chars) / sizeof(chars[0]); ++i)
        chars[i] = (char)('A' + i % 26);
    for (size_t i = 0; i < sizeof(integers) / sizeof(integers[0]); ++i)
        integers[i] = (int)(i * i);
    for (size_t i = 0; i < sizeof(reals) / sizeof(reals[0]); ++i)
        reals[i] = (double)i / 4.0;
#endif

    printf("-- integers --\n");
    for (size_t i = 0; i < sizeof(integers) / sizeof(integers[0]); ++i)
        printf("integers[%zu] = %d\n", i, integers[i]);

    printf("-- reals --\n");
    for (size_t i = 0; i < sizeof(reals) / sizeof(reals[0]); ++i)
        printf("reals[%zu] = %f\n", i, reals[i]);

    printf("-- chars (10 per row, codes) --\n");
    for (size_t i = 0; i < sizeof(chars) / sizeof(chars[0]); ++i) {
        printf("%4d", (unsigned char)chars[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    printf("chars:    sizeof=%zu elem=%zu count=%zu\n",
           sizeof(chars), sizeof(chars[0]), sizeof(chars) / sizeof(chars[0]));
    printf("integers: sizeof=%zu elem=%zu count=%zu\n",
           sizeof(integers), sizeof(integers[0]), sizeof(integers) / sizeof(integers[0]));
    printf("reals:    sizeof=%zu elem=%zu count=%zu\n",
           sizeof(reals), sizeof(reals[0]), sizeof(reals) / sizeof(reals[0]));
    return 0;
}
