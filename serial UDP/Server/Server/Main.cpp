#pragma comment (lib, "ws2_32.lib")

#include<winsock2.h> 
#include<iostream> 
#include<stdlib.h> 
#include<process.h>

using namespace std;
 
int main() {
	setlocale(LC_ALL, "rus");
	WORD wVersionRequested; 
	WSADATA wsaData;

	int err; 
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
		return 0;

	SOCKET s;
	s = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in ad; 
	ad.sin_port = htons(1024); 
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = 0;

	bind(s, (struct sockaddr*) & ad, sizeof(ad));

	char b[200], tmр = '\0'; 
	int l, len;
	char x[255], y[255], z[255];
	
	while (1)
	{
		l = sizeof(ad);

		cout << "Ожидание установки соединения..." << endl;

		int rv = recvfrom(s, b, sizeof(b), 0, (struct sockaddr*) & ad, &l);	
		b[rv] = 0;
		cout << endl << b << endl;
		
		len = strnlen_s(b, sizeof(b));
		cout << "Длина строки - " << len << endl;

		if (len % 4 == 0) 
		{
			strncpy_s(x, b, len / 2);
			x[len / 2] = '\0';
			int j = len / 2;
			for (int i = 0; i < len; i++)
			{
				y[i] = b[j];
				j++;					
			}
			y[len / 2] = '\0';
			strncpy_s(z, y, len/2);
			strcat_s(z, x);
			z[len] = '\0';			
			strncpy_s(b, z, len);
		}
		sendto(s, b, lstrlen(b), 0, (struct sockaddr*) & ad, l);
	}

	closesocket(s);
	WSACleanup();
	return 0;
}

