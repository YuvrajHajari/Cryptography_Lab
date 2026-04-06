#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>
#include <iostream>

namespace Crypto {
    // Modular exponentiation: (base^exp) % mod
    long long modExp(long long base, long long exp, long long mod) {
        long long result = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    // Simulated Encryption: In a real environment, this would be AES.
    // For lab visibility, we format it as a tagged string to trace the flow.
    std::string simulateEncrypt(const std::string& data, long long key) {
        return "{ENC_DATA: [" + data + "] using KEY: " + std::to_string(key) + "}";
    }

    // Basic extraction to simulate decryption (for demonstration purposes)
    std::string simulateDecrypt(const std::string& cipher, long long key) {
        std::string keyTag = "KEY: " + std::to_string(key) + "}";
        if (cipher.find(keyTag) != std::string::npos) {
            size_t start = cipher.find("[") + 1;
            size_t end = cipher.find("]");
            return cipher.substr(start, end - start);
        }
        return "DECRYPTION_FAILED";
    }
}

#endif