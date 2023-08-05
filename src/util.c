#include "../include/util.h"
#include <inttypes.h>

uint8_t get_bit(uint8_t byte, int bit) {
    return (byte >> bit) & 1;
}