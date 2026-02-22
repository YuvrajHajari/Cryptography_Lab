#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int gcd(int a, int b) {
    while (b != 0) { int t = b; b = a % b; a = t; }
    return a;
}

long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1; base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1; base = (base * base) % mod;
    }
    return res;
}

int main() {
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(8081), INADDR_ANY};
    bind(s, (SOCKADDR*)&serv, sizeof(serv)); listen(s, 3);

    printf("=== SECURE AUTHENTICATION SERVER ===\n");
    printf("Modulus n = 97 (PRIME - SECURE!)\n");
    printf("Secret k = 23\n");
    printf("phi(97) = 96 (large cycle length)\n");
    printf("Server listening on port 8081...\n\n");

    while (1) {
        SOCKET c = accept(s, NULL, NULL);
        int id = 0, token = 0;
        recv(c, (char*)&id, sizeof(id), 0);
        recv(c, (char*)&token, sizeof(token), 0);

        int common_factor = gcd(id, 97);
        int expected = (int)mod_exp(id, 23, 97);

        printf("--- Authentication Request ---\n");
        printf("User_ID received: %d\n", id);
        printf("Token received: %d\n", token);
        printf("GCD(%d, 97) = %d <- Coprime verified v\n", id, common_factor);
        printf("Expected token: %d\n", expected);

        if (token == expected) {
            printf("Result: v AUTHENTICATION SUCCESS\n\n");
            send(c, "SUCCESS", 7, 0);
        } else {
            printf("Result: X AUTHENTICATION FAILED\n\n");
            send(c, "FAILED", 6, 0);
        }
        closesocket(c);
    }
    return 0;
}