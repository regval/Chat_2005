//#include "StdAfx.h"
#include "SockNet.h"

//#include "..\Exchange\Exchange.h"

UINT SockServer( LPVOID param )
{
	int nA = ((SockNet*) param)->NetProc();

	return nA;
}

//Код шифрующей функции 
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
	*p = '\0'; // Теперь в szNewPath записан путь к файлу

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

	// Константы ключей, принимаемых в программе.
	strcpy(outText, "Output____Text");
	strcpy(keyNewV, "Input_New_Vers");
	strcpy(keyExit, "Program___Exit");

	// Инициализация интерфейса работы с сокетами	
	nRes = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (nRes != 0)
	{
		return;
	}
	// Создание сокета
	listet_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listet_Sock == 0)
	{
		return;
	}

	return;
}
SockNet::~SockNet()
{
	// Удаление сокета
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
		AddHist("Вышли из цикла с кодом 100.");

	char szMsg[80];
	sprintf(szMsg, "Время ожидания = %d", a);
	AddHist(szMsg);
	
	// Освобождение ресурсов интерфейса сокетов
	WSACleanup();

	return;
}

// // Создание переменной параметров (лок адр) и привязка к сокету
int SockNet::BindAddr(void)
{
	// Создание переменной параметров (локальный адрес)
	SOCKADDR_IN addr_Sock;

	addr_Sock.sin_family = AF_INET;					// тип адр, принят в Интернет
	addr_Sock.sin_addr.s_addr = htonl(INADDR_ANY);	// ждем коннекта с любого адр.
	addr_Sock.sin_port = htons(nListenPort);				// номер порта, котор слушаем

	// Привязка адреса к сокету
	nRes = bind(listet_Sock,
		(LPSOCKADDR)&addr_Sock,
		sizeof(struct sockaddr));
	if (nRes)	
	{
		AddHist("Не удалось настроить работу с портом.");
		AddHist("nListenPort");
		return -1;
	}

	return 0;
}

// Открытие порта для соединения
int SockNet::Listen(void)
{
	// Открытие порта для соединения
	if (listen(listet_Sock, 1))	
	{
		AddHist("Не удалось открыть порт.");
		return -1;
	}

	// Запуск сервера приема сообщений
	hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)SockServer,
		(LPVOID)this,
		0,
		&dwThreadId);
	
	return 0;
}



// // Запуск сервера

int SockNet::NetProc()
{
	// Цикл ожидания коннекта

	while (!isExitConnect) // Цикл ожидания коннекта
	{
		// Ожидание соединения
		SOCKET cycle_Sock = accept(listet_Sock, 0, 0);
		if (cycle_Sock == INVALID_SOCKET)
		{
			// Освобождение ресурсов сокета коннекта
			shutdown(cycle_Sock, 1);
			closesocket(cycle_Sock);
			
			return 2;
		}

		// Соединение устанолено
		// Цикл обработки команд
		while (!isExitResive)	
		{
			char szTmp[20];
			// Принятие команды
			nRes = recv(cycle_Sock, type, 15, 0);
			if (nRes == SOCKET_ERROR)
			{
				sprintf(szTmp, "Ошибка приема данных при принятии команды № %d", WSAGetLastError());
				AddHist(szTmp);
			}
			if ((nRes == SOCKET_ERROR) || (nRes == 0))	break;

			// Принятие размера следующего за командой буфера
			nRes = recv(cycle_Sock, (char*)&nSizeOfBuffer, 4, 0);
			if (nRes == SOCKET_ERROR)
			{
				sprintf(szTmp, "Ошибка приема данных при принятии размера буфера № %d", WSAGetLastError());
				AddHist(szTmp);
			}
			if ((nRes == SOCKET_ERROR) )	break;

			// Принятая команда - текстовое сообщение
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
			// Принятая команда - новая версия
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

			// Принятая команда - Выход
			if (!strncmp(type, keyExit, 15))
			{
				// Sleep(100);				// Подождать гарантированного завершения
				isExitConnect = true;
				isExitResive = true;
			}
		}

		// Освобождение ресурсов сокета коннекта
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
	// Создание сокета
	SOCKET client_Sock = socket(AF_INET, SOCK_STREAM, 0);

	// Создание переменной параметров (локальный адрес)
	SOCKADDR_IN dest_sin;

	dest_sin.sin_family = AF_INET;					// тип адр, принят в Интернет

	PHOSTENT phe = gethostbyname(szDSP_name); // VALERA
	if (phe == NULL)	
	{
		closesocket(client_Sock);
		AddHist("Ошибка функции gethostbyname");
		return -1;
	}
	memcpy((char* FAR *)&dest_sin.sin_addr,
		phe->h_addr, 
		phe->h_length);/**/

	//dest_sin.sin_addr.s_addr = inet_addr("192.168.0.150");	// адрес192.168.14.83

	dest_sin.sin_port = htons(nSendPort);				// номер порта, котор слушаем

	int nRes = 0;
	nRes = connect(client_Sock, (PSOCKADDR)&dest_sin, sizeof(dest_sin));
	if (nRes < 0)
	{
		closesocket(client_Sock);
		AddHist("Ошибка функции connect");
		return -1;
	}
	

	int nNumChar;
	nNumChar = send(client_Sock, szFlag, 15, 0);
	if (nNumChar == SOCKET_ERROR)
	{
		sprintf(szTmp, "Ошибка передачи команды № %d", WSAGetLastError());
		AddHist(szTmp);
	}
	nNumChar = send(client_Sock, (char*)&SizeOfBuffer, 4, 0);
	if (nNumChar == SOCKET_ERROR)
	{
		sprintf(szTmp, "Ошибка передачи размера буфера № %d", WSAGetLastError());
		AddHist(szTmp);
	}

	if (SizeOfBuffer) 
	{
		char *szShifr;
		// Шифрование
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
			sprintf(szTmp, "Ошибка передачи буфера № %d", WSAGetLastError());
			AddHist(szTmp);
		}
		// Шифрование
		if (!strncmp(szFlag, outText, 15))
		{
			delete [] szShifr;

			//code(szBuffer, SizeOfBuffer, szKey, strlen(szKey));
		}
	}

	// Удаление сокета
	shutdown(client_Sock, 1);
	nRes = closesocket(client_Sock);

	return 0;
}


