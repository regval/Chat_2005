/*
Graph.h
============================================================================
Файл объявления функций вывода графиков

Coded by 
ШУБИН ВАЛЕРИЙ СЕРГЕЕВИЧ, 2004, Санкт-Петербург
E-mail: Valeriy_Shubin@nwgsm.ru, тел.: +7(921)-984-17-53
============================================================================
*/

#pragma once
#include <windows.h>

class Graph
{
	HWND hWndOwnerDld;		// Весь блок диалога
	HWND hWndGraf;		// Окно рисования
	
	// График всего ВКФ
	HDC hDCMem;		// Виртуальный контекст для рисования
	RECT rct;		// Размеры области графика(копирования)
	HDC hDC;		// Контекст устройства вывода графика

	// Для вывода текстовой строки
	char szStr[50];
	int DisplayMaxVal();

	// Реализация ZOOM
	POINT ptGrafOnDlg;	// Положение области графика на блоке диалога
	
	POINTS ptBegin;	// Первоначальное положение мыши при нажатии
	RECT ptPrevMove; // Промежуточное положение при ведении
	bool bIsZoomActive;	// Признак начала выделения

	bool bIsAutoZOOM;	// Признак вывода графика в режиме ZOOM
	RECT rctZOOM;		// Координаты ZOOM в пикселях

public:
	Graph(void);
	~Graph(void);

	double dOut_Max;

	bool bIn_IsZoomY;
	double dIn_MinVal;
	double dIn_MaxVal;

	bool bIn_IsZoomX;
	int nIn_BeginData;
	int nIn_Amount;
	
	bool IsRing;	// Признак перестановкм данных в корреляционных функциях

	int nOut_BeginZOOM;
	int nOut_AmountZOOM;


	// Вызывается при инициализции диалога
	int InitGraph(HWND hDlg, int nIDDlgGraph);
	// Вызывается при возникновении события WM_PAINT
	int OnPaint(void);
	// При приходе сообщения WM_LBUTTONDOWN
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_MOUSEMOVE
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_LBUTTONUP
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	// При приходе сообщения WM_LBUTTONDBLCLK
	int OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	// Вызывается при возникновении события WM_DESTROY
	int DestroyGraph(void);
	// Отобразить график
	int DisplayGraphFloat(int nAmount, float* pfData);

	int DisplayDialogZoom(void);
};
