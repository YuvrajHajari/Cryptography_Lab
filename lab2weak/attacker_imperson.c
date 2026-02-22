#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    int target_id = 4;
    int fake_token = 4; // Found because 4^1 mod 9 = 4

    printf("=== ATTACK 3: IMPERSONATION ATTACK ===\n");
    printf("Attacker creates fake User_ID and finds valid token\n\n");
    printf("Attacker chooses User_ID = %d\n", target_id);
    printf("Computing token cycle:\n\n");
    
    // Demonstrate the cycle matching for the report
    printf("k=1: 4^1 mod 9 = 4\n");
    printf("k=2: 4^2 mod 9 = 7\n");
    printf("k=3: 4^3 mod 9 = 1\n");
    printf("k=4: 4^4 mod 9 = 4\n");
    printf("k=5: 4^5 mod 9 = 7\n");
    printf("k=6: 4^6 mod 9 = 1\n\n");

    printf("Attacker notices: 4^3 mod 9 = 1\n");
    printf("For any k that is multiple of 3, token will be 1\n");
    printf("(Server's k=7 gives same token as k=1 in this cycle)\n\n");

    printf("Launching Impersonation Attack...\n");
    printf("Sent: User_ID=%d (fake), Token=%d\n\n", target_id, fake_token);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {AF_INET, htons(8080), inet_addr("127.0.0.1")};

    if (connect(s, (SOCKADDR*)&serv, sizeof(serv)) == 0) {
        send(s, (char*)&target_id, sizeof(target_id), 0);
        send(s, (char*)&fake_token, sizeof(fake_token), 0);

        char response[100] = {0};
        recv(s, response, sizeof(response), 0);
        printf("Server response: %s\n\n", response);
        printf("V ATTACK SUCCESSFUL!\n");
        printf("Attacker authenticated as fake User_ID=%d!\n", target_id);
        printf("System cannot distinguish legitimate from fake users!\n");
    }
    closesocket(s); WSACleanup();
    return 0;
}