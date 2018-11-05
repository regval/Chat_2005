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
	
	SOCKET listet_Sock;		// �������� ������

	HANDLE hThread;
	DWORD dwThreadId;




	int nRes;
	char type[15];
	int nSizeOfBuffer;			// ������ ����������� ������

	bool isExitConnect;
	bool isExitResive;
public:
	// // �������� ���������� ���������� (��� ���) � �������� � ������
	int BindAddr(void);
	// �������� ����� ��� ����������
	int Listen(void);

	// �������� �� ����
	int SendBack(char *szFlag,
					char *szBuffer,
					int SizeOfBuffer);
	// ��������� ������, ����������� � ���������.
	char outText[15];
	char keyExit[15];
	char keyNewV[15];
};

UINT SockServer( LPVOID param );

#endif