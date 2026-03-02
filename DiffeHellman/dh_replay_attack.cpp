#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

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

// Simple XOR Encryption/Decryption using the DH Secret Key
long long int encryptDecrypt(long long int data, long long int key) {
    return data ^ key; 
}

int main() {
    long long int P, G, a, b;
    long long int sharedSecret;
    
    cout << "=====================================================" << endl;
    cout << " INTERACTIVE Diffie-Hellman REPLAY ATTACK Simulation " << endl;
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

    // 3. Establish the Secure Connection
    long long int publicAlice = power(G, a, P);
    long long int publicBob = power(G, b, P);
    sharedSecret = power(publicBob, a, P); // Both calculate this independently
    
    cout << "\n-----------------------------------------------------" << endl;
    cout << "[SYSTEM] Keys exchanged securely." << endl;
    cout << "[SYSTEM] The Shared Secret is: " << sharedSecret << endl;
    cout << "-----------------------------------------------------" << endl;

    // --- PHASE 1: THE VULNERABILITY ---
    cout << "\n=====================================================" << endl;
    cout << " PHASE 1: Standard Encryption (VULNERABLE TO REPLAY) " << endl;
    cout << "=====================================================" << endl;
    
    long long int originalMessage;
    cout << "Alice: Enter an amount to transfer to Bob ($): ";
    originalMessage = getSafeInput();

    long long int interceptedCiphertext = encryptDecrypt(originalMessage, sharedSecret);
    cout << "\n>> Alice encrypts and sends packet: [" << interceptedCiphertext << "]" << endl;
    cout << "   [!] MALLORY RECORDS THE ENCRYPTED PACKET IN TRANSIT." << endl;
    
    long long int bobDecrypted = encryptDecrypt(interceptedCiphertext, sharedSecret);
    cout << "\n<< Bob receives packet." << endl;
    cout << "<< Bob decrypts successfully. ACTION: Transfer $" << bobDecrypted << " to Bob." << endl;

    cout << "\n--- Press Enter to execute Mallory's Replay Attack ---";
    cin.ignore(); // Clear buffer
    cin.get();    // Wait for user to press Enter

    cout << ">> Mallory REPLAYS the exact same intercepted packet: [" << interceptedCiphertext << "]" << endl;
    long long int bobDecryptedReplay = encryptDecrypt(interceptedCiphertext, sharedSecret);
    cout << "<< Bob receives packet." << endl;
    cout << "<< Bob decrypts successfully. ACTION: Transfer $" << bobDecryptedReplay << " to Bob." << endl;
    cout << "\n[VULNERABILITY EXPLOITED]: Bob executed the transaction a second time blindly!\n";


    // --- PHASE 2: THE DEFENSE ---
    cout << "\n=====================================================" << endl;
    cout << " PHASE 2: Defense Mode (USING NONCES AND TRACKING) " << endl;
    cout << "=====================================================" << endl;
    
    vector<long long int> bobsLedger; // Bob's database of used Nonces
    long long int newMessage, aliceNonce;
    
    cout << "Alice: Enter a NEW amount to transfer to Bob ($): ";
    newMessage = getSafeInput();
    cout << "Alice: Enter a random Nonce (e.g., a 5-digit number): ";
    aliceNonce = getSafeInput();

    long long int encryptedMessage2 = encryptDecrypt(newMessage, sharedSecret);
    long long int encryptedNonce = encryptDecrypt(aliceNonce, sharedSecret);
    
    cout << "\n>> Alice encrypts and sends packet: [Msg: " << encryptedMessage2 << ", Nonce: " << encryptedNonce << "]" << endl;
    cout << "   [!] MALLORY RECORDS THE NEW PACKETS IN TRANSIT." << endl;

    // Bob processes the legitimate message
    cout << "\n<< Bob receives packet." << endl;
    long long int decryptedMsg2 = encryptDecrypt(encryptedMessage2, sharedSecret);
    long long int decryptedNonce = encryptDecrypt(encryptedNonce, sharedSecret);
    
    cout << "<< Bob decrypts Nonce: " << decryptedNonce << endl;
    
    // Check if nonce exists in ledger
    if (find(bobsLedger.begin(), bobsLedger.end(), decryptedNonce) != bobsLedger.end()) {
        cout << "<< [SECURITY ALERT] Nonce used before! Dropping packet." << endl;
    } else {
        cout << "<< Nonce is new. Saving to ledger..." << endl;
        bobsLedger.push_back(decryptedNonce); 
        cout << "<< Bob decrypts successfully. ACTION: Transfer $" << decryptedMsg2 << " to Bob." << endl;
    }

    cout << "\n--- Press Enter to execute Mallory's Replay Attack ---";
    cin.ignore(); // Clear buffer
    cin.get();    // Wait for user to press Enter

    cout << ">> Mallory REPLAYS the exact same intercepted packet: [Msg: " << encryptedMessage2 << ", Nonce: " << encryptedNonce << "]" << endl;
    
    // Bob receives the replay attack from Mallory
    cout << "\n<< Bob receives packet." << endl;
    long long int replayMsg2 = encryptDecrypt(encryptedMessage2, sharedSecret);
    long long int replayNonce = encryptDecrypt(encryptedNonce, sharedSecret);
    
    cout << "<< Bob decrypts Nonce: " << replayNonce << endl;
    
    if (find(bobsLedger.begin(), bobsLedger.end(), replayNonce) != bobsLedger.end()) {
        cout << "<< [SECURITY ALERT] NONCE (" << replayNonce << ") ALREADY EXISTS IN LEDGER!" << endl;
        cout << "<< [SYSTEM LOG] REPLAY ATTACK DETECTED. CONNECTION TERMINATED." << endl;
        cout << "<< ACTION: Packet dropped. No money transferred." << endl;
    } else {
        // Won't hit this block in this scenario
        bobsLedger.push_back(replayNonce); 
        cout << "<< Bob decrypts successfully. ACTION: Transfer $" << replayMsg2 << " to Bob." << endl;
    }

    cout << "\n=====================================================" << endl;
    cout << "             DEFENSE SIMULATION COMPLETE             " << endl;
    cout << "=====================================================" << endl;

    return 0;
}