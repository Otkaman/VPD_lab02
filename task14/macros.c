/* gcc -Wall -Wextra -Wpedantic macros.c -o macros
 * gcc -DDEBUG ...  -> DEBUG_LOG prints to stderr
 * gcc -DSHOW_UB ... -> GOOD_SQUARE(i++) (UB: i modified twice) */
#include <stdio.h>

#define BUFFER_SIZE 304
static const size_t buffer_size = 304;

#define BAD_SQUARE(x) x * x
#define GOOD_SQUARE(x) ((x) * (x))

#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b

#ifdef DEBUG
# define DEBUG_LOG(msg) fprintf(stderr, "DEBUG: %s\n", msg)
#else
# define DEBUG_LOG(msg) ((void)0)
#endif

int main(void)
{
    printf("BUFFER_SIZE=%d buffer_size=%zu\n", BUFFER_SIZE, buffer_size);

    printf("BAD_SQUARE(2 + 3)  = %d\n", BAD_SQUARE(2 + 3));   /* 2 + 3 * 2 + 3 = 11 */
    printf("GOOD_SQUARE(2 + 3) = %d\n", GOOD_SQUARE(2 + 3));  /* 25 */

#ifdef SHOW_UB
    int i = 3;
    printf("GOOD_SQUARE(i++) = %d, i = %d (UB!)\n", GOOD_SQUARE(i++), i);
#endif

    printf("STRINGIFY(hello + 1) = %s\n", STRINGIFY(hello + 1));
    int CONCAT(val, 1) = 42;                 /* declares val1 */
    printf("CONCAT(val, 1) -> val1 = %d\n", val1);

    DEBUG_LOG("this appears only with -DDEBUG");
    return 0;
}
