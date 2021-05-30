#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

struct player {
	string name;
	int number;
	int age;
	int height;
	int weight;
};

void printNumberOfClients();
void showPlayersFromFile(vector <player>&);
DWORD WINAPI ThreadFunc(LPVOID client_socket);
string quest(vector <player>& a);
string getInfo(vector <player>& a);
void writeToEndOfFileWithPlayers(string str);
void writeToFileWithPlayers();
void readFromFileWithPlayers();
void fileWithPlayersCreation();

int numcl = 0;
const int PORT = 1280;
const int BUF_LEN = 256;
const string PATH = "D://My//input.txt";

vector <player> vector_players;

int main()
{
	fileWithPlayersCreation();
	readFromFileWithPlayers();

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cerr << "WSAStartup error: " << err << "\n\n";
		system("pause");
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = 0;

	err = bind(s, (sockaddr*)& local_addr, sizeof(local_addr));
	if (err != 0) {
		cerr << "Bind error: " << err << "\n\n";
		system("pause");
		return -1;
	}

	err = listen(s, 5);
	if (err != 0) {
		cerr << "Listen error: " << err << "\n\n";
		system("pause");
		return -1;
	}
	showPlayersFromFile(vector_players);
	cout << "\nServer's ready to recieve!\n";
	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)
	int client_addr_size = sizeof(client_addr);

	while ((client_socket = accept(s, (sockaddr*)& client_addr, &client_addr_size))) {
		char* connected_ip = inet_ntoa(client_addr.sin_addr);
		cout << "\nClient connected: " << connected_ip << ".\n\n";
		cout << "\nPort number: " << ntohs(client_addr.sin_port);
		numcl++;
		printNumberOfClients();
		// вызов нового потока для обслуживания клиента
		DWORD thID;  //thID идентификатор типа DWORD
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
	return 0;
}

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	cout << "\nThread has been created!\n\n";
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[BUF_LEN], action[2], act, number[5];
	string result_string;
	int bytes_recieved, bytes_send, req_number;
	bool flag = true;
	while (flag)
	{
		ZeroMemory(buf, BUF_LEN);
		bytes_recieved = recv(s2, action, sizeof(action), 0);
		cout << "Bytes recieved: " << bytes_recieved << endl;
		act = action[0];
		switch (act)
		{
		case '1':
			result_string = quest(vector_players);
			for (int i = 0; i != result_string.size(); i++)
				buf[i] = result_string[i];
			bytes_send = send(s2, buf, strlen(buf) + 1, 0);
			cout << "Bytes send: " << bytes_send << endl;
			break;
		case '2':
			result_string = getInfo(vector_players);
			for (int i = 0; i != result_string.size(); i++)
				buf[i] = result_string[i];
			bytes_send = send(s2, buf, strlen(buf) + 1, 0);
			cout << "Bytes send: " << bytes_send << endl;
			break;
		case '3':
		{
			bytes_recieved = recv(s2, buf, sizeof(buf), 0);
			cout << "Bytes recieved: " << bytes_recieved << endl;
			writeToEndOfFileWithPlayers(buf);
			readFromFileWithPlayers();
			cout << "\nPlayer info has been added!\n";
		}
			break;
		case '4':
			snprintf(number, sizeof(number), "%d", vector_players.size());
			send(s2, number, strlen(number) + 1, 0);
			result_string = getInfo(vector_players);
			for (int i = 0; i != result_string.size(); i++)
				buf[i] = result_string[i];
			bytes_send = send(s2, buf, strlen(buf) + 1, 0);
			cout << "Bytes send: " << bytes_send << endl;
			ZeroMemory(number, 5);
			if (vector_players.size() != 0)
			{
				recv(s2, number, sizeof(number), 0);
				req_number = atoi(number) - 1;
				recv(s2, action, sizeof(action), 0);
				act = action[0];
				switch (act)
				{
				case '1':
					recv(s2, buf, sizeof(buf), 0);
					vector_players.at(req_number).name = buf;
					break;
				case '2':
					recv(s2, buf, sizeof(buf), 0);
					vector_players.at(req_number).number = atoi(buf);
					break;
				case '3':
					recv(s2, buf, sizeof(buf), 0);
					vector_players.at(req_number).age = atoi(buf);
					break;
				case '4':
					recv(s2, buf, sizeof(buf), 0);
					vector_players.at(req_number).height = atoi(buf);
					break;
				case '5':
					recv(s2, buf, sizeof(buf), 0);
					vector_players.at(req_number).weight = atoi(buf);
					break;
				}
			}
			cout << "\nPlayer info has been edited!\n";
			break;
		case '5':
			snprintf(number, sizeof(number), "%d", vector_players.size());
			send(s2, number, strlen(number) + 1, 0);
			result_string = getInfo(vector_players);
			for (int i = 0; i != result_string.size(); i++)
				buf[i] = result_string[i];
			bytes_send = send(s2, buf, strlen(buf) + 1, 0);
			cout << "Bytes send: " << bytes_send << endl;
			ZeroMemory(number, 5);
			if (vector_players.size() != 0)
			{
				recv(s2, number, sizeof(number), 0);
				req_number = atoi(number) - 1;
				vector_players.erase(vector_players.begin() + req_number);
			}
			cout << "\nPlayer info has been deleted!\n";
			break;
		case '0':
			flag = false;
			numcl--;
			cout << "\nClient disconnected!\n";
			printNumberOfClients();
		}
	}
	writeToFileWithPlayers();
	closesocket(s2);
	cout << "\nThread has been finished!\n\n";
	return 0;
}

void showPlayersFromFile(vector <player>& a) {
	cout << "Data is loaded:\n\n";
	cout << "|   Name   |   Number   |   Age   |   Height   |   Weight   |\n";
	for (int i = 0; i < a.size(); i++)
		cout << vector_players.at(i).name << " " << vector_players.at(i).number << " " << vector_players.at(i).age << " " << vector_players.at(i).height << " " << vector_players.at(i).weight << endl;
}

void printNumberOfClients()
{
	if (numcl)
		printf("\nTotal number of clients connected: %d.\n", numcl);
	else
		printf("\nNo clients connected.\n");
}

string quest(vector <player>& a)
{
	int min_index = 0, min_age = a.at(min_index).age;
	for (int i = 1; i != a.size(); i++)
		if (a.at(i).age < min_age)
		{
			min_age = a.at(i).age;
			min_index = i;
		}
	return vector_players.at(min_index).name + " " + to_string(vector_players.at(min_index).number) + " " + to_string(vector_players.at(min_index).age) + " " + to_string(vector_players.at(min_index).height) + " " + to_string(vector_players.at(min_index).weight);
}

string getInfo(vector <player>& a)
{
	string result;
	for (int i = 0; i < vector_players.size(); i++)
		result += (to_string(i + 1) + ". " + vector_players.at(i).name + " " + to_string(vector_players.at(i).number) + " " + to_string(vector_players.at(i).age) + " " += to_string(vector_players.at(i).height) + " " + to_string(vector_players.at(i).weight) + '\n');
	return result;
}

void readFromFileWithPlayers()
{
	vector_players.clear();
	ifstream fin(PATH, ios::in);
	if (fin.eof() != fin.peek() + 1) // Если файл не пуст.
	{
		player player;
		int i = 0;
		while (!fin.eof())
		{
			vector_players.push_back(player);
			fin >> vector_players.at(i).name >> vector_players.at(i).number >> vector_players.at(i).age >> vector_players.at(i).height >> vector_players.at(i).weight;
			i++;
		}
	}
	fin.close();
}

void writeToEndOfFileWithPlayers(string str)
{
	ofstream fout(PATH, ios::app);
	fout << '\n' + str;
	fout.close();
}

void writeToFileWithPlayers()
{
	ofstream fout(PATH, ios::trunc);
	for (int i = 0; i < vector_players.size(); i++)
	{
		fout << vector_players.at(i).name << " " << vector_players.at(i).number << " " << vector_players.at(i).age << " " << vector_players.at(i).height << " " << vector_players.at(i).weight;
		if (i < vector_players.size() - 1)
			fout << endl;
	}
	fout.close();
}

void fileWithPlayersCreation()
{
	ifstream fin(PATH);
	if (!fin.is_open() || fin.eof() == fin.peek() + 1)	//Если файла с игроками не существует,
	{
		ofstream fout(PATH, ios::out); //то создаём его и добавляем данные.
		fout << "George_Lucas" << " " << "1" << " " << "22" << " " << "184" << " " << "66\n"
			<< "Adam_Eevan" << " " << "5" << " " << "26" << " " << "181" << " " << "64\n"
			<< "Jack_Nikolson" << " " << "12" << " " << "19" << " " << "189" << " " << "60\n"
			<< "Normann_Reedus" << " " << "13" << " " << "27" << " " << "185" << " " << "66\n"
			<< "Jakob_O'Lonnie" << " " << "6" << " " << "21" << " " << "177" << " " << "69";
		fout.close();
	}
	else
		fin.close();
}