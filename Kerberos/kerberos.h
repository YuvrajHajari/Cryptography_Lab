#ifndef KERBEROS_H
#define KERBEROS_H

#include <string>
#include <ctime>

// Kerberos Ticket Structure
struct Ticket {
    std::string clientID;
    std::string serverID;
    long long sessionKey;
    time_t timestamp;
    int validityPeriod;
};

// Kerberos Authenticator Structure (Prevents Replay Attacks)
struct Authenticator {
    std::string clientID;
    time_t timestamp;
};

// Network Message Structures
struct AS_Reply {
    std::string encryptedSessionKey; // Encrypted with DH Shared Key
    std::string encryptedTGT;        // Ticket Granting Ticket (Encrypted with TGS Secret)
};

struct TGS_Request {
    std::string encryptedTGT;
    std::string encryptedAuthenticator; // Encrypted with Client-TGS Session Key
    std::string serviceID;
};

struct TGS_Reply {
    std::string encryptedServiceSessionKey; // Encrypted with Client-TGS Session Key
    std::string encryptedServiceTicket;     // Encrypted with Service Secret
};

struct Service_Request {
    std::string encryptedServiceTicket;
    std::string encryptedAuthenticator; // Encrypted with Client-Service Session Key
};

#endif