#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int id = atoi(argv[1]);
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    connect(s, (SOCKADDR*)&serv, sizeof(serv));

    // Calculate token for display
    int token = 1;
    for(int i=0; i<7; i++) token = (token * id) % 9;

    printf("=== LEGITIMATE CLIENT ===\n");
    printf("User_ID: %d\n", id);
    printf("Computing token: %d^7 mod 9 = %d\n", id, token);
    printf("Sent to server: User_ID=%d, Token=%d\n", id, token);

    send(s, (char*)&id, sizeof(id), 0);
    send(s, (char*)&token, sizeof(token), 0);

    char buf[1024] = {0};
    recv(s, buf, 1024, 0);
    printf("Server response: %s\n", buf);
    closesocket(s); WSACleanup();
    return 0;
}