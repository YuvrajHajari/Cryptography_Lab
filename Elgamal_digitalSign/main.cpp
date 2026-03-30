#include <iostream>
#include <limits>
#include "elgamal.h"
#include "math_utils.h"

// Robust input function that prevents bad types (chars, negative numbers)
long long readPositiveInteger(const std::string& prompt) {
    long long value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        } else {
            std::cout << "[!] Invalid input. Please enter a positive integer.\n";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Flush buffer
        }
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  ElGamal Digital Signature Laboratory  \n";
    std::cout << "========================================\n\n";

    try {
        // 1. Key Generation Phase
        std::cout << "--- Phase 1: Key Generation ---\n";
        long long p = readPositiveInteger("Enter a prime number (p): ");
        long long g = readPositiveInteger("Enter a generator (g): ");
        
        long long x;
        while (true) {
            x = readPositiveInteger("Enter a private key (x) [1 < x < p-1]: ");
            if (x > 1 && x < p - 1) break;
            std::cout << "[!] Constraint failed: x must be between 1 and " << p - 2 << ".\n";
        }

        ElGamalKeys keys = ElGamalSignature::generateKeys(p, g, x);
        std::cout << "\n[+] Keys generated successfully!\n";
        std::cout << "    Public Key (p, g, y): (" << keys.p << ", " << keys.g << ", " << keys.y << ")\n";
        std::cout << "    Private Key (x): " << keys.x << "\n\n";

        // 2. Signing Phase
        std::cout << "--- Phase 2: Signing ---\n";
        long long m = readPositiveInteger("Enter the message as an integer (m): ");
        
        long long k;
        while (true) {
            k = readPositiveInteger("Enter ephemeral key (k) coprime to p-1: ");
            if (k > 0 && k < p - 1 && CryptoMath::gcd(k, p - 1) == 1) break;
            std::cout << "[!] Constraint failed: k must be between 1 and " << p - 2 << " AND coprime to " << p - 1 << ".\n";
        }

        Signature sig = ElGamalSignature::sign(m, k, keys);
        std::cout << "\n[+] Message signed successfully!\n";
        std::cout << "    Signature (r, s): (" << sig.r << ", " << sig.s << ")\n\n";

        // 3. Verification Phase
        std::cout << "--- Phase 3: Verification ---\n";
        std::cout << "Verifying signature...\n";
        bool isValid = ElGamalSignature::verify(m, sig, keys);

        if (isValid) {
            std::cout << "[*] RESULT: The signature is VALID. The math checks out.\n";
        } else {
            std::cout << "[*] RESULT: The signature is INVALID. Authentication failed.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL ERROR] " << e.what() << "\n";
        return 1;
    }

    return 0;
}