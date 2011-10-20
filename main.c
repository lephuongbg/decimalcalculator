#include "decimal.h"
#include <limits.h>
#include <errno.h>

void print_help()
{
        printf(
                "Usage: decimal NUMERATOR DENOMINATOR\n\n"
                "Note: NUMERATOR and DENOMINATOR must be unsigned integers smaller than %llu\n", ULLONG_MAX
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
        
        if (parse_number(argv[1], &numerator) == -1 || parse_number(argv[2], &denominator) == -1 ) return -1;

        pattern_len = calculate_decimal(numerator,denominator, &decimal_number);
        if (pattern_len == TERMINATING) {
                printf("The rational number of the fraction %llu/%llu is a terminating rational number:\n"
                       "%s\n",numerator, denominator,decimal_number);
        } else if (pattern_len == OVERFLOW_REPT || pattern_len == OVERFLOW_TERM) {
                printf("The rational number of the fraction %llu/%llu is a %s rational number that exceeds the arithmetic precision of %d digits:\n"
                       "%s...\n",numerator, denominator,
                       pattern_len == OVERFLOW_REPT ? "terminating" : "repeating"
                       ,PRECISION,decimal_number);
        } else {
                printf("The rational number of the fraction %llu/%llu is a repeating rational number with %d repeating digit(s):\n"
                       "%s\n",numerator, denominator,pattern_len,decimal_number);
        }
        return 0;
}
