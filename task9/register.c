#include <stdint.h>
#include <stdio.h>

enum RegisterBit {
    REG_POWER = 1u << 0,
    REG_READY = 1u << 1,
    REG_ERROR = 1u << 2,
    REG_TX    = 1u << 3,
    REG_RX    = 1u << 4
};

static void print_binary8(uint8_t value)
{
    for (int bit = 7; bit >= 0; --bit) {
        putchar((value & (1u << bit)) ? '1' : '0');
    }
}

static void show(const char *what, uint8_t reg)
{
    printf("%-22s dec=%3u hex=0x%02X bin=", what, (unsigned)reg, (unsigned)reg);
    print_binary8(reg);
    putchar('\n');
}

int main(void)
{
    uint8_t reg = 0;
    show("start", reg);

    reg |= REG_POWER;                    show("set POWER", reg);
    reg |= REG_READY | REG_TX;           show("set READY|TX", reg);
    reg &= (uint8_t)~REG_READY;          show("clear READY", reg);
    reg ^= REG_ERROR;                    show("toggle ERROR", reg);
    reg ^= REG_ERROR;                    show("toggle ERROR again", reg);

    printf("TX set?    %s\n", (reg & REG_TX) != 0 ? "yes" : "no");
    printf("RX set?    %s\n", (reg & REG_RX) != 0 ? "yes" : "no");

    unsigned n = 4;
    unsigned mask = 1u << n;             /* mask for bit n */
    reg |= (uint8_t)mask;                show("set bit 4 via 1u<<n", reg);
    return 0;
}
