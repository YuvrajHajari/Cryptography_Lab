#include "elgamal.h"
#include "math_utils.h"
#include <stdexcept>
#include <iostream>

using namespace CryptoMath;

ElGamalKeys ElGamalSignature::generateKeys(long long p, long long g, long long x) {
    if (x <= 1 || x >= p - 1) {
        throw std::invalid_argument("Private key 'x' must be strictly between 1 and p-1.");
    }
    
    ElGamalKeys keys;
    keys.p = p;
    keys.g = g;
    keys.x = x;
    keys.y = modExp(g, x, p);
    
    // --- NEW: Verbose Math Output ---
    std::cout << "\n    [Math Tracker] Calculating Public Key (y)...\n";
    std::cout << "    [Math Tracker] Formula: y = g^x mod p\n";
    std::cout << "    [Math Tracker] y = " << g << "^" << x << " mod " << p << " = " << keys.y << "\n";
    
    return keys;
}

Signature ElGamalSignature::sign(long long message, long long k, const ElGamalKeys& keys) {
    if (gcd(k, keys.p - 1) != 1) {
        throw std::invalid_argument("Ephemeral key 'k' must be coprime to p-1.");
    }

    Signature sig;
    std::cout << "\n    [Math Tracker] --- Generating Signature (r, s) ---\n";
    
    // r = g^k mod p
    sig.r = modExp(keys.g, k, keys.p);
    std::cout << "    [Math Tracker] Step 1: Calculate 'r'\n";
    std::cout << "    [Math Tracker] Formula: r = g^k mod p\n";
    std::cout << "    [Math Tracker] r = " << keys.g << "^" << k << " mod " << keys.p << " = " << sig.r << "\n\n";

    // k^-1 mod (p-1)
    long long k_inv = modInverse(k, keys.p - 1);
    std::cout << "    [Math Tracker] Step 2: Calculate modular inverse of k modulo (p-1)\n";
    std::cout << "    [Math Tracker] Formula: k^-1 mod " << (keys.p - 1) << "\n";
    std::cout << "    [Math Tracker] " << k << "^-1 mod " << (keys.p - 1) << " = " << k_inv << "\n\n";

    // s = (m - x*r) * k^-1 mod (p-1)
    long long m_minus_xr = mod(message - (keys.x * sig.r), keys.p - 1);
    sig.s = mod(m_minus_xr * k_inv, keys.p - 1);
    std::cout << "    [Math Tracker] Step 3: Calculate 's'\n";
    std::cout << "    [Math Tracker] Formula: s = (m - x*r) * k^-1 mod (p-1)\n";
    std::cout << "    [Math Tracker] s = (" << message << " - " << keys.x << "*" << sig.r << ") * " << k_inv << " mod " << (keys.p - 1) << "\n";
    std::cout << "    [Math Tracker] s = " << m_minus_xr << " * " << k_inv << " mod " << (keys.p - 1) << " = " << sig.s << "\n";

    return sig;
}

bool ElGamalSignature::verify(long long message, const Signature& sig, const ElGamalKeys& keys) {
    // Constraint check: 0 < r < p
    if (sig.r <= 0 || sig.r >= keys.p) {
        std::cout << "    [Error] Signature 'r' is out of bounds (0 < r < p).\n";
        return false;
    }

    std::cout << "\n    [Math Tracker] --- Verifying Signature ---\n";

    // Left Side: g^m mod p
    long long leftSide = modExp(keys.g, message, keys.p);
    std::cout << "    [Math Tracker] Step 1: Calculate Left Side (v1)\n";
    std::cout << "    [Math Tracker] Formula: v1 = g^m mod p\n";
    std::cout << "    [Math Tracker] v1 = " << keys.g << "^" << message << " mod " << keys.p << " = " << leftSide << "\n\n";

    // Right Side: (y^r * r^s) mod p
    long long y_pow_r = modExp(keys.y, sig.r, keys.p);
    long long r_pow_s = modExp(sig.r, sig.s, keys.p);
    long long rightSide = mod(y_pow_r * r_pow_s, keys.p);
    
    std::cout << "    [Math Tracker] Step 2: Calculate Right Side (v2)\n";
    std::cout << "    [Math Tracker] Formula: v2 = (y^r * r^s) mod p\n";
    std::cout << "    [Math Tracker] y^r mod p = " << keys.y << "^" << sig.r << " mod " << keys.p << " = " << y_pow_r << "\n";
    std::cout << "    [Math Tracker] r^s mod p = " << sig.r << "^" << sig.s << " mod " << keys.p << " = " << r_pow_s << "\n";
    std::cout << "    [Math Tracker] v2 = (" << y_pow_r << " * " << r_pow_s << ") mod " << keys.p << " = " << rightSide << "\n\n";

    std::cout << "    [Math Tracker] Step 3: Final Equality Check\n";
    std::cout << "    [Math Tracker] Does " << leftSide << " == " << rightSide << "?\n";

    return leftSide == rightSide;
}