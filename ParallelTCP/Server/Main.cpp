#include <stdio.h> 
#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

struct Books
{
	string book;
	string author;
	int year;
	string publ;
	int page;
	int number;
};

vector <Books> bk;
Books m;

DWORD WINAPI ThreadFunc(LPVOID client_socket) {
	SOCKET s2 = ((SOCKET*)client_socket)[0]; 
	char buf[100], act, action[2] , num[2];
	int k, ch;
	string str, ork;
	
	
	
	while (recv(s2, action, sizeof(action), 0)) {
		act = action[0];
		switch (act)
		{
		case '1':
			bk.push_back(m);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).book = string(buf);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).author = string(buf);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).year = atoi(buf);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).publ = string(buf);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).page = atoi(buf);

			recv(s2, buf, sizeof(buf), 0);
			bk.at(bk.size() - 1).number = atoi(buf);

			break;
		case '2':
			for (unsigned int i = 0; i < bk.size(); i++)
			{
				ork = bk.at(i).book + ' ' + bk.at(i).author + ' ' + to_string(bk.at(i).year)
					+ ' ' + bk.at(i).publ + ' ' + to_string(bk.at(i).page) + ' ' + to_string(bk.at(i).number);
				for (unsigned int j = 0; j < ork.size(); j++)
					buf[j] = ork[j];
				buf[ork.size()] = '\0';
				send(s2, buf, sizeof(buf), 0);
			}
			break;
		case '3':
			for (unsigned int i = 0; i < bk.size(); i++)
			{
				ork = bk.at(i).book + ' ' + bk.at(i).author + ' ' + to_string(bk.at(i).year)
					+ ' ' + bk.at(i).publ + ' ' + to_string(bk.at(i).page) + ' ' + to_string(bk.at(i).number);
				for (unsigned int j = 0; j < ork.size(); j++)
					buf[j] = ork[j];
				buf[ork.size()] = '\0';
				send(s2, buf, sizeof(buf), 0);
			}

			recv(s2, num, sizeof(num), 0);
			k = atoi(num) - 1;

			recv(s2, action, sizeof(action), 0);
			act = action[0];
			switch (act)
			{
			case '1':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).book = buf;
				break;
			case '2':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).author = buf;
				break;
			case '3':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).year = atoi(buf);
				break;
			case '4':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).publ = buf;
				break;
			case '5':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).page = atoi(buf);
				break;
			case '6':
				recv(s2, buf, sizeof(buf), 0);
				bk.at(k).number = atoi(buf);
				break;
			}
			break;
		case '4':
			for (unsigned int i = 0; i < bk.size(); i++)
			{
				ork = bk.at(i).book + ' ' + bk.at(i).author + ' ' + to_string(bk.at(i).year)
					+ ' ' + bk.at(i).publ + ' ' + to_string(bk.at(i).page) + ' ' + to_string(bk.at(i).number);
				for (unsigned int j = 0; j < ork.size(); j++)
					buf[j] = ork[j];
				buf[ork.size()] = '\0';
				send(s2, buf, sizeof(buf), 0);
			}

			recv(s2, num, sizeof(num), 0);
			k = atoi(num) - 1;
			bk.erase(bk.begin() + k);
			break;
		case '5':
			
			ch = 0;
			recv(s2, num, sizeof(num), 0);
			for (unsigned int i = 0; i < bk.size(); i++)
			{
				if (string(num) == bk.at(i).author)
					ch++;
			}
			char b[2];

			b[0] = to_string(ch)[0];
			b[1] = '\0';
			send(s2, b, sizeof(b), 0);
			for (unsigned int i = 0; i < bk.size(); i++)
			{
				if (string(num) == bk.at(i).author)
				{
					ork = bk.at(i).book + ' ' + bk.at(i).author + ' ' + to_string(bk.at(i).year)
						+ ' ' + bk.at(i).publ + ' ' + to_string(bk.at(i).page) + ' ' + to_string(bk.at(i).number);
					for (unsigned int j = 0; j < ork.size(); j++)
						buf[j] = ork[j];
					buf[ork.size()] = '\0';
					send(s2, buf, sizeof(buf), 0);
				}
			}
		}

	}
	closesocket(s2); 
	return 0;
}

int numcl = 0;

void print() {
	if (numcl)
		printf("%d client connected\n", numcl);
	else
		printf("No clients connected\n");
}


int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
		return -1;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 5);
	cout << "Server receive ready" << endl;
	cout << endl;
	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		numcl++;
		print();
		DWORD thID;
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
	return 0;
}



