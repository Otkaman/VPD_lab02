//15.2

/* C17: auto = storage class (x would be implicit int in C89 only; error in C99+).
 * C23: auto = type inference. */
#include <stdio.h>
int main(void)
{
    auto inferred_value = 304;
    printf("%d\n", inferred_value);
    return 0;
}
