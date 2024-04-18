#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "my_helpers.h"

void dump_buffer(const void *buffer, size_t size) {
    const uint8_t *p = (const uint8_t *)buffer;
    size_t i, j;

    for (i = 0; i < size; i += 16) {
        printf("%08x:", (unsigned int)i);
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                printf(" %02x", p[i + j]);
            } else {
                printf("   ");
            }
        }
        printf("  ");
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                printf("%c", isprint(p[i + j]) ? p[i + j] : '.');
            }
        }
        printf("\r\n");
    }
}