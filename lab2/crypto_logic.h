#include <stdio.h>

// Modular Exponentiation: Base^Exp % Mod
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// GCD Computation: Used to verify co-primes
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Euler Totient Function: To calculate cycle length
int euler_totient(int n) {
    int result = 0;
    for (int i = 1; i < n; i++) {
        if (gcd(i, n) == 1) result++;
    }
    return result;
}