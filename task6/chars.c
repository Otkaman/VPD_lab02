#include <stdio.h>

int main(void)
{
    char a[] = {'M', 'A', 'I', '3', '0', '4'};
    short b[] = {'M', 'A', 'I', '3', '0', '4'};
    int c[] = {'M', 'A', 'I', '3', '0', '4'};
    unsigned long long d[] = {'M', 'A', 'I', '3', '0', '4'};

    for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i) printf("%c", a[i]);
    printf("\nsizeof(a) = %zu\n", sizeof(a));
    for (size_t i = 0; i < sizeof(b) / sizeof(b[0]); ++i) printf("%c", (char)b[i]);
    printf("\nsizeof(b) = %zu\n", sizeof(b));
    for (size_t i = 0; i < sizeof(c) / sizeof(c[0]); ++i) printf("%c", (char)c[i]);
    printf("\nsizeof(c) = %zu\n", sizeof(c));
    for (size_t i = 0; i < sizeof(d) / sizeof(d[0]); ++i) printf("%c", (char)d[i]);
    printf("\nsizeof(d) = %zu\n", sizeof(d));

    /* Experiment with %s: add '\0' to each array. */
    char a0[] = {'M', 'A', 'I', '3', '0', '4', '\0'};
    short b0[] = {'M', 'A', 'I', '3', '0', '4', '\0'};
    int c0[] = {'M', 'A', 'I', '3', '0', '4', '\0'};
    unsigned long long d0[] = {'M', 'A', 'I', '3', '0', '4', '\0'};

    printf("\n%%s experiment (only char[] is a real C string):\n");
    printf("a0 -> \"%s\"\n", a0);
    /* On little-endian, 'M' is followed by zero bytes, so %s stops after "M".
     * Casting to (char *) hides the compiler warning; this is only for observation. */
    printf("b0 -> \"%s\"\n", (const char *)b0);
    printf("c0 -> \"%s\"\n", (const char *)c0);
    printf("d0 -> \"%s\"\n", (const char *)d0);
    return 0;
}
