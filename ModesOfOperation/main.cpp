#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

// --- DUMMY BLOCK CIPHER (XOR Engine) ---
char encryptBlock(char data, char key) { return data ^ key; }
char decryptBlock(char data, char key) { return data ^ key; }

// --- ENCRYPTION MODES ---

string modeECB(string p, char k) {
    string c = "";
    for (char b : p) c += encryptBlock(b, k);
    return c;
}


string modeCBC(string p, char k, char iv) {
    string c = "";
    char prev = iv;
    for (char b : p) {
        char encrypted = encryptBlock(b ^ prev, k);
        c += encrypted;
        prev = encrypted;
    }
    return c;
}


string modeCFB(string p, char k, char iv) {
    string c = "";
    char prev = iv;
    for (char b : p) {
        char keystream = encryptBlock(prev, k);
        char encrypted = b ^ keystream;
        c += encrypted;
        prev = encrypted; // Feedback is the ciphertext we just created
    }
    return c;
}


string modeOFB(string p, char k, char iv) {
    string c = "";
    char prev = iv;
    for (char b : p) {
        prev = encryptBlock(prev, k); // Feedback is the internal output
        c += (b ^ prev);
    }
    return c;
}


string modeCTR(string p, char k, char nonce) {
    string c = "";
    for (int i = 0; i < (int)p.length(); i++) {
        char keystream = encryptBlock(nonce + i, k);
        c += (p[i] ^ keystream);
    }
    return c;
}

// --- DECRYPTION MODES ---

string decECB(string c, char k) {
    string p = "";
    for (char b : c) p += decryptBlock(b, k);
    return p;
}


string decCBC(string c, char k, char iv) {
    string p = "";
    char prev = iv;
    for (char b : c) {
        char decrypted = decryptBlock(b, k);
        p += (decrypted ^ prev);
        prev = b; 
    }
    return p;
}


string decCFB(string c, char k, char iv) {
    string p = "";
    char prev = iv;
    for (char b : c) {
        char keystream = encryptBlock(prev, k); // Note: Still using encryptBlock
        p += (b ^ keystream);
        prev = b; // FEEDBACK: In decryption, we use the ciphertext byte received
    }
    return p;
}

// OFB and CTR use the exact same function for encryption and decryption
// because the keystream generation is independent of the plaintext.

// --- MAIN INTERFACE ---

void display(string mode, string cipher, string plain) {
    cout << left << setw(5) << mode << " | Hex: ";
    for (unsigned char b : cipher) printf("%02X ", b);
    cout << " | Decrypted: " << plain << endl;
}

int main() {
    string plaintext;
    char key = 0x64;
    char iv  = 0x01;

    cout << "--- Cryptography Lab: Final Corrected Modes ---" << endl;
    cout << "Enter plaintext (or 'exit'): ";
    
    while (getline(cin, plaintext) && plaintext != "exit") {
        if (plaintext.empty()) continue;

        cout << "\nResults for: " << plaintext << endl;
        cout << "--------------------------------------" << endl;

        // 1. ECB
        string cECB = modeECB(plaintext, key);
        display("ECB", cECB, decECB(cECB, key));

        // 2. CBC
        string cCBC = modeCBC(plaintext, key, iv);
        display("CBC", cCBC, decCBC(cCBC, key, iv));

        // 3. CFB
        string cCFB = modeCFB(plaintext, key, iv);
        display("CFB", cCFB, decCFB(cCFB, key, iv));

        // 4. OFB
        string cOFB = modeOFB(plaintext, key, iv);
        display("OFB", cOFB, modeOFB(cOFB, key, iv));

        // 5. CTR
        string cCTR = modeCTR(plaintext, key, iv);
        display("CTR", cCTR, modeCTR(cCTR, key, iv));

        cout << "--------------------------------------" << endl;
        cout << "\nEnter next plaintext: ";
    }

    return 0;
}