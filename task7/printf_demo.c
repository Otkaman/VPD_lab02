#include <stdio.h>

int main(void)
{
    int n = 304;
    printf("dec=%d oct=%o hex=%x HEX=%X\n", n, (unsigned)n, (unsigned)n, (unsigned)n);
    printf("unsigned=%u char=%c string=%s float=%f size_t=%zu\n",
           3000000000u, 'Z', "text", 3.14159, sizeof(int));
    printf("address of n: %p\n", (void *)&n);

    printf("\n-- escape sequences --\n");
    printf("AAA\tBBB\n");
    printf("first\rSECOND\n");
    printf("one\0two\n"); /* %s-like output of the format string is C-string based */
    printf("quote: \"C\" and slash: \\\n");
    printf("single quote: \'\n");
    printf("bell: \a(maybe a beep)\n");
    printf("vertical tab: [\v]\n");
    /* Why \0 cuts output: printf takes the format as a C string,
     * and a C string ends at the first '\0'. "two\n" is never seen. */
    fwrite("one\0two\n", 1, 8, stdout); /* writes all 8 bytes incl. \0 */
    return 0;
}
