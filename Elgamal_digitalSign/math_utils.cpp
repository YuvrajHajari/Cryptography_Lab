#include "math_utils.h"

namespace CryptoMath {

    long long mod(long long a, long long m) {
        return (a % m + m) % m;
    }

    long long modExp(long long base, long long exp, long long m) {
        long long result = 1;
        base = mod(base, m);
        while (exp > 0) {
            if (exp % 2 == 1)
                result = mod(result * base, m);
            base = mod(base * base, m);
            exp /= 2;
        }
        return result;
    }

    long long extendedGCD(long long a, long long b, long long &x, long long &y) {
        if (a == 0) {
            x = 0;
            y = 1;
            return b;
        }
        long long x1, y1;
        long long gcd = extendedGCD(b % a, a, x1, y1);
        x = y1 - (b / a) * x1;
        y = x1;
        return gcd;
    }

    long long modInverse(long long a, long long m) {
        long long x, y;
        long long g = extendedGCD(a, m, x, y);
        if (g != 1) {
            throw std::invalid_argument("Modular inverse does not exist.");
        }
        return mod(x, m);
    }

    long long gcd(long long a, long long b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }
}