//#include "StdAfx.h"
#include "SockNet.h"

//#include "..\Exchange\Exchange.h"

UINT SockServer( LPVOID param )
{
	int nA = ((SockNet*) param)->NetProc();

	return nA;
}

//��� ��������� ������� 
void code(char *in, char *out, int szin,char *key,int szkey)
{
	if (in == 0){return;}
	for(int i = 0,t = 0; i < szin; out[i++] = in[i] ^ key[t++]) if(t >= szkey) t = 0;
}

int CopyNewVersion(void * Data, int nLen)
{
	int nres;
	char szPath[MAX_PATH];
	char *p;

	GetModuleFileName(NULL,szPath,MAX_PATH);
	p = strrchr(szPath, '\\');
	*p = '\0'; // ������ � szNewPath ������� ���� � �����

	strcat(szPath, "\\NewVers.exe");

	remove(szPath);

	FILE * fNV;
	fNV = fopen(szPath, "wb");
	nres = fwrite(Data, nLen, 1, fNV);
	nres = fclose(fNV);

	return 0;
}

SockNet::SockNet()
{
	nRes = 0;
	isExitConnect = false;
	isExitResive = false;

	nLenHist = 0;
	*szHistory = 0;

	// ��������� ������, ����������� � ���������.
	strcpy(outText, "Output____Text");
	strcpy(keyNewV, "Input_New_Vers");
	strcpy(keyExit, "Program___Exit");

	// ������������� ���������� ������ � ��������	
	nRes = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (nRes != 0)
	{
		return;
	}
	// �������� ������
	listet_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listet_Sock == 0)
	{
		return;
	}

	return;
}
SockNet::~SockNet()
{
	// �������� ������
	shutdown(listet_Sock, 1);
	nRes = closesocket(listet_Sock);

	DWORD ExitCode;
	int a = 0;
	//do {
		Sleep(100);
		GetExitCodeThread(hThread, &ExitCode);
		if (ExitCode == STILL_ACTIVE)//(++a > 10) 
			TerminateThread(hThread,0);
	//} while(ExitCode == STILL_ACTIVE);
	
	if (ExitCode == 100)
		AddHist("����� �� ����� � ����� 100.");

	char szMsg[80];
	sprintf(szMsg, "����� �������� = %d", a);
	AddHist(szMsg);
	
	// ������������ �������� ���������� �������
	WSACleanup();

	return;
}

// // �������� ���������� ���������� (��� ���) � �������� � ������
int SockNet::BindAddr(void)
{
	// �������� ���������� ���������� (��������� �����)
	SOCKADDR_IN addr_Sock;

	addr_Sock.sin_family = AF_INET;					// ��� ���, ������ � ��������
	addr_Sock.sin_addr.s_addr = htonl(INADDR_ANY);	// ���� �������� � ������ ���.
	addr_Sock.sin_port = htons(nListenPort);				// ����� �����, ����� �������

	// �������� ������ � ������
	nRes = bind(listet_Sock,
		(LPSOCKADDR)&addr_Sock,
		sizeof(struct sockaddr));
	if (nRes)	
	{
		AddHist("�� ������� ��������� ������ � ������.");
		AddHist("nListenPort");
		return -1;
	}

	return 0;
}

// �������� ����� ��� ����������
int SockNet::Listen(void)
{
	// �������� ����� ��� ����������
	if (listen(listet_Sock, 1))	
	{
		AddHist("�� ������� ������� ����.");
		return -1;
	}

	// ������ ������� ������ ���������
	hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)SockServer,
		(LPVOID)this,
		0,
		&dwThreadId);
	
	return 0;
}



// // ������ �������

int SockNet::NetProc()
{
	// ���� �������� ��������

	while (!isExitConnect) // ���� �������� ��������
	{
		// �������� ����������
		SOCKET cycle_Sock = accept(listet_Sock, 0, 0);
		if (cycle_Sock == INVALID_SOCKET)
		{
			// ������������ �������� ������ ��������
			shutdown(cycle_Sock, 1);
			closesocket(cycle_Sock);
			
			return 2;
		}

		// ���������� ����������
		// ���� ��������� ������
		while (!isExitResive)	
		{
			char szTmp[20];
			// �������� �������
			nRes = recv(cycle_Sock, type, 15, 0);
			if (nRes == SOCKET_ERROR)
			{
				sprintf(szTmp, "������ ������ ������ ��� �������� ������� � %d", WSAGetLastError());
				AddHist(szTmp);
			}
			if ((nRes == SOCKET_ERROR) || (nRes == 0))	break;

			// �������� ������� ���������� �� �������� ������
			nRes = recv(cycle_Sock, (char*)&nSizeOfBuffer, 4, 0);
			if (nRes == SOCKET_ERROR)
			{
				sprintf(szTmp, "������ ������ ������ ��� �������� ������� ������ � %d", WSAGetLastError());
				AddHist(szTmp);
			}
			if ((nRes == SOCKET_ERROR) )	break;

			// �������� ������� - ��������� ���������
			if (!strncmp(type, outText, 15))	
			{	
				if (!nSizeOfBuffer)	break;
				char *szResiv = new char [nSizeOfBuffer];

				nRes = recv(cycle_Sock, szResiv, nSizeOfBuffer, 0);
				if ((nRes == SOCKET_ERROR) || (nRes == 0))	break;

				code(szResiv, szResiv, nSizeOfBuffer, szKey, strlen(szKey));

				AddHist(szResiv, ">", 2);
				SetWindowText(hWndMain_net, szResiv);
				if (bIsTray())
					ShowWindow(hWndMain_net, SW_RESTORE);
				
				FlashWindow(hWndMain_net,TRUE);

				delete [] szResiv;
			}
			// �������� ������� - ����� ������
			if (!strncmp(type, keyNewV, 15))	
			{	
				if (!nSizeOfBuffer)	break;
				char *szResiv = new char [nSizeOfBuffer];

				int nNbyte = 0;
				do 
				{
					nRes = recv(cycle_Sock,
						(char*)(szResiv + nNbyte), 
						nSizeOfBuffer - nNbyte,
						0);
					nNbyte += nRes;
					if ((nRes == SOCKET_ERROR) || (nRes == 0))	break;
				} while(nNbyte < nSizeOfBuffer);

				CopyNewVersion((void*)szResiv, nSizeOfBuffer);

				delete [] szResiv;
				

				SendMessage(hWnd, WM_USER + 256, 0, 0);
			}

			// �������� ������� - �����
			if (!strncmp(type, keyExit, 15))
			{
				// Sleep(100);				// ��������� ���������������� ����������
				isExitConnect = true;
				isExitResive = true;
			}
		}

		// ������������ �������� ������ ��������
		shutdown(cycle_Sock, 1);
		closesocket(cycle_Sock);
	}
	return 0;
}
int SockNet::SendBack(char *szFlag,
					char *szBuffer,
					int SizeOfBuffer)
{
	char szTmp[100];
	// �������� ������
	SOCKET client_Sock = socket(AF_INET, SOCK_STREAM, 0);

	// �������� ���������� ���������� (��������� �����)
	SOCKADDR_IN dest_sin;

	dest_sin.sin_family = AF_INET;					// ��� ���, ������ � ��������

	PHOSTENT phe = gethostbyname(szDSP_name); // VALERA
	if (phe == NULL)	
	{
		closesocket(client_Sock);
		AddHist("������ ������� gethostbyname");
		return -1;
	}
	memcpy((char* FAR *)&dest_sin.sin_addr,
		phe->h_addr, 
		phe->h_length);/**/

	//dest_sin.sin_addr.s_addr = inet_addr("192.168.0.150");	// �����192.168.14.83

	dest_sin.sin_port = htons(nSendPort);				// ����� �����, ����� �������

	int nRes = 0;
	nRes = connect(client_Sock, (PSOCKADDR)&dest_sin, sizeof(dest_sin));
	if (nRes < 0)
	{
		closesocket(client_Sock);
		AddHist("������ ������� connect");
		return -1;
	}
	

	int nNumChar;
	nNumChar = send(client_Sock, szFlag, 15, 0);
	if (nNumChar == SOCKET_ERROR)
	{
		sprintf(szTmp, "������ �������� ������� � %d", WSAGetLastError());
		AddHist(szTmp);
	}
	nNumChar = send(client_Sock, (char*)&SizeOfBuffer, 4, 0);
	if (nNumChar == SOCKET_ERROR)
	{
		sprintf(szTmp, "������ �������� ������� ������ � %d", WSAGetLastError());
		AddHist(szTmp);
	}

	if (SizeOfBuffer) 
	{
		char *szShifr;
		// ����������
		if (!strncmp(szFlag, outText, 15))
		{
			if (strlen(szBuffer) < SizeOfBuffer) SizeOfBuffer = strlen(szBuffer)+1;
			szShifr = new char [SizeOfBuffer];

			code(szBuffer,szShifr, SizeOfBuffer, szKey, strlen(szKey));

			nNumChar = send(client_Sock, szShifr, SizeOfBuffer, 0);

		}
		else 
		{
			nNumChar = send(client_Sock, szBuffer, SizeOfBuffer, 0);
		}
		
		//nNumChar = send(client_Sock, szShifr, SizeOfBuffer, 0);
		if (nNumChar == SOCKET_ERROR)
		{
			sprintf(szTmp, "������ �������� ������ � %d", WSAGetLastError());
			AddHist(szTmp);
		}
		// ����������
		if (!strncmp(szFlag, outText, 15))
		{
			delete [] szShifr;

			//code(szBuffer, SizeOfBuffer, szKey, strlen(szKey));
		}
	}

	// �������� ������
	shutdown(client_Sock, 1);
	nRes = closesocket(client_Sock);

	return 0;
}


