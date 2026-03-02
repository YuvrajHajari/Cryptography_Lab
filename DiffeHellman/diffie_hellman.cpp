#include <iostream>
#include <limits> // Required for clearing the input buffer

using namespace std;

// Function to safely get integer input and prevent infinite loops if user types letters
long long int getSafeInput() {
    long long int input;
    while (!(cin >> input)) {
        cout << "  [Error] Invalid input. Please enter numbers only: ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
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

// Function to compute (base^exp) % mod
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
    long long int P, G, x, a, y, b, ka, kb;
    
    cout << "==============================================" << endl;
    cout << "  Secure Diffie-Hellman Key Exchange Setup    " << endl;
    cout << "==============================================" << endl;

    // 1. Get a valid Prime Number (P)
    while (true) {
        cout << "Enter a positive Prime number (P): ";
        P = getSafeInput();
        if (P > 2 && isPrime(P)) {
            break;
        } else {
            cout << "  [Error] P must be a prime number greater than 2.\n";
        }
    }

    // 2. Get a valid Generator (G)
    while (true) {
        cout << "Enter a Primitive Root (G) where 1 < G < " << P << ": ";
        G = getSafeInput();
        if (G > 1 && G < P) {
            break;
        } else {
            cout << "  [Error] G must be strictly between 1 and " << P << ".\n";
        }
    }
    
    cout << "\n[Public Data Locked] P = " << P << ", G = " << G << endl;
    cout << "----------------------------------------------" << endl;

    // 3. Get Alice's Private Key (a)
    while (true) {
        cout << "Alice: Enter your private key (a) [1 to " << P - 1 << "]: ";
        a = getSafeInput();
        if (a >= 1 && a < P) break;
        cout << "  [Error] Private key must be between 1 and " << P - 1 << ".\n";
    }
    x = power(G, a, P);
    cout << "Alice: Generated Public Key (x) to send: " << x << endl;

    // 4. Get Bob's Private Key (b)
    cout << "----------------------------------------------" << endl;
    while (true) {
        cout << "Bob: Enter your private key (b) [1 to " << P - 1 << "]: ";
        b = getSafeInput();
        if (b >= 1 && b < P) break;
        cout << "  [Error] Private key must be between 1 and " << P - 1 << ".\n";
    }
    y = power(G, b, P);
    cout << "Bob: Generated Public Key (y) to send: " << y << endl;
    
    cout << "----------------------------------------------" << endl;
    cout << "--- Exchanging Public Keys (x and y) ---" << endl;
    cout << "----------------------------------------------" << endl;

    // 5. Calculate Secrets
    ka = power(y, a, P); 
    kb = power(x, b, P); 
    
    cout << "Alice's calculated Secret Key: " << ka << endl;
    cout << "Bob's calculated Secret Key:   " << kb << endl;
    
    cout << "----------------------------------------------" << endl;
    
    if(ka == kb) {
        cout << "SUCCESS: Both parties have established the same shared secret!" << endl;
    } else {
        cout << "FAILURE: The keys do not match." << endl;
    }

    return 0;
}