#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdexcept>

namespace CryptoMath {
    // Standard mathematical modulo that handles negative numbers correctly in C++
    long long mod(long long a, long long m);

    // Modular exponentiation: (base^exp) % mod
    long long modExp(long long base, long long exp, long long m);

    // Extended Euclidean Algorithm
    long long extendedGCD(long long a, long long b, long long &x, long long &y);

    // Modular Inverse: returns a^-1 mod m
    long long modInverse(long long a, long long m);

    // Basic GCD
    long long gcd(long long a, long long b);
}

#endif