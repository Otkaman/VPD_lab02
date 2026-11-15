#include <stdio.h>

int main(void) {
    
    /* Classic, also works in C17 and C23*/
    auto int local_value = 304;
    register int reg_hint = 66;

    /*Output by initialyzer - just in C23*/
    auto inferred_value = 304;

    printf("local_value    = %d\n", local_value);
    printf("reg_hint       = %d\n", reg_hint);
    printf("inferred value = %d (sizeof = %zu)\n", inferred_value, sizeof(inferred_value));

    return 0;
}
