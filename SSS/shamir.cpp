#include "Shamir.h"
#include <iostream>
#include <random>
#include <stdexcept>

long long extended_gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    long long x1, y1;
    long long d = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, x, y);
    if (g != 1) {
        throw std::invalid_argument("Modular inverse does not exist. Ensure prime is valid.");
    }
    return (x % m + m) % m;
}

std::vector<Share> split_secret(long long secret, int n, int k, long long prime, bool verbose) {
    std::vector<long long> coeffs(k);
    coeffs[0] = secret; 
    
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dist(1, prime - 1);

    for (int i = 1; i < k; ++i) {
        coeffs[i] = dist(gen);
    }

    if (verbose) {
        std::cout << "\n[VERBOSE] Generated Polynomial:\n";
        std::cout << "[VERBOSE] f(x) = " << coeffs[0];
        for (int i = 1; i < k; ++i) {
            std::cout << " + " << coeffs[i] << "x^" << i;
        }
        std::cout << " (mod " << prime << ")\n\n";
    }

    std::vector<Share> shares;
    for (int i = 1; i <= n; ++i) {
        long long x = i;
        long long y = 0;
        long long x_pow = 1;
        
        for (int j = 0; j < k; ++j) {
            long long term = (coeffs[j] * x_pow) % prime;
            y = (y + term) % prime;
            x_pow = (x_pow * x) % prime;
        }
        shares.push_back({x, y});
        
        if (verbose) {
            std::cout << "[VERBOSE] Evaluating at x = " << x << " -> y = " << y << "\n";
        }
    }
    return shares;
}

long long reconstruct_secret(const std::vector<Share>& shares, long long prime, bool verbose) {
    long long secret = 0;
    int k = shares.size();

    if (verbose) std::cout << "\n[VERBOSE] Starting Lagrange Interpolation...\n";

    for (int i = 0; i < k; ++i) {
        long long num = 1;
        long long den = 1;
        
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                // Calculate numerator and denominator for the basis polynomial
                num = (num * (-shares[j].x)) % prime;
                den = (den * (shares[i].x - shares[j].x)) % prime;
            }
        }
        
        // Handle negative modulo results in C++
        num = (num % prime + prime) % prime;
        den = (den % prime + prime) % prime;

        long long inv_den = mod_inverse(den, prime);
        long long term = (shares[i].y * num) % prime;
        term = (term * inv_den) % prime;
        
        secret = (secret + term) % prime;

        if (verbose) {
            std::cout << "[VERBOSE] Share (" << shares[i].x << ", " << shares[i].y << ") processing:\n";
            std::cout << "          -> Basis Numerator: " << num << "\n";
            std::cout << "          -> Basis Denominator: " << den << " (Inverse: " << inv_den << ")\n";
            std::cout << "          -> Calculated Term Contribution: " << term << "\n";
        }
    }
    
    long long final_secret = (secret % prime + prime) % prime;
    if (verbose) {
        std::cout << "[VERBOSE] Summing contributions mod " << prime << " gives final secret: " << final_secret << "\n\n";
    }
    
    return final_secret;
}