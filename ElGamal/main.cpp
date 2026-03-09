#include <iostream>
#include "ElGamal.h"

// Helper function to check prime
bool is_prime(long long num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (long long i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

int main() {
    long long p, g, x, m1, m2;

    std::cout << "=== Homomorphic ElGamal Encryption ===\n";

    // 1. Setup Phase
    while (true) {
        std::cout << "Enter a large prime number (p): ";
        std::cin >> p;
        if (is_prime(p)) break;
        std::cout << "Error: Not a prime number.\n";
    }

    std::cout << "Enter a generator (g) strictly less than p: ";
    std::cin >> g;

    std::cout << "Enter your private key (x) strictly less than p-1: ";
    std::cin >> x;

    PublicKey pub_key;
    PrivateKey priv_key;
    // Generate keys with verbose output enabled
    generate_keys(p, g, x, pub_key, priv_key, true);

    std::cout << "\nPublic Key: (p=" << pub_key.p << ", g=" << pub_key.g << ", h=" << pub_key.h << ")\n";
    std::cout << "Private Key: (x=" << priv_key.x << ")\n";

    // 2. Input Messages
    std::cout << "\nEnter first message to encrypt (m1 < p): ";
    std::cin >> m1;
    std::cout << "Enter second message to encrypt (m2 < p): ";
    std::cin >> m2;

    if (m1 >= p || m2 >= p) {
        std::cout << "Error: Messages must be smaller than the prime modulus (p).\n";
        return 1;
    }

    // 3. Encryption
    Ciphertext ct1 = encrypt(m1, pub_key, true);
    std::cout << "Encrypted m1 -> c1: " << ct1.c1 << ", c2: " << ct1.c2 << "\n";

    Ciphertext ct2 = encrypt(m2, pub_key, true);
    std::cout << "Encrypted m2 -> c1: " << ct2.c1 << ", c2: " << ct2.c2 << "\n";

    // 4. Homomorphic Operation
    std::cout << "\n--- Performing Homomorphic Multiplication on Ciphertexts ---\n";
    Ciphertext ct_mult = homomorphic_multiply(ct1, ct2, p, true);
    std::cout << "Resulting Ciphertext -> c1: " << ct_mult.c1 << ", c2: " << ct_mult.c2 << "\n";

    // 5. Decryption and Proof
    std::cout << "\n--- Decrypting the Resulting Ciphertext ---\n";
    long long decrypted_result = decrypt(ct_mult, priv_key, true);

    std::cout << "\n====================================\n";
    std::cout << "Original m1: " << m1 << "\n";
    std::cout << "Original m2: " << m2 << "\n";
    std::cout << "Expected Product (m1 * m2) mod p: " << (m1 * m2) % p << "\n";
    std::cout << "Decrypted Result: " << decrypted_result << "\n";
    
    if (decrypted_result == (m1 * m2) % p) {
        std::cout << "\nSUCCESS: The homomorphic property holds true!\n";
    } else {
        std::cout << "\nFAILURE: Something went wrong.\n";
    }
    std::cout << "====================================\n";

    return 0;
}