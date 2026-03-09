#include <iostream>
#include <vector>
#include <limits>
#include "Shamir.h"

// Helper function to check if a number is actually prime
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
    long long secret, prime;
    int n, k;

    std::cout << "=== Shamir's Secret Sharing ===\n";
    
    // 1. Get Secret (Must be positive)
    do {
        std::cout << "Enter a positive secret (integer): ";
        std::cin >> secret;
    } while (secret < 0);

    // 2. Get Total Shares (Must be > 0)
    do {
        std::cout << "Enter total number of shares to generate (n): ";
        std::cin >> n;
    } while (n <= 0);

    // 3. Get Threshold (Must be > 0 and <= n)
    do {
        std::cout << "Enter threshold of shares needed to reconstruct (k): ";
        std::cin >> k;
        if (k > n) std::cout << "Error: Threshold (k) cannot be greater than total shares (n).\n";
        if (k <= 0) std::cout << "Error: Threshold must be at least 1.\n";
    } while (k > n || k <= 0);

    // 4. Get Prime (Must be prime, > secret, and > n)
    while (true) {
        std::cout << "Enter a prime number strictly larger than the secret AND larger than 'n': ";
        std::cin >> prime;

        if (prime <= secret || prime <= n) {
            std::cout << "Error: Prime must be larger than both the secret (" << secret << ") and 'n' (" << n << ").\n";
        } else if (!is_prime(prime)) {
            std::cout << "Error: " << prime << " is not a prime number. Try again.\n";
        } else {
            break; // Input is valid
        }
    }

    std::cout << "\n--- Splitting the Secret ---\n";
    std::vector<Share> all_shares = split_secret(secret, n, k, prime, true);
    
    std::cout << "\nGenerated Shares available for distribution:\n";
    for (const auto& share : all_shares) {
        std::cout << "Share " << share.x << ": " << share.y << "\n";
    }

    std::cout << "\n--- Reconstructing the Secret ---\n";
    std::cout << "Enter " << k << " shares to reconstruct the secret.\n";
    
    std::vector<Share> input_shares;
    for (int i = 0; i < k; ++i) {
        Share s;
        std::cout << "Enter x for share " << (i + 1) << ": ";
        std::cin >> s.x;
        std::cout << "Enter y for share " << (i + 1) << ": ";
        std::cin >> s.y;
        input_shares.push_back(s);
    }

    long long reconstructed_secret = reconstruct_secret(input_shares, prime, true);
    
    std::cout << "====================================\n";
    std::cout << "RESULT: The reconstructed secret is: " << reconstructed_secret << "\n";
    std::cout << "====================================\n";
//10, 5, 3, 17
    return 0;
}