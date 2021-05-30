#include <winsock2.h> 
#include <stdlib.h> 
#include <iostream> 
#include <Ws2tcpip.h> 
#pragma comment (lib, "ws2_32.lib") 
using namespace std;


int main()
{
	char buf[100], b[100];
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		cout << "Can't start Winsock! " << err;
		return -1;
	}
	int t, length, choice = 1;
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(1024);
	inet_pton(AF_INET, "127.0.0.1", &(add.sin_addr));

	bool flag = true;
	while (flag)
	{
		t = sizeof(add);
		ZeroMemory(buf, 100);
		ZeroMemory(b, 100);
		cout << "Input string: ";
		cin.getline(buf, sizeof(buf), '\n');
		int send_ok = sendto(s, buf, strlen(buf) + 1, 0, (sockaddr*)& add, t);
		if (send_ok == SOCKET_ERROR)
			cout << "That didn't work! " << WSAGetLastError() << endl;
		int rv = recvfrom(s, b, sizeof(b), 0, (sockaddr*)& add, &t);

		cout << b;
		cout << "\n\nAgain?\n1. Yes\n2. Exit" << endl;
		cout << "->";
		cin >> choice;
		if (choice == 2)
			flag = false;
		cin.ignore();
	}
	closesocket(s);
	WSACleanup();
	return 0;
}