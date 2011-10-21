#include "decimal.h"
#include <limits.h>
#include <errno.h>


uint64_t denominator_limit;
/*
 * The denominator must have at least one less digit than ULLONG_MAX
 * because otherwise we would have to multiply the dividend by 10 manifold
 * to be greater than the denominator and have it greater than ULLONG_MAX
 * So if ULLONG_MAX=18446744073709551615 (20 digits), denominator_limit should be
 * 9999999999999999999 (19 digits)
 * */

uint64_t get_denominator_limit()
/*
 * On my machine I have ULLONG_MAX=18446744073709551615 but in the
 * future it may not be so anymore with the advent of 128-bit-or-more machines *
 * */
{
        uint64_t result=0, tmp = ULLONG_MAX / 10;
        while (tmp > 0) {
                result = result * 10 + 9;
                tmp /=10;
        }
        return result;
}

void print_help()
{
        printf(
                "Usage: decimal NUMERATOR DENOMINATOR\n\n"
                "NUMERATOR and DENOMINATOR must be unsigned integers in which:\n"
                "NUMERATOR   <= %llu and\n"
                "DENOMINATOR <= %llu\n", ULLONG_MAX, denominator_limit
        );
}

int parse_number(char *string, uint64_t *number)
{
        char *end;
        *number = strtoull(string, &end, 10);
        if ((*number == 0 && end == string) //not a valid number
            || (*number == ULLONG_MAX && errno == ERANGE) //out of range
           ) {
                print_help();
                return -1;
        }
        return 0;
}

int main(int argc, char **argv)
{
        uint64_t numerator, denominator;
        char *decimal_number = NULL;
        int pattern_len;

        if (argc != 3) {
                print_help();
                return -1;
        }

        denominator_limit = get_denominator_limit();
        if (parse_number(argv[1], &numerator) == -1 || parse_number(argv[2], &denominator) == -1 ) return -1;
        if (denominator > denominator_limit) {
                print_help();
                return -1;
        }

        pattern_len = calculate_decimal(numerator,denominator, &decimal_number);
        if (pattern_len == TERMINATING) {
                printf("The rational number of the fraction %llu/%llu is a terminating rational number:\n"
                       "%s\n",numerator, denominator,decimal_number);
        } else if (pattern_len == OVERFLOW_REPT || pattern_len == OVERFLOW_TERM) {
                printf("The rational number of the fraction %llu/%llu is a %s rational number that exceeds the arithmetic precision of %d digits:\n"
                       "%s...\n",numerator, denominator,
                       pattern_len == OVERFLOW_TERM ? "terminating" : "repeating"
                       ,PRECISION,decimal_number);
        } else {
                printf("The rational number of the fraction %llu/%llu is a repeating rational number with %d repeating digit(s):\n"
                       "%s\n",numerator, denominator,pattern_len,decimal_number);
        }
        return 0;
}
