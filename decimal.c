#include "decimal.h"

uint64_t calculate_decimal(unsigned long long numerator, unsigned long long denominator, char **result)
/* Calculates the rational number for the specified fraction, puts result in '**result' (it can be uninitialized)
 * Return: repeating pattern length (zero if the number is a terminating rational number)
 * */
{
        uint64_t *dividends=NULL;
        uint64_t i=0;
        uint64_t remainder;
        uint64_t tmp;
        uint64_t integer_part;
        uint64_t dividends_len=1;
        char *digits=NULL, *tmp_str=NULL;
        int is_terminating = 1;

        integer_part = numerator < denominator ? 0 : numerator / denominator;
        dividends = realloc((void*)dividends, sizeof(uint64_t));

        digits = realloc((void*)digits, 4096); //allocate a whole page for maximum efficiency
        tmp_str = realloc((void*)tmp_str, 4096);
        memset((void*)digits, 0, 4096);
        memset((void*)tmp_str, 0, 4096);
        dividends[0] = numerator - integer_part * denominator;
        while (dividends[i] != 0)	{
			/*
			 * We are implementing a long division here, terminating if a dividend is zero
			 *  (terminating rational number) or is already encountered (repeating rational number)
			 * */
			 
                tmp = atomic_div(&dividends[i], denominator);
                if (is_match(dividends[i], dividends,dividends_len)) {
                        is_terminating = 0;
                        break;
                }
                sprintf(tmp_str, "%llu", tmp);
                strcat (digits, tmp_str);
                dividends = realloc(dividends, (i+2)*sizeof(uint64_t));
                dividends_len++;
                dividends[i+1] = dividends[i] - tmp * denominator;
                i++;
        }
        if (is_terminating) {
                sprintf(tmp_str, "%llu,%s",integer_part, digits);
                *result = tmp_str;
                return 0;
        }

        sprintf(tmp_str, "%llu,(%s)",integer_part, digits);
        *result = tmp_str;
        return strlen(digits);
}

uint64_t is_match(uint64_t value, uint64_t array[], uint64_t array_len)
/*
 * Return 1 if a dividend is already encountered (we've found a repeating number)
 * */
{
        uint64_t i;
        //Search for "value" in all "array" entries except the last one.
        for (i=0; i< array_len-1; i++) {
                if (array[i] == value) return 1;
        }
        return 0;
}

uint64_t atomic_div(uint64_t *dividend, uint64_t divisor)
/*
 * Atomic divide operation in a long division
 * */
{
        if (*dividend < divisor) *dividend *=10;
        return *dividend / divisor;
}


