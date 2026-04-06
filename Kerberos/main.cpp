#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "kerberos.h"
#include "kdc_server.h"

// Robust input function
long long readPositiveInteger(const std::string& prompt) {
    long long value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) return value;
        std::cout << "[!] Invalid input. Enter a positive integer.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  Kerberos Authentication via Diffie-Hellman Exchange   \n";
    std::cout << "========================================================\n\n";

    // Setup network entities
    long long p = readPositiveInteger("Enter a DH prime (p) [e.g., 23]: ");
    long long g = readPositiveInteger("Enter a DH generator (g) [e.g., 5]: ");
    
    KDC kdc(p, g);
    std::string clientID = "Alice";
    std::string serviceID = "FileServer";

    // --- PHASE 1: DH Key Exchange & AS Request ---
    std::cout << "\n--- PHASE 1: AS Exchange (PKINIT Simulation) ---\n";
    long long client_private_key = readPositiveInteger("Enter Client Private Key (a): ");
    
    long long client_public_key = Crypto::modExp(g, client_private_key, p);
    std::cout << "[Client] Generating DH Public Key (A): " << client_public_key << "\n";
    
    // Network Call to AS
    AS_Reply as_reply = kdc.processASRequest(clientID, client_public_key);
    
    // Client computes shared secret to decrypt the session key
    long long as_public_key = kdc.getASPublicKey();
    long long shared_secret = Crypto::modExp(as_public_key, client_private_key, p);
    std::cout << "\n[Client] Computed DH Shared Secret: " << shared_secret << "\n";
    
    std::string decrypted_tgs_key_str = Crypto::simulateDecrypt(as_reply.encryptedSessionKey, shared_secret);
    if (decrypted_tgs_key_str == "DECRYPTION_FAILED") {
        std::cerr << "[!] Client Error: Failed to decrypt AS reply. DH parameters mismatch.\n";
        return 1;
    }
    long long client_tgs_key = std::stoll(decrypted_tgs_key_str);
    std::cout << "[Client] Successfully recovered Client-TGS Session Key: " << client_tgs_key << "\n";


    // --- PHASE 2: TGS Exchange ---
    std::cout << "\n--- PHASE 2: Ticket Granting Server (TGS) Exchange ---\n";
    TGS_Request tgs_req;
    tgs_req.encryptedTGT = as_reply.encryptedTGT; // Forwarding the opaque ticket
    tgs_req.serviceID = serviceID;
    
    // Create and encrypt Authenticator
    std::string authenticator = clientID + "|" + std::to_string(time(nullptr));
    tgs_req.encryptedAuthenticator = Crypto::simulateEncrypt(authenticator, client_tgs_key);
    
    // Network Call to TGS
    TGS_Reply tgs_reply = kdc.processTGSRequest(tgs_req);
    
    // Client decrypts new session key
    long long client_service_key = std::stoll(Crypto::simulateDecrypt(tgs_reply.encryptedServiceSessionKey, client_tgs_key));
    std::cout << "[Client] Successfully recovered Client-Service Session Key: " << client_service_key << "\n";


    // --- PHASE 3: Service Access & Edge Case Testing ---
    std::cout << "\n--- PHASE 3: Service Access ---\n";
    Service_Request srv_req;
    srv_req.encryptedServiceTicket = tgs_reply.encryptedServiceTicket;
    
    // Create final authenticator
    std::string final_authenticator = clientID + "|" + std::to_string(time(nullptr));
    srv_req.encryptedAuthenticator = Crypto::simulateEncrypt(final_authenticator, client_service_key);

    std::cout << "[1] Testing Standard Authentication Flow...\n";
    bool success = kdc.verifyServiceRequest(srv_req);
    if(success) std::cout << "    [Result] Login Successful!\n";

    std::cout << "\n[2] Testing Edge Case: Replay Attack (Interceptor resends ticket later)...\n";
    std::cout << "    Simulating a 10-second network delay...\n";
// std::this_thread::sleep_for(std::chrono::seconds(2)); <-- Commented out
    bool replay_success = kdc.verifyServiceRequest(srv_req, 10);
    if(!replay_success) std::cout << "    [Result] Login Rejected as expected!\n";

    std::cout << "\n========================================================\n";
    return 0;
}