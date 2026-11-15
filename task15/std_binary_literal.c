//15.2

/* 0b literals are standard in C23 (GCC also allows them as an extension) */
#include <stdio.h>
int main(void) { int d = 0b100110000; printf("%d\n", d); return 0; }
