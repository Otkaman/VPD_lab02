#include <stdio.h>

int main(void)
{
    int number;
    unsigned int unum;
    double real;

    printf("Enter int: ");
    if (scanf("%d", &number) != 1) {
        fprintf(stderr, "Invalid integer input\n");
        return 1;
    }
    printf("Enter unsigned int: ");
    if (scanf("%u", &unum) != 1) {
        fprintf(stderr, "Invalid unsigned input\n");
        return 1;
    }
    printf("Enter double: ");
    if (scanf("%lf", &real) != 1) {
        fprintf(stderr, "Invalid double input\n");
        return 1;
    }
    printf("int=%d unsigned=%u double=%f\n", number, unum, real);
    return 0;
}
