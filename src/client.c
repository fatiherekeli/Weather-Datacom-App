#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>  // Windows soket kütüphanesi

#pragma comment(lib, "ws2_32.lib")  // Windows soket kütüphanesini ekle

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"  // Sunucu IP adresi
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];
    int bytesReceived;

    // Winsock başlatma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock başlatılamadı.\n");
        return 1;
    }

    // Soket oluşturma
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Soket oluşturulamadı.\n");
        WSACleanup();
        return 1;
    }

    // Sunucu adresini yapılandırma
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Sunucuya bağlanma
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Sunucuya bağlanılamadı.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Kullanıcıdan veri al
    printf("Sunucuya gönderilecek mesajı girin: ");
    fgets(sendBuffer, BUFFER_SIZE, stdin);
    sendBuffer[strcspn(sendBuffer, "\n")] = '\0';  // Yeni satır karakterini kaldır

    // Sunucuya veri gönderme
    if (send(sock, sendBuffer, strlen(sendBuffer), 0) == SOCKET_ERROR) {
        printf("Veri gönderilemedi.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Sunucudan yanıt alma
    bytesReceived = recv(sock, recvBuffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        printf("Veri alınamadı.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Yanıtı ekrana yazdırma
    recvBuffer[bytesReceived] = '\0';  // Null terminator ekle
    printf("Sunucudan gelen yanıt: %s\n", recvBuffer);

    // Soketi kapatma ve Winsock'ı temizleme
    closesocket(sock);
    WSACleanup();

    return 0;
}