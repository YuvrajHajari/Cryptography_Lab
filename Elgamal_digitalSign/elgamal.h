#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <utility>

struct ElGamalKeys {
    long long p; // Prime
    long long g; // Generator
    long long y; // Public Key
    long long x; // Private Key
};

struct Signature {
    long long r;
    long long s;
};

class ElGamalSignature {
public:
    // Generate public/private keys given p, g, and x
    static ElGamalKeys generateKeys(long long p, long long g, long long x);

    // Sign a message
    static Signature sign(long long message, long long k, const ElGamalKeys& keys);

    // Verify a signature
    static bool verify(long long message, const Signature& sig, const ElGamalKeys& keys);
};

#endif