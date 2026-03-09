#include "ElGamal.h"
#include <iostream>
#include <random>
#include <stdexcept>

// Helper: Fast Modular Exponentiation (base^exp % mod)
long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

// Helper: Extended Euclidean Algorithm
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

// Helper: Modular Inverse
long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, x, y);
    if (g != 1) throw std::invalid_argument("Modular inverse does not exist.");
    return (x % m + m) % m;
}

void generate_keys(long long p, long long g, long long x, PublicKey& pub_key, PrivateKey& priv_key, bool verbose) {
    pub_key.p = p;
    pub_key.g = g;
    pub_key.h = mod_exp(g, x, p);
    
    priv_key.p = p;
    priv_key.x = x;

    if (verbose) {
        std::cout << "\n[VERBOSE] --- Key Generation ---\n";
        std::cout << "[VERBOSE] Calculated Public Key Component h = g^x mod p \n";
        std::cout << "[VERBOSE] h = " << g << "^" << x << " mod " << p << " = " << pub_key.h << "\n";
    }
}

Ciphertext encrypt(long long m, const PublicKey& pub_key, bool verbose) {
    // Generate random ephemeral key 'y'
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dist(1, pub_key.p - 2);
    long long y = dist(gen);

    Ciphertext ct;
    ct.c1 = mod_exp(pub_key.g, y, pub_key.p);
    
    // c2 = m * (h^y) mod p
    long long s = mod_exp(pub_key.h, y, pub_key.p);
    ct.c2 = (m * s) % pub_key.p;

    if (verbose) {
        std::cout << "\n[VERBOSE] --- Encryption for message: " << m << " ---\n";
        std::cout << "[VERBOSE] Random ephemeral key chosen: y = " << y << "\n";
        std::cout << "[VERBOSE] Shared secret: s = h^y mod p = " << s << "\n";
        std::cout << "[VERBOSE] Ciphertext c1 = g^y mod p = " << ct.c1 << "\n";
        std::cout << "[VERBOSE] Ciphertext c2 = m * s mod p = " << ct.c2 << "\n";
    }

    return ct;
}

long long decrypt(const Ciphertext& ct, const PrivateKey& priv_key, bool verbose) {
    // Shared secret s = c1^x mod p
    long long s = mod_exp(ct.c1, priv_key.x, priv_key.p);
    long long s_inv = mod_inverse(s, priv_key.p);
    
    // m = c2 * s^-1 mod p
    long long m = (ct.c2 * s_inv) % priv_key.p;

    if (verbose) {
        std::cout << "\n[VERBOSE] --- Decryption ---\n";
        std::cout << "[VERBOSE] Recovered shared secret: s = c1^x mod p = " << s << "\n";
        std::cout << "[VERBOSE] Modular inverse of s: s^-1 = " << s_inv << "\n";
        std::cout << "[VERBOSE] Recovered message: m = c2 * s^-1 mod p = " << m << "\n";
    }

    return m;
}

Ciphertext homomorphic_multiply(const Ciphertext& ct1, const Ciphertext& ct2, long long p, bool verbose) {
    Ciphertext result;
    result.c1 = (ct1.c1 * ct2.c1) % p;
    result.c2 = (ct1.c2 * ct2.c2) % p;

    if (verbose) {
        std::cout << "\n[VERBOSE] --- Homomorphic Multiplication ---\n";
        std::cout << "[VERBOSE] Multiplying c1 components: (" << ct1.c1 << " * " << ct2.c1 << ") mod " << p << " = " << result.c1 << "\n";
        std::cout << "[VERBOSE] Multiplying c2 components: (" << ct1.c2 << " * " << ct2.c2 << ") mod " << p << " = " << result.c2 << "\n";
    }

    return result;
}