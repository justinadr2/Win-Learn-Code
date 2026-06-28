#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

#define PORT "8888"
#define BUFFER_SIZE 512

DWORD WINAPI client_thread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buffer[BUFFER_SIZE];
    int recv_size;

    while ((recv_size = recv(client_sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[recv_size] = '\0';
        printf("Client [%llu]: %s\n", (unsigned long long)GetCurrentThreadId(), buffer);

        // Echo back
        send(client_sock, buffer, recv_size, 0);
    }

    printf("[-] Client [%llu] disconnected\n", (unsigned long long)GetCurrentThreadId());

    closesocket(client_sock);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET listen_sock;
    struct addrinfo hints, *res;

    WSAStartup(MAKEWORD(2,2), &wsa);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    getaddrinfo(NULL, PORT, &hints, &res);

    listen_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(listen_sock, res->ai_addr, (int)res->ai_addrlen);
    freeaddrinfo(res);

    listen(listen_sock, SOMAXCONN);
    printf("Server listening on port %s...\n", PORT);

    while (1) {

        SOCKET client_sock = accept(listen_sock, NULL, NULL);
        if (client_sock == INVALID_SOCKET)
            continue;

        printf("[+] Client [%llu] connected\n", (unsigned long long)GetCurrentThreadId());

        HANDLE thread = CreateThread(NULL, 0, client_thread, (LPVOID)client_sock, 0, NULL);

        CloseHandle(thread); 
    }

    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
