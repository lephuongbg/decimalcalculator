#include "decimal.h"

int64_t calculate_decimal(uint64_t numerator, uint64_t denominator, char **result)
/* Calculates the rational number for the specified fraction, puts result
 * in '**result' (it can be uninitialized)
 *
 * Return: repeating pattern length (zero if the number is a terminating
 * rational number)
 * */
{
        uint64_t *dividends=NULL; //
        uint64_t dividends_len=1;
        uint64_t i=0,tmp;
        int position=0; //where the repeating pattern begins
        uint64_t integer_part;

        char *digits=NULL, *tmp_str=NULL, *non_repeating=NULL;
        int is_terminating = 1;

        integer_part = numerator < denominator ? 0 : numerator / denominator;
        dividends = realloc((void*)dividends, sizeof(*dividends));

        digits = realloc((void*)digits, PRECISION);
        tmp_str = realloc((void*)tmp_str, PRECISION); // PRECISION is not necessary but convenient
        non_repeating = realloc((void*)non_repeating, PRECISION); // ~ again

        memset((void*)non_repeating, 0, PRECISION);
        memset((void*)digits, 0, PRECISION);
        memset((void*)tmp_str, 0, PRECISION);


        dividends[0] = numerator - integer_part * denominator;
        while (dividends[i] != 0 && strlen(digits) < PRECISION) {
                /*
                 * We are implementing a long division here, terminating if a dividend is zero
                 *  (terminating rational number) or is already encountered (repeating rational number)
                 * */

                tmp = atomic_div(&dividends[i], denominator);
                position=is_match(dividends[i], dividends,dividends_len);
                if (position > -1) {
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
                if (strlen(digits) == 0 ) digits="0"; // "3/1" = "3,0" not "3,"
                sprintf(tmp_str, "%llu,%s",integer_part, digits);
                *result = tmp_str;
                if (dividends[i] != 0) {
                        return is_terminating_func(denominator) == 0 ? OVERFLOW_REPT : OVERFLOW_TERM;
                }
                return TERMINATING;
        }

        //Split 'digits' into two parts: 'non_repeating' holding the static
        //digits and 'digits' holding the repeating pattern
        if (position != 0) {
                non_repeating = strdup(digits);
                non_repeating[position] = 0;
                digits += position;
        }
        sprintf(tmp_str, "%llu%s%s(%s)",integer_part,DECIMAL_POINT, non_repeating, digits);
        *result = tmp_str;
        return strlen(digits);
}

int64_t is_match(uint64_t value, uint64_t array[], uint64_t array_len)
/*
 * If 'value' is already encountered, return the position of that
 * instance in 'array'
 * */
{
        uint64_t i=0;
        //Search for 'value' in all 'array' entries except the last one (because 'value' *is* the last entry).
        for (i=0; i< array_len-1; i++) {
                if (array[i] == value) return i;
        }
        return -1;
}

uint64_t is_terminating_func(uint64_t denominator)
/*
 * This function is used when the rational number exceeds the precision
 * threshold and whether it is terminating cannot be determined with the
 * other method
 * */
{

        while ((denominator%2 == 0) && (denominator > 1)) denominator /= 2;
        while ((denominator%5 == 0) && (denominator > 1)) denominator /= 5;
        return denominator == 1 ? 1 : 0;
}

uint64_t atomic_div(uint64_t *dividend, uint64_t divisor)
/*
 * Atomic divide operation in a long division
 * */
{
        if (*dividend < divisor) *dividend *=10;
        return *dividend / divisor;
}


