#ifndef __DECIMAL_H__
#define __DECIMAL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t atomic_div(uint64_t *dividend, uint64_t divisor);
uint64_t is_match(uint64_t value, uint64_t array[], uint64_t array_len);
uint64_t calculate_decimal(unsigned long long numerator, unsigned long long denominator, char **result);

#endif
