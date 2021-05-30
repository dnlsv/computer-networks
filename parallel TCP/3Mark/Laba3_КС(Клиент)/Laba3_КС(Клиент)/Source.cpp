#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <string>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int checkInput(int left_range, int right_range);

int main() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	// указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(s, (sockaddr*)& dest_addr, sizeof(dest_addr)) != 0)
	{
		cout << "\nConnect error.";
		system("pause");
		return -1;
	}
	char buf[256], action[2], number[5];
	int act, req_player;
	bool flag = true;
	while (flag)
	{
		ZeroMemory(buf, 256);
		cout << "Choose action:\n\n";
		cout << "1. Get information about the youngest player.\n";
		cout << "2. Show players list.\n";
		cout << "3. Add information.\n";
		cout << "4. Edit information.\n";
		cout << "5. Delete information.\n";
		cout << "0. Disconnect from server.\n\n";
		cout << "Enter the answer: ";
		act = checkInput(0, 5);
		cout << endl;
		snprintf(action, sizeof(action), "%d", act);
		send(s, action, sizeof(action), 0);
		switch (act)
		{
		case 1:
			recv(s, buf, sizeof(buf), 0);
			cout << "Received info:\n" << buf << "\n\n";
			break;
		case 2:
			recv(s, buf, sizeof(buf), 0);
			cout << "Received info:\n" << buf << "\n\n";
			break;
		case 3:
		{
			cout << "Input new player's name: ";
			string name;
			cin >> name;
			cout << "\nInput new player's number: ";
			int number;
			cin >> number;
			cout << "\nInput new player's age: ";
			int age;
			cin >> age;
			cout << "\nInput new player's height: ";
			int height;
			cin >> height;
			cout << "\nInput new player's weight: ";
			int weight;
			cin >> weight;
			cout << endl;
			string result_string = name + ' ' + to_string(number) + ' ' + to_string(age) + ' ' + to_string(height) + ' ' + to_string(weight);
			for (int i = 0; i != result_string.size(); i++)
				buf[i] = result_string[i];
			send(s, buf, strlen(buf) + 1, 0);
		}
			break;
		case 4:
		{
			recv(s, number, sizeof(number), 0);
			int num = atoi(number);
			if (num == 0)
			{
				cout << "Players list is empty!" << endl;
				break;
			}
			recv(s, buf, sizeof(buf), 0);
			cout << buf;
			
			while (true)
			{
				cout << "\nAmount of players on server: " << num << endl;
				cout << "\nInput player's number to edit: ";
				cin >> number;
				req_player = atoi(number);
				if (req_player > 0 && req_player <= num)
					break;
				cout << "Incorrect input, there are no players with such number!" << endl;
			}
			send(s, number, strlen(number) + 1, 0);
			cout << "Choose what to edit:\n\t1.Name\n\t2.Player's number\n\t3.Age\n\t4.Height\n\t5.Weight\n\tEnter the answer: ";
			act = checkInput(1, 5);
			snprintf(action, sizeof(action), "%d", act);
			act = action[0];
			send(s, action, strlen(action) + 1, 0);
			switch (act)
			{
			case '1':
				cout << "New Name: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '2':
				cout << "New Number: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '3':
				cout << "New Age: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '4':
				cout << "New Height: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			case '5':
				cout << "New Weight: ";
				cin >> buf;
				send(s, buf, strlen(buf) + 1, 0);
				break;
			}
			break;
		}
			break;
		case 5:
		{
			recv(s, number, sizeof(number), 0);
			int num = atoi(number);
			if (num == 0)
			{
				cout << "Players list is empty!" << endl;
				break;
			}
			recv(s, buf, sizeof(buf), 0);
			cout << buf;

			while (true)
			{
				cout << "\nAmount of players on server: " << num << endl;
				cout << "\nInput player's number to delete: ";
				cin >> number;
				req_player = atoi(number);
				if (req_player > 0 && req_player <= num)
					break;
				cout << "Incorrect input, there are no players with such number!" << endl;
			}
			send(s, number, strlen(number) + 1, 0);
		}
			break;
		case 0:
			flag = false;
		}
	}
	closesocket(s);
	WSACleanup();
	cout << "\nShutting down...\n\n";
	system("pause");
	return 0;
}

int checkInput(int left_range, int right_range)
{
	int a;
	while (true)
	{
		cin >> a;
		if (a >= left_range && a <= right_range && cin.get() == '\n') break;
		else
		{
			cin.clear();
			cin.ignore(256, '\n');
			cout << "Incorrect input. Please, repeat.";
		}
	}
	return a;
}