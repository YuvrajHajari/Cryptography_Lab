#ifndef ELGAMAL_H
#define ELGAMAL_H

struct PublicKey {
    long long p; // Prime number
    long long g; // Generator (primitive root)
    long long h; // Public key component (h = g^x mod p)
};

struct PrivateKey {
    long long p; // Prime number
    long long x; // The actual secret key
};

struct Ciphertext {
    long long c1;
    long long c2;
};

// Generates the public/private key pair
void generate_keys(long long p, long long g, long long x, PublicKey& pub_key, PrivateKey& priv_key, bool verbose = false);

// Encrypts a plaintext message
Ciphertext encrypt(long long m, const PublicKey& pub_key, bool verbose = false);

// Decrypts a ciphertext back to plaintext
long long decrypt(const Ciphertext& ct, const PrivateKey& priv_key, bool verbose = false);

// Homomorphically multiplies two ciphertexts
Ciphertext homomorphic_multiply(const Ciphertext& ct1, const Ciphertext& ct2, long long p, bool verbose = false);

#endif // ELGAMAL_H