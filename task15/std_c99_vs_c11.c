//15.2

/* _Static_assert appears in C11 */
#include <stdio.h>
_Static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
int main(void) { puts("ok"); return 0; }
