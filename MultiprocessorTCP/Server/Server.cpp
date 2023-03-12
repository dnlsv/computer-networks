#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int N = 5;

struct Employee
{
	char name[35];
	char number[10];
	char marks[5];
};

// процедура для обслуживания соединения
int Func(int newS, Employee *em)
{
	int  num;
	char buf[256];
	while (true)
	{
		recv(newS, buf, sizeof(buf), 0);
		char p = buf[0];
		cout << p << endl;
		switch (p)
		{
		case '1':
		{	buf[0] = '\0';
		recv(newS, buf, sizeof(buf), 0);
		for (int i = 0; i < N; i++)
		{
			if (strcmp(buf, em[i].name) == 0)
			{
				swap(em[i].name, em[N].name);
				swap(em[i].number, em[N].number);
				swap(em[i].marks, em[N].marks);
				N--;
				send(newS, "1\0", sizeof("1\0"), 0);
				break;
			}
			if (i == N)
			{
				send(newS, "0\0", sizeof("0\0"), 0);
			}
		}
		}
		break;
		case '2':
		{	recv(newS, buf, sizeof(buf), 0);  //Номер
		num = atoi(buf);
		cout << num << endl;
			recv(newS, buf, sizeof(buf), 0);
		char p1 = buf[0];
		switch (p1)
		{
		case '1':
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[num].name, buf);
			break;
		case '2':
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[num].marks, buf);
			break;
		default: break;
		}
		}
		break;
		case '3':
		{	for (int i = 0; i < N; i++)
		{
			buf[0] = '\0';
			strcat(buf, em[i].name); strcat(buf, " ");
			strcat(buf, em[i].number); strcat(buf, " ");
			strcat(buf, em[i].marks); strcat(buf, "\n");
			send(newS, buf, sizeof(buf), 0);
		}
		}
		break;
		case '4':
		{
			recv(newS, buf, sizeof(buf), 0);
			for (int i = 0; i < N; i++)
			{
				if (strcmp(buf, em[i].name) == 0)
				{
					strcat(buf, em[i].name); strcat(buf, " ");
					strcat(buf, em[i].number); strcat(buf, " ");
					strcat(buf, em[i].marks); strcat(buf, "\n");
				}
			}
			send(newS, buf, sizeof(buf), 0);
		}
		break;
		case '5':
		{
			N++;
			Employee *tmp = new Employee[N];
			for (int i = 0; i < N; i++)
			{
				strcpy(tmp[i].name, em[i].name);
				strcpy(tmp[i].number, em[i].number);
				strcpy(tmp[i].marks, em[i].marks);
			}
			recv(newS, buf, sizeof(buf), 0);
			int i, j = 0;
			for (i = 0; i < 256; i++)
			{
				if (buf[i] == ' ')
				{
					j++;
				}
				if (j == 3)
				{
					tmp[N].name[i - 1] = '\0';
					break;
				}
				tmp[N - 1].name[i] = buf[i];
			}
			for (i = i + 1; buf[i] != ' '; i++)
			{
				int j = 0;
				tmp[N - 1].number[j] = buf[i];
				j++;
			}
			for (i = i + 1; buf[i] != '\0'; i++)
			{
				int j = 0;
				tmp[N - 1].marks[j] = buf[i];
				j++;
			}
			delete[]em;
			em = tmp;
		}
		break;
		case '0':
			delete[]em;
			exit(0);
		}
	}
}

int main()
{
	Employee *em = new Employee[N];

	strcpy(em[0].name, "Lisenkov Denis Yrievich");
	strcpy(em[0].number, "1");
	strcpy(em[0].marks, "9");

	strcpy(em[1].name, "Konoplianik Mark Valer'evich");
	strcpy(em[1].number, "2");
	strcpy(em[2].marks, "8");

	strcpy(em[2].name, "Karachyn Egor Igorevich");
	strcpy(em[2].number, "3");
	strcpy(em[2].marks, "4");

	strcpy(em[3].name, "Nagiev Dmitrii Vladimirovich");
	strcpy(em[3].number, "4");
	strcpy(em[3].marks, "10");

	strcpy(em[4].name, "Kantish Vitalii Sergeevich");
	strcpy(em[4].number, "5");
	strcpy(em[4].marks, "6");

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(7500);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	int s = socket(AF_INET, SOCK_STREAM, 0);
	bind(s, (struct sockaddr *)&local, sizeof(local));
	listen(s, 5);
	//(void)signal(SIGCHLD,reaper);
	while (true)
	{
		int newS = accept(s, NULL, NULL);
		switch (fork())
		{
		case 0:
			(void)close(s);
			exit(Func(newS, em));
		default:
			(void)close(newS);
		}
	}
	delete[]em;
	return 0;
}
