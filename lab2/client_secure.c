#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

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

int main(int argc, char* argv[]) {
    if (argc < 2) return 1; // Need User_ID as argument
    int user_id = atoi(argv[1]);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(8081);

    connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));

    // Client computes token using secret k=23 and modulus n=97
    int token = mod_exp(user_id, 23, 97);
    send(ConnectSocket, (char*)&user_id, sizeof(user_id), 0);
    send(ConnectSocket, (char*)&token, sizeof(token), 0);

    char buffer[1024] = { 0 };
    recv(ConnectSocket, buffer, 1024, 0);
    printf("Server response: %s\n", buffer);

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}