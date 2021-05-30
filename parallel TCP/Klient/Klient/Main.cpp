#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h> 
#include<iostream> 
#include<winsock2.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	WORD wVersionRequested; 
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2); 
	err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0) 
		return -1;

	char buf[100], action[2], numh[5];
	int act, nur = 0;
	string book, author, publ, str;
	int year, page, number;

	while (true)
	{
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(1280);
		dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));

		cout << "1 - ����������" << endl << "2 - ��������" << endl << "3 - ��������������"
			<< endl << "4 - ��������" << endl << "5 - ������� " << endl;
		cin >> act;
		snprintf(action, sizeof(action), "%d", act);
		send(s, action, sizeof(action), 0);

		switch (act)
		{
		case 1:
			nur++;

			cout << "�������\t" << endl << "�������� �����" << endl;
			cin >> book;
			for (unsigned int i = 0; i < book.size(); i++)
				buf[i] = book[i];
			buf[book.size()] = '\0';
			send(s, buf, sizeof(buf), 0);

			cout << "��� ������" << endl;
			cin >> author;
			for (unsigned int i = 0; i < author.size(); i++)
				buf[i] = author[i];
			buf[author.size()] = '\0';
			send(s, buf, sizeof(buf), 0);

			cout << "��� �������" << endl;
			cin >> year;
			str = to_string(year);
			for (unsigned int i = 0; i < str.size(); i++)
				buf[i] = str[i];
			buf[str.size()] = '\0';
			send(s, buf, sizeof(buf), 0);

			cout << "������������" << endl;
			cin >> publ;
			for (unsigned int i = 0; i < publ.size(); i++)
				buf[i] = publ[i];
			buf[publ.size()] = '\0';
			send(s, buf, sizeof(buf), 0);

			cout << "���������� ������" << endl;
			cin >> page;
			str = to_string(page);
			for (unsigned int i = 0; i < str.size(); i++)
				buf[i] = str[i];
			buf[str.size()] = '\0';
			send(s, buf, sizeof(buf), 0);

			cout << "��������������� ����� �����" << endl;
			cin >> number;
			str = to_string(number);
			for (unsigned int i = 0; i < str.size(); i++)
				buf[i] = str[i];
			buf[str.size()] = '\0';
			send(s, buf, sizeof(buf), 0);
			break;
		case 2:
			cout << endl;
			for (int i = 0; i < nur; i++)
			{
				recv(s, buf, sizeof(buf), 0);
				cout << i + 1 << ". " << buf << endl;
			}
			cout << endl;
			break;
		case 3:
			cout << endl;
			for (int i = 0; i < nur; i++)
			{
				recv(s, buf, sizeof(buf), 0);
				cout << i + 1 << ". " << buf << endl;
			}
			cout << endl;
			cout << "������� ����� ������������� �����: ";
			cin >> numh;
			send(s, numh, strlen(numh) + 1, 0);

			cout << "����� ������ �� ������ ��������?" << endl << "1 - ��������" << endl << "2 - ������" << endl
				<< "3 - ��� �������" << endl << "4 - ������������" << endl << "5 - ���������� �������" << endl
				<< "6 - ��������������� �����" << endl;
			cin >> act;

			snprintf(action, sizeof(action), "%d", act);
			act = action[0];
			send(s, action, sizeof(action), 0);
			switch (act)
			{
			case '1':
				cout << "����� ��������: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '2':
				cout << "����� �����: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '3':
				cout << "����� ��� �������: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '4':
				cout << "����� ������������: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '5':
				cout << "����� ���������� �������: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '6':
				cout << "����� ��������������� �����: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			}
			break;
		case 4:
			cout << endl;
			for (int i = 0; i < nur; i++)
			{
				recv(s, buf, sizeof(buf), 0);
				cout << i + 1 << ". " << buf << endl;
			}
			cout << endl;
			cout << "������� ����� ��������� �����: ";
			cin >> numh;
			send(s, numh, strlen(numh) + 1, 0);
			nur--;
			break;
		case 5:
			cout << "������� ������� ������: ";
			cin >> numh;
			send(s, numh, strlen(numh) + 1, 0);

			recv(s, buf, sizeof(buf), 0);
			cout << buf << endl;
			int p;
			p = atoi(buf);
			cout << "���������� ����������: " << endl << endl;
			for (int i = 0; i < p; i++)
			{
				recv(s, buf, sizeof(buf), 0);
				cout << buf << endl;
			}
			cout << endl;
			break;
		}
		closesocket(s);		
	}
	WSACleanup();
	return 0;
}
