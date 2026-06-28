#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define SERVER_IP "127.0.0.1"
#define PORT "8888"
#define BUFFER_SIZE 512

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct addrinfo hints, *res;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(SERVER_IP, PORT, &hints, &res);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        printf("Connect failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(res);

    printf("Connected to server. Type messages:\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);

        int len = (int)strlen(buffer);
        if (buffer[len-1] == '\n') buffer[len-1] = '\0'; 

        if (strcmp(buffer, "exit") == 0)
            break;

        send(sock, buffer, (int)strlen(buffer), 0);
        int recv_size = recv(sock, buffer, BUFFER_SIZE-1, 0);
        if (recv_size > 0) {
            buffer[recv_size] = '\0';
            printf("Server echoed: %s\n", buffer);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
