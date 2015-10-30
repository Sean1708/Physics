#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Allocate 150 characters for all strings.
static const int LENGTH = 150;
// These strings have about 100 decimal places.
static const char* ONE_NINTH = "0.1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
static const char* TEN_MILLION_NINTHS = "1111111.1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
static const char* TEN_BILLION_NINTHS = "1111111111.1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";


void float_precision(void);
void double_precision(void);
int number_correct_digits_float(const char* result, const char* reference);

int main(int argc, char* argv[]) {
    float_precision();
    double_precision();

    return 0;
}

void float_precision() {
    float at_zero = (float)1/(float)9;
    // calloc sets all bytes to '\0'.
    char* str_at_zero = calloc(LENGTH, sizeof(char));
    // Convert float to string with 100 decimal places.
    snprintf(str_at_zero, LENGTH, "%0.100f", at_zero);
    printf(
        "Near zero, floats have about %d digits of precision.\n",
        number_correct_digits(str_at_zero, ONE_NINTH),
    );
    free(str_at_zero);

    // Ten million divided by nine.
    float at_million = (float)10000000/(float)9;
    char* str_at_million = calloc(LENGTH, sizeof(char));
    snprintf(str_at_million, LENGTH, "%0.100f", at_million);
    printf("Near one million, floats have about %d digits of precision.\n", number_correct_digits(str_at_million, TEN_MILLION_NINTHS));
    free(str_at_million);

    // Ten billion divided by nine.
    float at_billion = (float)10000000000/(float)9;
    char* str_at_billion = calloc(LENGTH, sizeof(char));
    snprintf(str_at_billion, LENGTH, "%0.100f", at_billion);
    printf("Near one billion, floats have about %d digits of precision.\n", number_correct_digits(str_at_billion, TEN_BILLION_NINTHS));
    free(str_at_billion);
}

void double_precision() {
    double at_zero = (double)1/(double)9;
    char* str_at_zero = calloc(LENGTH, sizeof(char));
    snprintf(str_at_zero, LENGTH, "%0.100lf", at_zero);
    printf("Near zero, doubles have about %d digits of precision.\n", number_correct_digits(str_at_zero, ONE_NINTH));
    free(str_at_zero);

    // Ten million divided by nine.
    double at_million = (double)10000000/(double)9;
    char* str_at_million = calloc(LENGTH, sizeof(char));
    snprintf(str_at_million, LENGTH, "%0.100lf", at_million);
    printf("Near one million, doubles have about %d digits of precision.\n", number_correct_digits(str_at_million, TEN_MILLION_NINTHS));
    free(str_at_million);

    // Ten billion divided by nine.
    double at_billion = (double)10000000000/(double)9;
    char* str_at_billion = calloc(LENGTH, sizeof(char));
    snprintf(str_at_billion, LENGTH, "%0.100lf", at_billion);
    printf("Near one billion, doubles have about %d digits of precision.\n", number_correct_digits(str_at_billion, TEN_BILLION_NINTHS));
    free(str_at_billion);
}

// Note that this functions counts the number of correct digits in a float, NOT
// the number of correct decimal places.
int number_correct_digits_float(const char* result, const char* reference) {
    // Avoid out-of-bounds access.
    int res_len = strlen(result);
    int ref_len = strlen(reference);
    int min_length = res_len < ref_len ? res_len : ref_len;
    // Loop until min_length-1 to ignore the nul-character.
    int i = 0;
    for (; i < min_length; i++) {
        // Break at first incorrect character, since indexing starts at zero i will be the number of correct digits plus one for the decimal point.
        if (result[i] != reference[i]) { break; }
    }

    // Account for the decimal point.
    return i-1;
}
