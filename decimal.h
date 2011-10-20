#ifndef __DECIMAL_H__
#define __DECIMAL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define DECIMAL_POINT ","
#define OVERFLOW_TERM -1
#define OVERFLOW_REPT -2
#define TERMINATING 0
#define PRECISION 4096

uint64_t is_terminating_func(uint64_t denominator);
uint64_t atomic_div(uint64_t *dividend, uint64_t divisor);
int64_t is_match(uint64_t value, uint64_t array[], uint64_t array_len);
int64_t calculate_decimal(uint64_t numerator, uint64_t denominator, char **result);

#endif
