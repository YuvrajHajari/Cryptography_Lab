#include <iostream>
#include <limits> 

using namespace std;

// Function to safely get integer input and prevent infinite loops
long long int getSafeInput() {
    long long int input;
    while (!(cin >> input)) {
        cout << "  [Error] Invalid input. Please enter numbers only: ";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    }
    return input;
}

// Function to check if a number is prime
bool isPrime(long long int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Function to compute (base^exp) % mod safely
long long int power(long long int base, long long int exp, long long int mod) {
    long long int res = 1;
    base = base % mod; 
    if (base == 0) return 0; 
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1; 
        base = (base * base) % mod;
    }
    return res;
}

int main() {
    long long int P, G, a, b, m;
    long long int publicAlice, publicBob, publicMallory;
    long long int secretAlice, secretBob, secretMalloryWithAlice, secretMalloryWithBob;
    
    cout << "=====================================================" << endl;
    cout << " INTERACTIVE Diffie-Hellman MAN-IN-THE-MIDDLE Attack " << endl;
    cout << "=====================================================" << endl;

    // 1. Network setup (Public Variables)
    while (true) {
        cout << "Enter the Network Prime number (P): ";
        P = getSafeInput();
        if (P > 2 && isPrime(P)) break;
        cout << "  [Error] P must be a prime number greater than 2.\n";
    }

    while (true) {
        cout << "Enter the Network Primitive Root (G) [1 < G < " << P << "]: ";
        G = getSafeInput();
        if (G > 1 && G < P) break;
        cout << "  [Error] G must be strictly between 1 and " << P << ".\n";
    }
    
    cout << "\n[Public Data Locked] P = " << P << ", G = " << G << endl;
    cout << "-----------------------------------------------------" << endl;

    // 2. Private Keys Input
    while (true) {
        cout << "Alice: Enter your private key (a) [1 to " << P - 1 << "]: ";
        a = getSafeInput();
        if (a >= 1 && a < P) break;
        cout << "  [Error] Private key must be between 1 and " << P - 1 << ".\n";
    }
    
    while (true) {
        cout << "Bob: Enter your private key (b) [1 to " << P - 1 << "]: ";
        b = getSafeInput();
        if (b >= 1 && b < P) break;
        cout << "  [Error] Private key must be between 1 and " << P - 1 << ".\n";
    }

    while (true) {
        cout << "Mallory (Hacker): Enter your private key (m) [1 to " << P - 1 << "]: ";
        m = getSafeInput();
        if (m >= 1 && m < P) break;
        cout << "  [Error] Private key must be between 1 and " << P - 1 << ".\n";
    }

    cout << "\n-----------------------------------------------------" << endl;
    cout << "--- Generation Phase ---" << endl;
    publicAlice = power(G, a, P);
    cout << "Alice generates Public Key: " << publicAlice << endl;
    
    publicBob = power(G, b, P);
    cout << "Bob generates Public Key: " << publicBob << endl;
    
    publicMallory = power(G, m, P);
    cout << "Mallory generates Fake Public Key: " << publicMallory << endl;

    cout << "\n--- Interception Phase (The Attack) ---" << endl;
    cout << ">> Alice sends her key (" << publicAlice << ") to Bob..." << endl;
    cout << "   [!] MALLORY INTERCEPTS ALICE'S KEY." << endl;
    cout << "   [!] MALLORY SENDS HER FAKE KEY (" << publicMallory << ") TO BOB INSTEAD." << endl;
    
    cout << "\n>> Bob sends his key (" << publicBob << ") to Alice..." << endl;
    cout << "   [!] MALLORY INTERCEPTS BOB'S KEY." << endl;
    cout << "   [!] MALLORY SENDS HER FAKE KEY (" << publicMallory << ") TO ALICE INSTEAD." << endl;

    cout << "\n-----------------------------------------------------" << endl;
    cout << "--- Secret Calculation Phase ---" << endl;
    
    // Alice computes her secret using Mallory's fake public key
    secretAlice = power(publicMallory, a, P); 
    
    // Bob computes his secret using Mallory's fake public key
    secretBob = power(publicMallory, b, P); 
    
    // Mallory calculates the real secrets for both sides
    secretMalloryWithAlice = power(publicAlice, m, P);
    secretMalloryWithBob = power(publicBob, m, P);

    cout << "Alice's Calculated Secret (thinking it's Bob): " << secretAlice << endl;
    cout << "Bob's Calculated Secret (thinking it's Alice): " << secretBob << endl;
    
    cout << "\n--- Hacker's Dashboard (Mallory) ---" << endl;
    cout << "Mallory's Shared Secret with Alice: " << secretMalloryWithAlice << endl;
    cout << "Mallory's Shared Secret with Bob:   " << secretMalloryWithBob << endl;

    cout << "\n=====================================================" << endl;
    cout << "[RESULT]: Attack Successful!" << endl;
    
    if (secretAlice != secretBob) {
        cout << "Proof: Alice and Bob DO NOT share the same secret." << endl;
    }
    
    cout << "Mallory can decrypt Alice's data using Secret [" << secretMalloryWithAlice << "]," << endl;
    cout << "read/modify it, and re-encrypt it for Bob using Secret [" << secretMalloryWithBob << "]." << endl;
    cout << "=====================================================" << endl;

    return 0;
}