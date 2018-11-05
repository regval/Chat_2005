/*
Graph.h
============================================================================
���� ���������� ������� ������ ��������

Coded by 
����� ������� ���������, 2004, �����-���������
E-mail: Valeriy_Shubin@nwgsm.ru, ���.: +7(921)-984-17-53
============================================================================
*/

#pragma once
#include <windows.h>

class Graph
{
	HWND hWndOwnerDld;		// ���� ���� �������
	HWND hWndGraf;		// ���� ���������
	
	// ������ ����� ���
	HDC hDCMem;		// ����������� �������� ��� ���������
	RECT rct;		// ������� ������� �������(�����������)
	HDC hDC;		// �������� ���������� ������ �������

	// ��� ������ ��������� ������
	char szStr[50];
	int DisplayMaxVal();

	// ���������� ZOOM
	POINT ptGrafOnDlg;	// ��������� ������� ������� �� ����� �������
	
	POINTS ptBegin;	// �������������� ��������� ���� ��� �������
	RECT ptPrevMove; // ������������� ��������� ��� �������
	bool bIsZoomActive;	// ������� ������ ���������

	bool bIsAutoZOOM;	// ������� ������ ������� � ������ ZOOM
	RECT rctZOOM;		// ���������� ZOOM � ��������

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
	
	bool IsRing;	// ������� ������������ ������ � �������������� ��������

	int nOut_BeginZOOM;
	int nOut_AmountZOOM;


	// ���������� ��� ������������ �������
	int InitGraph(HWND hDlg, int nIDDlgGraph);
	// ���������� ��� ������������� ������� WM_PAINT
	int OnPaint(void);
	// ��� ������� ��������� WM_LBUTTONDOWN
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_MOUSEMOVE
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_LBUTTONUP
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	// ��� ������� ��������� WM_LBUTTONDBLCLK
	int OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	// ���������� ��� ������������� ������� WM_DESTROY
	int DestroyGraph(void);
	// ���������� ������
	int DisplayGraphFloat(int nAmount, float* pfData);

	int DisplayDialogZoom(void);
};
