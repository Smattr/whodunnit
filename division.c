#include <stdbool.h>

extern void debug_msg(int,void*,void*);

long long divide(long long n, long long d) {
    bool negate = false;
    long long r;

    if (d == 0)
        debug_msg(0, 0, 0);

    if (n < 0) {
        n = -n;
        r = divide(n, d);
        negate = !negate;
    } else if (d < 0) {
        d = -d;
        r = divide(n, d);
        negate = !negate;
    } else {
        r = 0;
        while (n >= d) {
            n -= d;
            r++;
        }
    }

    return (negate? -1 : 1) * r;
}

long long __divdi3(long long n, long long d) __attribute__((weak, alias("divide")));
