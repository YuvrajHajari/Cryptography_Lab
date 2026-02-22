#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(8081), inet_addr("127.0.0.1")};

    if (connect(s, (SOCKADDR*)&serv, sizeof(serv)) == 0) {
        int id = 5;      // Target User ID
        int token = 509; // WRONG TOKEN to trigger failure

        send(s, (char*)&id, sizeof(id), 0);
        send(s, (char*)&token, sizeof(token), 0);
        
        printf("Sent User_ID: 5 with WRONG Token: 509\n");
    }
    closesocket(s); WSACleanup();
    return 0;
}