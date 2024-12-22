#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

DWORD WINAPI client_thread(LPVOID client_socket) {
    SOCKET socket = *(SOCKET *)client_socket;
    char buffer[BUFFER_SIZE];
    int recv_size;

    while ((recv_size = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[recv_size] = '\0';
        printf("Received: %s\n", buffer);

        if (send(socket, buffer, recv_size, 0) == SOCKET_ERROR) {
            printf("Send failed. Error Code: %d\n", WSAGetLastError());
        }
    }

    if (recv_size == 0) {
        printf("Client disconnected\n");
    } else {
        printf("recv failed. Error Code: %d\n", WSAGetLastError());
    }

    closesocket(socket);
    return 0;
}

int main() {
    WSADATA wsa_data;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_size = sizeof(client_addr);

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        CreateThread(NULL, 0, client_thread, (LPVOID)&client_socket, 0, NULL);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
