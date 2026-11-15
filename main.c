/* VPD Lab 2: stages (switch/enum), for/while/do-while, progress bar,
 * three timers, flags, extern/static.
 * Usage: ./vpd_lab2 [rounds_per_item] [items_per_stage]
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "device.h"
#include "timer.h"

#define DEFAULT_ROUNDS 2500u
#define DEFAULT_ITEMS  200000u   /* >= 100000 per stage */

enum LabStage {
    STAGE_FOR,
    STAGE_WHILE,
    STAGE_DO_WHILE,
    STAGE_DONE
};

enum LabFlag {
    FLAG_TYPES  = 1u << 0,
    FLAG_ARRAYS = 1u << 1,
    FLAG_BITS   = 1u << 2,
    FLAG_FOR    = 1u << 3,
    FLAG_WHILE  = 1u << 4,
    FLAG_DO     = 1u << 5
};

enum RegisterBit {
    REG_POWER = 1u << 0,
    REG_READY = 1u << 1,
    REG_ERROR = 1u << 2,
    REG_TX    = 1u << 3,
    REG_RX    = 1u << 4
};

static void print_binary8(unsigned value)
{
    for (int bit = 7; bit >= 0; --bit) {
        putchar((value & (1u << bit)) ? '1' : '0');
    }
}

static void print_progress(const char *stage, unsigned percent)
{
    const unsigned width = 30;
    unsigned filled = percent * width / 100;

    printf("\r%-12s [", stage);
    for (unsigned i = 0; i < width; ++i) {
        putchar(i < filled ? '#' : '-');
    }
    printf("] %3u%%", percent);
    fflush(stdout);
}

/* Integer workload on uint64_t: shifts, XOR, add, multiply mod 2^64. */
static uint64_t mix(uint64_t x, unsigned rounds)
{
    for (unsigned r = 0; r < rounds; ++r) {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        x = x * 6364136223846793005ULL + 1442695040888963407ULL;
    }
    return x;
}

/* Prints progress only when the integer percent changes (<= 101 prints). */
static void tick_progress(const char *name, unsigned long done,
                          unsigned long total, unsigned *last)
{
    unsigned percent = (unsigned)(done * 100ul / total);
    if (percent != *last) {
        *last = percent;
        print_progress(name, percent);
    }
}

static uint64_t stage_for(unsigned long items, unsigned rounds)
{
    uint64_t acc = 0;
    unsigned last = 101;

    for (unsigned long i = 0; i < items; ++i) {
        acc ^= mix(i + 1, rounds);
        device_tick();
        tick_progress("for", i + 1, items, &last);
    }
    printf("\n");
    return acc;
}

static uint64_t stage_while(unsigned long items, unsigned rounds)
{
    uint64_t acc = 0;
    unsigned last = 101;
    unsigned long i = 0;

    while (i < items) {
        acc += mix(i + 1, rounds);
        device_tick();
        ++i;
        tick_progress("while", i, items, &last);
    }
    printf("\n");
    return acc;
}

static uint64_t stage_do_while(unsigned long items, unsigned rounds)
{
    uint64_t acc = 0;
    unsigned last = 101;
    unsigned long i = 0;

    do {
        acc = acc * 31u + mix(i + 1, rounds);
        device_tick();
        ++i;
        tick_progress("do-while", i, items, &last);
    } while (i < items);
    printf("\n");
    return acc;
}

int main(int argc, char *argv[])
{
    unsigned rounds = DEFAULT_ROUNDS;
    unsigned long items = DEFAULT_ITEMS;
    unsigned completed = 0;
    uint64_t checksum = 0;
    enum LabStage stage = STAGE_FOR;

    if (argc > 1) rounds = (unsigned)strtoul(argv[1], NULL, 10);
    if (argc > 2) items = strtoul(argv[2], NULL, 10);
    if (items < 100000ul) items = 100000ul;
    if (rounds == 0) rounds = 1;

    /* quick demos for the "types" / "arrays" / "bits" flags */
    printf("sizeof(int)=%zu sizeof(void*)=%zu sizeof(uint64_t)=%zu\n",
           sizeof(int), sizeof(void *), sizeof(uint64_t));
    completed |= FLAG_TYPES;

    int arr[8] = {0};
    printf("arr: %zu bytes, %zu elements\n",
           sizeof(arr), sizeof(arr) / sizeof(arr[0]));
    completed |= FLAG_ARRAYS;

    uint8_t reg = 0;
    reg |= REG_POWER | REG_READY;
    reg ^= REG_TX;
    reg &= (uint8_t)~REG_READY;
    printf("reg = %u = 0x%02X = ", (unsigned)reg, (unsigned)reg);
    print_binary8(reg);
    printf("\n");
    completed |= FLAG_BITS;

    printf("rounds=%u items/stage=%lu\n", rounds, items);

    /* all three timers start right before the workload */
    time_t wall_start = time(NULL);
    clock_t cpu_start = clock();
    double hi_start = highres_seconds();

    while (stage != STAGE_DONE) {
        switch (stage) {
        case STAGE_FOR:
            checksum ^= stage_for(items, rounds);
            completed |= FLAG_FOR;
            stage = STAGE_WHILE;
            break;
        case STAGE_WHILE:
            checksum ^= stage_while(items, rounds);
            completed |= FLAG_WHILE;
            stage = STAGE_DO_WHILE;
            break;
        case STAGE_DO_WHILE:
            checksum ^= stage_do_while(items, rounds);
            completed |= FLAG_DO;
            stage = STAGE_DONE;
            break;
        case STAGE_DONE:
            break;
        }
    }

    double hi_end = highres_seconds();
    clock_t cpu_end = clock();
    time_t wall_end = time(NULL);

    printf("checksum: 0x%016llX\n", (unsigned long long)checksum);
    printf("time/difftime: %.0f s\n", difftime(wall_end, wall_start));
    printf("clock:         %.6f s (CLOCKS_PER_SEC=%ld)\n",
           (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC,
           (long)CLOCKS_PER_SEC);
    printf("high-res time: %.6f s\n", hi_end - hi_start);

    /* extern / static demo */
    printf("global_runs = %lu\n", global_runs);
    printf("device_internal_calls() = %lu\n", device_internal_calls());
    /* extern unsigned long internal;  <- compiles, but the linker fails:
     * undefined reference to 'internal' (it is static in device.c). */

    const unsigned all = FLAG_TYPES | FLAG_ARRAYS | FLAG_BITS |
                         FLAG_FOR | FLAG_WHILE | FLAG_DO;
    printf("completed = 0x%02X = ", completed);
    print_binary8(completed);
    printf("\nall flags set: %s\n", completed == all ? "yes" : "NO");

    return completed == all ? 0 : 1;
}
