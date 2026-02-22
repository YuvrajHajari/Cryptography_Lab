#include <iostream>
#include <string>
#include <vector>
#include "rsa_functions.h"

using namespace std;

// Function to check if a number is prime
bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

// Function to get a valid prime from the user
long long getPrimeInput(string name) {
    long long val;
    while (true) {
        cout << "Enter prime number " << name << ": ";
        cin >> val;
        if (isPrime(val)) {
            return val;
        } else {
            cout << "Error: " << val << " is not a prime number. Please try again." << endl;
        }
    }
}

int main() {
    cout << "--- RSA Key Generation (Prime Validation Active) ---" << endl;

    // 1. Key Generation with Validation
    long long p = getPrimeInput("p");
    long long q = getPrimeInput("q");

    if (p == q) {
        cout << "Warning: p and q should ideally be different. Proceeding anyway..." << endl;
    }

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    // Choose e such that 1 < e < phi and gcd(e, phi) = 1
    long long e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1) break;
        e++;
    }

    // Calculate private key d
    long long d = modInverse(e, phi);

    cout << "\n-----------------------------------" << endl;
    cout << "Public Key (e, n): (" << e << ", " << n << ")" << endl;
    cout << "Private Key (d, n): (" << d << ", " << n << ")" << endl;
    cout << "-----------------------------------\n" << endl;

    // 2. Encryption
    string message;
    cout << "Enter message to encrypt: ";
    cin.ignore();
    getline(cin, message);

    vector<long long> ciphertext;
    for (char c : message) {
        // Encrypt: C = M^e mod n
        ciphertext.push_back(power(c, e, n));
    }

    cout << "\nCiphertext (Numeric): ";
    for (long long v : ciphertext) cout << v << " ";
    cout << endl;

    // 3. Decryption
    string decrypted = "";
    for (long long v : ciphertext) {
        // Decrypt: M = C^d mod n
        decrypted += (char)power(v, d, n);
    }

    cout << "Decrypted Message: " << decrypted << endl;

    return 0;
}