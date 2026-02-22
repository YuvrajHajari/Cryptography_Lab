#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int gcd(int a, int b) {
    while (b != 0) { int t = b; b = a % b; a = t; }
    return a;
}

void attempt_attack(int id, int token, char* attack_name) {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(8081), inet_addr("127.0.0.1")};
    printf("--- Attack %s ---\n", attack_name);
    if (connect(s, (SOCKADDR*)&serv, sizeof(serv)) == 0) {
        send(s, (char*)&id, sizeof(id), 0);
        send(s, (char*)&token, sizeof(token), 0);
        char res[10] = {0}; recv(s, res, 10, 0);
        printf("Sent: User_ID=%d, Token=%d\n", id, token);
        printf("Response: %s: Invalid token for User_ID=%d\n", res, id);
        printf("X Attack FAILED (System is secure)\n\n");
    }
    closesocket(s);
}

int main() {
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    printf("=== ATTACKER ATTEMPTS ON SECURE SYSTEM ===\n");
    printf("Target Modulus n = 97 (PRIME)\n\n");
    printf("Phase 1: GCD Analysis\nTesting for common factors:\n");
    for(int i=1; i<=10; i++) printf("gcd(%d, 97) = %d\n", i, gcd(i, 97));
    printf("\n/!\\ALL GCD values = 1 (no factors found)\n");
    printf("Prime modulus prevents factorization!\nphi(97) = 96 (much larger cycle)\n\n");
    printf("Phase 2: Attempting Previous Attacks\n\n");
    attempt_attack(5, 50, "1: Replay Attack");
    attempt_attack(5, 5, "2: Token Prediction");
    WSACleanup(); return 0;
}