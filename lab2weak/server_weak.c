#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

// Modular Exponentiation function for Square-and-Multiply
long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1; 
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1; 
        base = (base * base) % mod;
    }
    return res;
}

int main() {
    WSADATA wsa; 
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8080); // Set to Port 8080

    bind(s, (SOCKADDR*)&serv, sizeof(serv)); 
    listen(s, 3);

    printf("=== WEAK AUTHENTICATION SERVER ===\n");
    printf("Modulus n = 9 (COMPOSITE - VULNERABLE!)\n");
    printf("Secret k = 7\n");
    printf("Server listening on port 8080...\n\n");

    while (1) {
        // Accept the connection from the client
        SOCKET c = accept(s, NULL, NULL);
        if (c == INVALID_SOCKET) continue;

        int id = 0;
        int token = 0;

        // Receive User_ID and Token from the client
        recv(c, (char*)&id, sizeof(id), 0);
        recv(c, (char*)&token, sizeof(token), 0);

        // Server performs verification math: Token = ID^k mod n
        int expected_token = (int)mod_exp(id, 7, 9); 

        printf("--- Authentication Request ---\n");
        printf("User_ID received: %d\n", id);
        printf("Token received: %d\n", token);
        printf("Expected token: %d\n", expected_token);

        if (token == expected_token) {
            printf("Result: [v] AUTHENTICATION SUCCESS\n\n");
            send(c, "SUCCESS: Authentication approved", 32, 0);
        } else {
            printf("Result: [x] AUTHENTICATION FAILED\n\n");
            send(c, "FAILED: Invalid token", 21, 0);
        }

        closesocket(c); // Close connection for this request
    }

    closesocket(s);
    WSACleanup();
    return 0;
}