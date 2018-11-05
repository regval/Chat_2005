/*
Exchange.h
============================================================================
Файл объявления базового класса окна диалога

Coded by 
ШУБИН ВАЛЕРИЙ СЕРГЕЕВИЧ, 2005, Санкт-Петербург
E-mail: Valeriy_Shubin@nwgsm.ru, тел.: +7(921)-984-17-53
============================================================================
*/

#pragma once

#ifndef EXCHANGE_INTERFACE_FROM_SHUBIN_H
#define EXCHANGE_INTERFACE_FROM_SHUBIN_H

#include <windows.h>
#include "graph.h"
#include "Tray.h"

// История сообщений
#define LEN_HIST_MAX 10000

//Длина строк раздела и имени в Ини файле
#define EX_LEN_NM 32

#define EXCHANGE_INT 1
#define EXCHANGE_FLOAT 2
#define EXCHANGE_UINT 3
#define EXCHANGE_USHORT 4
#define EXCHANGE_SZ 5
#define EXCHANGE_BOOL_CHECK 6
#define EXCHANGE_GRAPH 7
#define EXCHANGE_SZ_MULTY 8

// Конвертирование имен файлов из MS-формата в массив и обратно
int ConvertFileNamesFromMS(int *nCount,		// Указатель на число файлов
						   char *szMSFiles,	// Указатель на архив имен файлов
						   char *szPath,		// Указатель на директорию
						   char *szFiles,		// Двумерный массив имен файлов ВТОРАЯ РАЗМЕРНОСТЬ - MAX_PATH
						   int nCountMax = 0 );	// Если 0 - From MS, != 0 - N_files_max

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
		void* pValue,				// Указатель на переменную
		unsigned char ucType,		// Тип переменной
		int nIDDlgItem,				// Идентификатор переменной в окне(для отображаемых)
		bool bIsSaveToIni = false,	// Признак записи в Ини файл
		char *lpAppName = 0,		// Имя раздела в Ини файле
		char *lpKeyName = 0,		// Имя переменной в Ини файл
		unsigned int unLength = 0	// Длина буфера переменной (для текстовых),

		);

	ValueList *pLastValue;
	void DeleteValueList();

	void UpdateData(bool fromWnd = true, int nIDDlgItem = 0);
	void UpdateDataIni(bool fromFile);


	void GetIniFileName(char* szIniFile){strncpy(szIniFile, szIniFileName, MAX_PATH);};

	// При создании диалога свойств, для внесение в его поля текущих величин
	//void InitOptionsBox(HWND hDlg);
	
	void OnInitApp(HWND hWnd);
	void OnSize(WPARAM wParam);
	void OnDestroyApp(void);
	void OnPaint(void);

	void OnNotifyTray(LPARAM lParam); 
	void OnCommand(WPARAM wParam);

	// При приходе сообщения WM_LBUTTONDOWN
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_MOUSEMOVE
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_LBUTTONUP
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_LBUTTONDBLCLK
	int OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	
	// При создании диалога свойств
	int OnInitOptDlg(HWND hDlg);

	// История сообщений
	// добаляет сообщение в поле истории
	int AddHist(char* szTxt, char *szExTxt = NULL, int nTimeMode = 1);// nTimeMode: 1-полный, 2-только время
	char szHistory[LEN_HIST_MAX];	// Текст истории сообщений
	int nLenHist;					// Число знаков в History
	HWND hWndHist_net;				// HWND элемента цправления истории
	HWND hWndMain_net;

#ifdef _exchange_use_tray
	bool bIsTray();				// Находится ли программа в Трее
#endif
private:
	// Имя ini файла для значений переменных
	char szIniFileName[MAX_PATH];
#ifdef _exchange_use_tray
	Tray tray;
#endif

};


#endif // EXCHANGE_INTERFACE_FROM_SHUBIN_H