#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (FAILED(WSAStartup(wVersionRequested, &wsaData)))
	{
		int error = WSAGetLastError();
		cout << "Error code " << error << endl;
		exit(-1);
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		cout << "Error code " << error << endl;
		exit(-1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(1280);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	int c = bind(s, (struct sockaddr*) & local, sizeof(local));
	int r = listen(s, 5);

	char str[255], success[11] = "Polindrome", not_succes[15] = "Not polindrome"; 
	bool flag = true;

	while (true)
	{
		flag = true;
		sockaddr_in remote_addr;
		int size = sizeof(remote_addr);

		cout << "Ожидание установки соединения...\n" << endl;

		SOCKET s2 = accept(s, (struct sockaddr*) & remote_addr, &size);
		if (!s2)
		{
			int error = WSAGetLastError();
			cout << "Error code " << error << endl;
		}
		else
			cout << "Соединение установлено!\n" << endl;

		while (recv(s2, str, sizeof(str), NULL) != 0)
		{
			flag = true;
			cout << str << endl;
			for (size_t i = 0, j = strlen(str) - 1; i < j; ++i, --j)
			{
				if (str[i] != str[j])
				{
					flag = false;
					break;
				}
			}
			if (flag)
				send(s2, success, sizeof(str), NULL);
			else
				send(s2, not_succes, sizeof(str), NULL);
			system("cls");
		}
		closesocket(s2);
	}
	WSACleanup();
	system("pause");
	return 0;
}
