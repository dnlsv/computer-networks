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
	char b[255], temp[255], success[11] = "Polindrome", not_polindrome[15] = "Not polindrome"; bool flag = true;
	while (true)
	{
		flag = true;
		//структура определяет удаленный адрес,
		//с которым соединяется сокет
		sockaddr_in remote_addr;
		int size = sizeof(remote_addr);
		cout << "Ожидание установки соединения...\n" << endl;
		SOCKET s2 = accept(s, (struct sockaddr*) & remote_addr, &size);
		if (!s2)
		{
			int error = WSAGetLastError();
			cout << "Error code " << error << endl;
		}
		cout << "Соединение установлено!\n" << endl;

		while (recv(s2, b, sizeof(b), NULL) != 0)
		{
			flag = true;
			cout << b << endl;
			strcpy_s(temp, b);
			for (size_t i = 0, j = strlen(b) - 1; i < j; ++i, --j)
			{
				if (b[i] != b[j])
				{
					flag = false;
					break;
				}
			}
			if (flag)
				send(s2, success, sizeof(b), NULL);
			else
				send(s2, not_polindrome, sizeof(b), NULL);
			system("cls");
		}
		closesocket(s2);
	}
	WSACleanup();
	system("pause");
	return 0;
}
/*
#include <winsock2.h>
#include <iostream> 
#include <stdio.h> 
#include <stdlib.h>

int main() {
	WORD wVersionRequested; 
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2); 
	WSAStartup(wVersionRequested, &wsaData);
	SOCKET	s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in local; 
	local.sin_family = AF_INET; 
	local.sin_port = htons(1280); 
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	int c = bind(s, (struct sockaddr*)&local, sizeof(local));
	int r = listen(s, 5);
	SOCKET accept(SOCKET s, struct sockaddr FAR * addr, int FAR* addrlen);
	while (true) {
		char buf[255], res[100], b[255], *Res;
		//структура определяет удаленный адрес,
		//с которым	соединяется сокет sockaddr_in remote_addr;
		int size = sizeof(remote_addr);
		SOCKET s2 = accept(s, (struct sockaddr*)&remote_addr, &size);
		int send(SOCKET s, const char FAR *buf, int len, int flags);
		while (recv(s2, b, sizeof(b), 0) != 0) {
			int i = 0;
			for (unsigned j = 0; j <= strlen(b); j++) if (b[j] == 'a') i++;

			_itoa(i, res, 10);

			Res = new char[strlen(res) + 1]; strcpy(Res, res); Res[strlen(res)] = '\0';

		}
		closesocket(s2);
	}
	WSACleanup();
}
*/
