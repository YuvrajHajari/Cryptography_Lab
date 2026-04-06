#ifndef KDC_SERVER_H
#define KDC_SERVER_H

#include "kerberos.h"
#include "crypto_utils.h"

class KDC {
private:
    long long dh_prime;
    long long dh_generator;
    long long as_private_key = 15; // KDC's DH private key
    
    // Internal KDC Master Keys (Never shared)
    long long tgs_secret_key = 9999;
    long long service_secret_key = 8888;

public:
    KDC(long p, long g) : dh_prime(p), dh_generator(g) {}

    // Step 1: Authentication Server (AS) Exchange with Diffie-Hellman
    AS_Reply processASRequest(const std::string& clientID, long long client_public_key) {
        std::cout << "\n[KDC-AS] Received Authentication Request from " << clientID << ".\n";
        
        // 1. Calculate DH Shared Key
        long long shared_secret = Crypto::modExp(client_public_key, as_private_key, dh_prime);
        std::cout << "[KDC-AS] Calculated DH Shared Secret: " << shared_secret << "\n";

        // 2. Generate Client-TGS Session Key
        long long client_tgs_key = 12345; // Simulated random session key

        // 3. Create TGT (Ticket Granting Ticket)
        std::string tgtData = clientID + "|TGS|" + std::to_string(client_tgs_key);
        std::string encryptedTGT = Crypto::simulateEncrypt(tgtData, tgs_secret_key);

        // 4. Encrypt Client-TGS Session Key with the DH Shared Secret
        std::string encryptedKeyData = Crypto::simulateEncrypt(std::to_string(client_tgs_key), shared_secret);

        return {encryptedKeyData, encryptedTGT};
    }

    // Step 2: Ticket Granting Server (TGS) Exchange
    TGS_Reply processTGSRequest(const TGS_Request& req) {
        std::cout << "\n[KDC-TGS] Processing TGS Request for Service: " << req.serviceID << "\n";

        // 1. Decrypt TGT
        std::string tgtData = Crypto::simulateDecrypt(req.encryptedTGT, tgs_secret_key);
        if (tgtData == "DECRYPTION_FAILED") {
            std::cout << "[!] TGS Error: Invalid TGT.\n";
            throw std::runtime_error("TGT Decryption Failed");
        }

        // 2. Extract Client-TGS Session Key from TGT (Simplified parsing)
        long long client_tgs_key = std::stoll(tgtData.substr(tgtData.find_last_of('|') + 1));

        // 3. Decrypt and Verify Authenticator
        std::string authData = Crypto::simulateDecrypt(req.encryptedAuthenticator, client_tgs_key);
        if (authData == "DECRYPTION_FAILED") {
             std::cout << "[!] TGS Error: Authenticator verification failed.\n";
             throw std::runtime_error("Authenticator Decryption Failed");
        }
        std::cout << "[KDC-TGS] Authenticator verified successfully.\n";

        // 4. Generate Client-Service Session Key
        long long client_service_key = 67890;

        // 5. Create Service Ticket
        std::string stData = "Alice|" + req.serviceID + "|" + std::to_string(client_service_key);
        std::string encryptedST = Crypto::simulateEncrypt(stData, service_secret_key);

        // 6. Encrypt Session Key for Client
        std::string encryptedReplyKey = Crypto::simulateEncrypt(std::to_string(client_service_key), client_tgs_key);

        return {encryptedReplyKey, encryptedST};
    }

    // Step 3: Target Service Verification
    bool verifyServiceRequest(const Service_Request& req, int delay_seconds = 0) {
        std::cout << "\n[Target Service] Processing Access Request...\n";

        // 1. Decrypt Service Ticket
        std::string stData = Crypto::simulateDecrypt(req.encryptedServiceTicket, service_secret_key);
        if (stData == "DECRYPTION_FAILED") return false;

        // Extract Client-Service Key
        long long client_service_key = std::stoll(stData.substr(stData.find_last_of('|') + 1));

        // 2. Decrypt Authenticator
        std::string authData = Crypto::simulateDecrypt(req.encryptedAuthenticator, client_service_key);
        if (authData == "DECRYPTION_FAILED") return false;

        // 3. Edge Case Handling: Replay Attack Check (Timestamp validation)
        time_t current_time = time(nullptr) + delay_seconds;
        time_t auth_time = std::stoll(authData.substr(authData.find('|') + 1));
        
        if (current_time - auth_time > 5) { // 5-second validity window
            std::cout << "[!] Security Alert: Authenticator timestamp expired! Possible Replay Attack detected.\n";
            return false;
        }

        std::cout << "[Target Service] Authentication complete. Access Granted.\n";
        return true;
    }

    // Expose AS public parameters for DH
    long long getASPublicKey() {
        return Crypto::modExp(dh_generator, as_private_key, dh_prime);
    }
};

#endif