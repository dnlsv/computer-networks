#pragma comment (lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h> 
#include <string> 
#include <Winsock2.h> 
#include <windows.h> 
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, ".UTF8");
    char buf[100], b[100];
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) { return 0; }

    SOCKET s;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(1024);

    add.sin_addr.s_addr = inet_addr("127.0.0.1");

    int t, flag;

    while (true)
    {
        t = sizeof(add);
        cout << "Введите строку" << endl;
        cin.getline(buf, 100, '\n');

        sendto(s, buf, lstrlen(buf), 0, (struct sockaddr*)&add, t);
        int rv = recvfrom(s, b, sizeof(b), 0, (struct sockaddr*)&add, &t);
        b[rv] = 0;
        cout << b << endl;

        cout << endl << "Еще раз?" << endl << "1 - Да" << endl << "0 - Нет" << endl;
        cin >> flag;
        if (flag == 0)
            break;
        cin.ignore();
    }
    closesocket(s);

    WSACleanup();
    return 0;
}
