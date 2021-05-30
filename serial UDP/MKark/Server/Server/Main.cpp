#pragma comment (lib, "ws2_32.lib") 
#include <iostream> 
#include <winsock2.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <process.h> 
#include <cstring> 
#include <WS2tcpip.h> 


using namespace std;

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		cout << "Can't start Winsock! " << err;
		return -1;
	}
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0); //создание сокета 
	struct sockaddr_in ad;//структура адреса сокета 
	ad.sin_family = AF_INET;
	ad.sin_port = htons(1024);//Поле определяет порт, который будет ассоциирован с сокетом.Функции преобразуют байты. 
	ad.sin_addr.s_addr = 0;//подставляет подходящий iр 
	bind(s, (struct sockaddr*) & ad, sizeof(ad));

	sockaddr_in client; //Информация о клиенте 
	int clientLength = sizeof(client);
	char clientIp[256];
	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);



	char recv_buf[200], send_buf2[200] = "String is less than 15 symbols long.", send_buf3[200];

	while (true)
	{
		ZeroMemory(recv_buf, 200);
		ZeroMemory(send_buf3, 200);
		int l = sizeof(ad);
		cout << "Waiting for a message from a client...\n" << endl;
		int rv = recvfrom(s, recv_buf, sizeof(recv_buf), 0, (sockaddr*)& ad, &l);//принимаем данные через функцию recv. Пока соединение не прервано. 
		if (rv == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}
		cout << "Message recieved from " << clientIp << " : " << recv_buf << endl;
		int length = strlen(recv_buf);
		cout << "Message's length: " << length << endl;
		int counter = 0;
		if (length > 15)
		{
			int i = 0, j = 0;
			for (i; i < length; i++, j++)
				if (recv_buf[i] < '0' || recv_buf[i] > '9')
					send_buf3[j] = recv_buf[i];
				else
				{
					j--;
					for (i; ;)
						if (recv_buf[i] > '0' && recv_buf[i] < '9')
							i++, counter++;
						else
						{
							i--;
							break;
						}
				}

			send_buf3[j] = '\0';
			char temp[20];
			sprintf_s(temp, "%d", counter);
			for (int i = strlen(send_buf3), j = 0; i < strlen(send_buf3) + strlen(temp); i++, j++)
				send_buf3[i] = temp[j];
			sendto(s, send_buf3, strlen(send_buf3) + 1, 0, (sockaddr*)& ad, l);
		}
		else
			sendto(s, send_buf2, strlen(send_buf2) + 1, 0, (sockaddr*)& ad, l);
	}
	closesocket(s);
	WSACleanup();
	return 0;
}