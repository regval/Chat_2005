#ifndef SOCK_NET_H
#define SOCK_NET_H

#include <stdio.h>
#include <WinSock2.h>

#include "..\Exchange\Exchange.h"



class SockNet: public Exchange
{
public:
	SockNet(void);
	~SockNet(void);

	int NetProc();

	char szDSP_name[255];
	int nListenPort;
	int nSendPort;

	char szKey[80];

private:
	WSADATA wsaData;
	
	SOCKET listet_Sock;		// Создание сокета

	HANDLE hThread;
	DWORD dwThreadId;




	int nRes;
	char type[15];
	int nSizeOfBuffer;			// Размер посылаемого буфера

	bool isExitConnect;
	bool isExitResive;
public:
	// // Создание переменной параметров (лок адр) и привязка к сокету
	int BindAddr(void);
	// Открытие порта для соединения
	int Listen(void);

	// Отправка по сети
	int SendBack(char *szFlag,
					char *szBuffer,
					int SizeOfBuffer);
	// Константы ключей, принимаемых в программе.
	char outText[15];
	char keyExit[15];
	char keyNewV[15];
};

UINT SockServer( LPVOID param );

#endif