/*
Exchange.h
============================================================================
���� ���������� �������� ������ ���� �������

Coded by 
����� ������� ���������, 2005, �����-���������
E-mail: Valeriy_Shubin@nwgsm.ru, ���.: +7(921)-984-17-53
============================================================================
*/

#pragma once

#ifndef EXCHANGE_INTERFACE_FROM_SHUBIN_H
#define EXCHANGE_INTERFACE_FROM_SHUBIN_H

#include <windows.h>
#include "graph.h"
#include "Tray.h"

// ������� ���������
#define LEN_HIST_MAX 10000

//����� ����� ������� � ����� � ��� �����
#define EX_LEN_NM 32

#define EXCHANGE_INT 1
#define EXCHANGE_FLOAT 2
#define EXCHANGE_UINT 3
#define EXCHANGE_USHORT 4
#define EXCHANGE_SZ 5
#define EXCHANGE_BOOL_CHECK 6
#define EXCHANGE_GRAPH 7
#define EXCHANGE_SZ_MULTY 8

// ��������������� ���� ������ �� MS-������� � ������ � �������
int ConvertFileNamesFromMS(int *nCount,		// ��������� �� ����� ������
						   char *szMSFiles,	// ��������� �� ����� ���� ������
						   char *szPath,		// ��������� �� ����������
						   char *szFiles,		// ��������� ������ ���� ������ ������ ����������� - MAX_PATH
						   int nCountMax = 0 );	// ���� 0 - From MS, != 0 - N_files_max

#define _exchange_use_tray

// *************************************************************************
struct ValueList
{
	ValueList(ValueList* pPrev)	{pPrevious = pPrev;}
	ValueList* pPrevious;

	void *pValue;
	unsigned char ucType;
	unsigned int unLength;

	int nIDDlgItem;	

	bool bIsSaveToIni;
	char lpAppName[EX_LEN_NM+1];
	char lpKeyName[EX_LEN_NM+1];
};
// ************************************************************************
class Exchange
{
public:
	HINSTANCE hInstance;
	HWND hWnd;
	HWND hDlgOpt;

	Exchange(void);
	~Exchange(void);
	void AddValue(
		void* pValue,				// ��������� �� ����������
		unsigned char ucType,		// ��� ����������
		int nIDDlgItem,				// ������������� ���������� � ����(��� ������������)
		bool bIsSaveToIni = false,	// ������� ������ � ��� ����
		char *lpAppName = 0,		// ��� ������� � ��� �����
		char *lpKeyName = 0,		// ��� ���������� � ��� ����
		unsigned int unLength = 0	// ����� ������ ���������� (��� ���������),

		);

	ValueList *pLastValue;
	void DeleteValueList();

	void UpdateData(bool fromWnd = true, int nIDDlgItem = 0);
	void UpdateDataIni(bool fromFile);


	void GetIniFileName(char* szIniFile){strncpy(szIniFile, szIniFileName, MAX_PATH);};

	// ��� �������� ������� �������, ��� �������� � ��� ���� ������� �������
	//void InitOptionsBox(HWND hDlg);
	
	void OnInitApp(HWND hWnd);
	void OnSize(WPARAM wParam);
	void OnDestroyApp(void);
	void OnPaint(void);

	void OnNotifyTray(LPARAM lParam); 
	void OnCommand(WPARAM wParam);

	// ��� ������� ��������� WM_LBUTTONDOWN
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_MOUSEMOVE
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_LBUTTONUP
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_LBUTTONDBLCLK
	int OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	
	// ��� �������� ������� �������
	int OnInitOptDlg(HWND hDlg);

	// ������� ���������
	// �������� ��������� � ���� �������
	int AddHist(char* szTxt, char *szExTxt = NULL, int nTimeMode = 1);// nTimeMode: 1-������, 2-������ �����
	char szHistory[LEN_HIST_MAX];	// ����� ������� ���������
	int nLenHist;					// ����� ������ � History
	HWND hWndHist_net;				// HWND �������� ���������� �������
	HWND hWndMain_net;

#ifdef _exchange_use_tray
	bool bIsTray();				// ��������� �� ��������� � ����
#endif
private:
	// ��� ini ����� ��� �������� ����������
	char szIniFileName[MAX_PATH];
#ifdef _exchange_use_tray
	Tray tray;
#endif

};


#endif // EXCHANGE_INTERFACE_FROM_SHUBIN_H