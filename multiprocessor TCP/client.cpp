#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include <sstream>

using namespace std;

int N = 5;

int main()
{
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(7500);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1");

	int s;
	char buf[256], p, p1, b[256];

	s = socket(AF_INET, SOCK_STREAM, 0);
	connect(s, (struct sockaddr *)&peer, sizeof(peer));
	while (true)
	{
		//Выбор пункта меню и отправка его серверу
cout << "choose:\n1 - Delete\n2 - Edit\n3 - View\n4 - Search\n5 - Add\n0 - Exit\n";
		cin >> buf;
		buf[1] = '\0';
		send(s, buf, sizeof(buf), 0);
		p = buf[0];

		switch (p)
		{
		case '1': //Delete
			buf[0] = '\0';
			cout << "Input the name you want to delete\n";
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, "\0");
			send(s, buf, sizeof(buf), 0);
			recv(s, buf, sizeof(buf), 0);
			if (atoi(buf) == 1)
			{
				cout << "deleted\n";
				N--;
			}
			else
			{
				cout << "not found\n";
			}
			break;
		case '2': //Edit
			cout << "What number to edit\n";
			cin >> buf;
			send(s, buf, sizeof(buf), 0);
			cout << "What field to edit:\n1 - Name\n2 - Marks\n";
			cin >> buf;
			send(s, buf, sizeof(buf), 0);
			p1 = buf[0];
			buf[0] = '\0';
			switch (p1)
			{	//Введите новые поля
			case '1': cout << "Name:\n";
				fflush(stdin); fflush(stdout);
				cin >> b; strcat(buf, b); strcat(buf, " ");
				cin >> b; strcat(buf, b); strcat(buf, " ");
				cin >> b; strcat(buf, b); strcat(buf, "\0");
				send(s, buf, sizeof(buf), 0);
				break;
			case '2':fflush(stdin); fflush(stdout);
				cout << "Marks: ";
				cin >> buf;
				send(s, buf, sizeof(buf), 0);
				break;
			default: break;
			}
			break;
		case '3'://View
			for (int i = 0; i < N; i++)
			{
				recv(s, buf, sizeof(buf), 0); cout << buf;
			}
			break;
		case '4'://search
			buf[0] = '\0';
			cout << "Input the name you want to search\n";
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, "\0");
			send(s, buf, sizeof(buf), 0);
			recv(s, buf, sizeof(buf), 0);
			if (strlen(buf) == 0)
			{
				cout << "not found\n";
				break;
			}
			cout << buf;
			break;
		case '5'://add
		{
			N++;
			char tmpl[10];
			sprintf(tmpl, "%d", N);
			buf[0] = '\0';
			cout << "Input name, marks\n";
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, " ");
			strcat(buf, tmpl); strcat(buf, " ");
			cin >> b; strcat(buf, b); strcat(buf, "\0");
			send(s, buf, sizeof(buf), 0);
		}
		break;
		case '0'://Exit
			send(s, "0\0", sizeof("0\0"), 0);
			exit(0);
		}
	}
}