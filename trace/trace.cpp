/*
trace.cpp
============================================================================
���������� �������

Coded by 
����� ������� ���������, 2005, �����-���������
E-mail: Valeriy_Shubin@nwgsm.ru, ���.: +7(921)-984-17-53
============================================================================
*/
//#pragma once
//#include "StdAfx.h"
//#include <windows.h>
#include "trace.h"

double trace(char* msg) 
{
#ifndef _NO_TRACE
	// ��������� �������� �������
	SYSTEMTIME SysTime;	GetLocalTime(&SysTime);
	// ��������� ����� exe - �����
	char szFileName[MAX_PATH]; GetModuleFileName(NULL,szFileName,MAX_PATH);
	// ��������� ����
	char *p = strrchr(szFileName, '\\');   *p = '\0';
	// ���������� ����� trace
	*(long*)p = '\\'| 't' << 8 | 'r' << 16 | 'a' << 24; p+=4;
	*(long*)p = 'c' | 'e' << 8 | '.' << 16 | 't' << 24; p+=4;
	*(long*)p = 'x' | 't' << 8 | '\0'<< 16 | '\0'<< 24; //p+=4;
	// �������� �����
	HANDLE hFile=CreateFile(szFileName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	// �������� ������
	char buf[255];
	itoa((int)SysTime.wDay,   buf, 10); p = strrchr(buf, '\0');*(short*)p = '.'| '\0' << 8;
	itoa((int)SysTime.wMonth, szFileName, 10); strcat(buf, szFileName); p = strrchr(buf, '\0');*(short*)p = ' '| '\0' << 8; 
	itoa((int)SysTime.wHour, szFileName, 10); strcat(buf, szFileName); p = strrchr(buf, '\0');*(short*)p = ':'| '\0' << 8; 
	itoa((int)SysTime.wMinute, szFileName, 10); strcat(buf, szFileName); p = strrchr(buf, '\0');*(short*)p = ':'| '\0' << 8;
	itoa((int)SysTime.wSecond, szFileName, 10); strcat(buf, szFileName); p = strrchr(buf, '\0');*(short*)p = ':'| '\0' << 8;
	itoa((int)SysTime.wMilliseconds, szFileName, 10); strcat(buf, szFileName); p = strrchr(buf, '\0');*(short*)p = ' '| '\0' << 8;
	strcat(buf, msg);
	p = strrchr(buf, '\0');	*(long*)p = '\r'| '\n' << 8 | '\0' << 16 | '\0' << 24; //p+=4;

	// ������� �� ����� �����
	OVERLAPPED olp; memset(&olp, 0, sizeof(OVERLAPPED)); olp.Offset = GetFileSize (hFile, NULL);
	// ������ ������ � ����
	WriteFile(hFile,buf,strlen(buf),0,&olp);
	// �������� �����
	CloseHandle(hFile);

#endif // _NO_TRACE
	return 0;
}