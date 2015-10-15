/*
 * Acme's Division as a Service
 * ============================
 * Our competitors have long offered Addition as a Service and there have been
 * rumours they will soon offer Multiplication as a Service. We will beat them
 * all by being first to market with Division as a Service. Our unparalleled
 * ability to deliver the quotient of two numbers will yield untold profits.
 *
 *   -- The Boss
 */

#include <stdio.h>

int main(int argc, char **argv) {
    long long numerator, denominator;

    printf("numerator: ");
    scanf("%lld", &numerator);

    printf("denominator: ");
    scanf("%lld", &denominator);

    long long result = numerator / denominator;

    printf("result: %lld\n", result);

    return 0;
}

