#include "decimal.h"

void print_help()
{
        printf(
                "Usage: decimal NUMERATOR DENOMINATOR\n\n"
                "Limit: <to be calculated>\n"
        );
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
        numerator = atoll(argv[1]);
        denominator = atoll(argv[2]);

        pattern_len = calculate_decimal(numerator,denominator, &decimal_number);
        if (pattern_len == 0) {
                printf("The rational of the fraction %llu/%llu is a terminating rational number:\n"
                       "%s\n",numerator, denominator,decimal_number);
        } else {
                printf("The rational of the fraction %llu/%llu is a repeating rational number:\n"
                       "%s\n",numerator, denominator,decimal_number);
        }
        return 0;
}
