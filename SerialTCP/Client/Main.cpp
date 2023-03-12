#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, ".UTF8");

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    if (FAILED(WSAStartup(wVersionRequested, &wsaData)))
    {
        int error = WSAGetLastError();
        cout << "Error code " << error << endl;
        exit(-1);
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        int error = WSAGetLastError();
        cout << "Error code " << error << endl;
        exit(-1);
    }

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (SOCKET_ERROR == connect(s, (struct sockaddr*)&peer, sizeof(peer)))
    {
        int error = WSAGetLastError();
        cout << "Error code " << error << endl;
        exit(-1);
    }

    char str[255], buf[255];
    while (1)
    {
        system("cls");
        cout << "Введите строку." << endl;
        cin.getline(str, 100, '\n');
        send(s, str, sizeof(str), 0);
        if (recv(s, buf, sizeof(buf), 0) != 0)
            cout << endl << buf << endl;
        cout << "\nПродолжить? \n\n1 - Да\n0 - Завершить" << endl;
        if (_getch() == '0')
            break;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
