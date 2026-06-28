#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define PORT "8888"
#define BUFFER_SIZE 512

int main() {
    WSADATA wsa;
    SOCKET listen_sock, client_sock;
    struct addrinfo hints, *res;
    char buffer[BUFFER_SIZE];
    int recv_size;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;      

    getaddrinfo(NULL, PORT, &hints, &res);

    listen_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listen_sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    if (bind(listen_sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(res);

    // Listen
    if (listen(listen_sock, 1) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %s...\n", PORT);

    // Accept a client
    client_sock = accept(listen_sock, NULL, NULL);
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }
    
    printf("Client connected!\n");

    while ((recv_size = recv(client_sock, buffer, BUFFER_SIZE-1, 0)) > 0) {
        buffer[recv_size] = '\0';
        printf("Client: %s\n", buffer);
        send(client_sock, buffer, recv_size, 0);
    }

    printf("Client disconnected\n");

    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
