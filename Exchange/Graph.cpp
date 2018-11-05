/*
Graph.cpp
============================================================================
���� ���������� ������� ������ ��������

Coded by 
����� ������� ���������, 2004, 2005, �����-���������
E-mail: Valeriy_Shubin@nwgsm.ru, ���.: +7(921)-984-17-53
============================================================================
*/
//#include "StdAfx.h"

#include ".\graph.h"
#include <stdio.h>
#include <math.h>

Graph::Graph(void)
{
	IsRing = false;

	dIn_MinVal = 0;
	dIn_MaxVal = 0;

	bIn_IsZoomX = false;
	bIn_IsZoomY = false;

	nIn_BeginData = 0;

	bIsZoomActive = false;
	memset(&ptPrevMove, 0, sizeof(RECT));

	bIsAutoZOOM = false;
	
	nOut_BeginZOOM = 0;
	nOut_AmountZOOM = 0;

}

Graph::~Graph(void)
{
}

// ���������� ��� ������������ �������
int Graph::InitGraph(HWND hWndOwnerDld_, int nIDDlgGraph)
{
	hWndOwnerDld = hWndOwnerDld_;
	hWndGraf = GetDlgItem(hWndOwnerDld, nIDDlgGraph);// ��������� ����������� ���� ���������
	GetClientRect(hWndGraf, &rct);// �������� �������� ���� ���������
	hDC = GetDC(hWndGraf);// ��������� ��������� ���� ��� ���������

	hDCMem = CreateCompatibleDC (hDC);// �������� ������������ ��������� ��� ���������
	// �������� ������� ������� ��� ���������
	HBITMAP hbmMem = CreateCompatibleBitmap(hDC, rct.right, rct.bottom);

	SelectObject(hDCMem, hbmMem);// ����������� ������� ������� 

	DeleteObject(hbmMem);// �������� ������� �������

	FillRect(hDCMem, &rct, (HBRUSH)RGB(0, 0, 255)); //(COLOR_ACTIVECAPTION+1))

	// ��������� ������� ������� ������� ���������� ������� �������
	ptGrafOnDlg.x = 0; ptGrafOnDlg.y = 0;
	MapWindowPoints(hWndGraf, hWndOwnerDld , &ptGrafOnDlg, 1 );

	return 0;
}

// // ���������� ��� ������������� ������� WM_PAINT
int Graph::OnPaint(void)
{
	BitBlt(
		hDC,
		0, 
		0, 
		rct.right, 
		rct.bottom,
		hDCMem, 
		0, 
		0, 
		SRCCOPY); 

	return 0;
}

// ���������� ��� ������������� ������� WM_DESTROY
int Graph::DestroyGraph(void)
{
	// �������� ����������� ���������� ���������
	DeleteDC(hDCMem);
	ReleaseDC(hWndGraf, hDC);// ������������ ��������� ��������� ��� ���������

	return 0;
}

// ��� ������� ��������� WM_LBUTTONDOWN
int Graph::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	// ��������� ��������� ����
	ptBegin.x=LOWORD(lParam) - ptGrafOnDlg.x;
	ptBegin.y=HIWORD(lParam) - ptGrafOnDlg.y;

	// ������������� �� ��� ������� ����� �������
	if (ptBegin.x > rct.left && ptBegin.x < rct.right &&
		ptBegin.y > rct.top && ptBegin.y < rct.bottom)
	{
		bIsZoomActive = true;	// �������������. ��� ���

		// ��������� ������� ��������� ���������
		ptPrevMove.left = ptBegin.x;
		ptPrevMove.top = ptBegin.y;
		ptPrevMove.right = ptBegin.x;
		ptPrevMove.bottom = ptBegin.y;
	}
	return 0;
}
// ��� ������� ��������� WM_MOUSEMOVE
int Graph::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	// ��������� ��������� ����
	POINTS ptEnd;
	ptEnd.x=LOWORD(lParam) - ptGrafOnDlg.x;
	ptEnd.y=HIWORD(lParam) - ptGrafOnDlg.y;

	// ����������� �� ���, ������ �� ����� ������ ����,
	// ������������� �� ��� ������� ����� �������
	if (bIsZoomActive && wParam == MK_LBUTTON &&
		ptEnd.x > rct.left && ptEnd.x < rct.right &&
		ptEnd.y > rct.top && ptEnd.y < rct.bottom)
	{
		// ��������� ���������� ������
		SetROP2(hDCMem, R2_NOTXORPEN);

		// ���� ���� ���������� ��������� - ������� �����
		if (ptPrevMove.left)
		{
			MoveToEx(hDCMem, ptPrevMove.right, ptPrevMove.top, NULL);
			LineTo(hDCMem, ptPrevMove.left, ptPrevMove.top);
			LineTo(hDCMem, ptPrevMove.left, ptPrevMove.bottom);
			LineTo(hDCMem, ptPrevMove.right, ptPrevMove.bottom);
			LineTo(hDCMem, ptPrevMove.right, ptPrevMove.top);
		}

		// ��������� ����� �����
		ptPrevMove.left = ptBegin.x;
		ptPrevMove.top = ptBegin.y;
		ptPrevMove.right = ptEnd.x;
		ptPrevMove.bottom = ptEnd.y;

		// ���������� �����
		MoveToEx(hDCMem, ptPrevMove.right, ptPrevMove.top, NULL);
		LineTo(hDCMem, ptPrevMove.left, ptPrevMove.top);
		LineTo(hDCMem, ptPrevMove.left, ptPrevMove.bottom);
		LineTo(hDCMem, ptPrevMove.right, ptPrevMove.bottom);
		LineTo(hDCMem, ptPrevMove.right, ptPrevMove.top);

		// ������ ���������� ������
		SetROP2(hDCMem, R2_COPYPEN);

		OnPaint();
	}
	return 0;
}
// ��� ������� ��������� WM_LBUTTONUP
int Graph::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	// ��������� ��������� ����
	POINTS ptEnd;
	ptEnd.x=LOWORD(lParam) - ptGrafOnDlg.x;
	ptEnd.y=HIWORD(lParam) - ptGrafOnDlg.y;

	// ���� ���� ���������� ��������� - ������� �����
	if (ptPrevMove.left)
	{
		SetROP2(hDCMem, R2_NOTXORPEN);
		MoveToEx(hDCMem, ptPrevMove.right, ptPrevMove.top, NULL);
		LineTo(hDCMem, ptPrevMove.left, ptPrevMove.top);
		LineTo(hDCMem, ptPrevMove.left, ptPrevMove.bottom);
		LineTo(hDCMem, ptPrevMove.right, ptPrevMove.bottom);
		LineTo(hDCMem, ptPrevMove.right, ptPrevMove.top);
		SetROP2(hDCMem, R2_COPYPEN);
		OnPaint();
	}
	
	// ����������� �� ���
	// ������������� �� ��� ������� ����� �������
	if (bIsZoomActive &&
		ptEnd.x > rct.left && ptEnd.x < rct.right &&
		ptEnd.y > rct.top && ptEnd.y < rct.bottom)
	{

		// ��������� ������� ����
		ptPrevMove.left = ptBegin.x;
		ptPrevMove.top = ptBegin.y;
		ptPrevMove.right = ptEnd.x;// - ptBegin.x;
		ptPrevMove.bottom = ptEnd.y;// - ptBegin.y;

		// �������� ������� ���� (��������)
		// FillRect(hDCMem, &ptPrevMove, (HBRUSH)RGB(0, 0, 255));
		bIsAutoZOOM = true;	// ������� ������ ������� � ������ ZOOM
		rctZOOM = ptPrevMove;	// ���������� ZOOM � ��������
	}				

	// ������ ����� ����������� ��������� �����
	ptPrevMove.left = 0;

	// ���������� ����
	bIsZoomActive = false;

	return 0;
}

// ��� ������� ��������� WM_LBUTTONDBLCLK
int Graph::OnLButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	bIsAutoZOOM = false;
	return 0;
}



int Graph::DisplayMaxVal()
{
	LOGFONT lf; 
	LOGBRUSH lb; 
	HPEN hPen, hPenOld; 

	COLORREF clrPrevText, clrPrevBkgnd;
	HFONT hfont, hFontOld;

	lb.lbStyle = BS_SOLID; 
	lb.lbColor = RGB(255,0,0); 
	lb.lbHatch = 0; 

	hPen = ExtCreatePen(PS_COSMETIC | PS_SOLID, 
		1, 
		&lb, 
		0, 
		NULL); 
	hPenOld = (HPEN)SelectObject(hDCMem, hPen); 

	if (fabs( -dOut_Max) > 1000.)
		sprintf(szStr, "%2.2e", dOut_Max);
	else
		sprintf(szStr, "%f", dOut_Max);

	lf.lfHeight = 0; 
	lf.lfWidth = 0; 
	lf.lfEscapement = 0; 
	lf.lfOrientation= 180; 
	lf.lfWeight = 300; 
	lf.lfItalic = FALSE; 
	lf.lfUnderline = FALSE; 
	lf.lfStrikeOut = FALSE; 
	lf.lfCharSet = DEFAULT_CHARSET ; 
	lf.lfOutPrecision = OUT_CHARACTER_PRECIS; 
	lf.lfClipPrecision = CLIP_CHARACTER_PRECIS; 
	lf.lfQuality = DEFAULT_QUALITY; 
	lf.lfPitchAndFamily = NULL;//DEFAULT_PITCH | FF_DECORATIVE; 
	strcpy(lf.lfFaceName, "TimeNewRoman"); 

	hfont = CreateFont(15,
		0,
		0,
		0,
		FW_ULTRALIGHT,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL); 

	clrPrevText = SetTextColor(hDCMem, RGB(255, 0, 0)); 
	clrPrevBkgnd = SetBkColor(hDCMem, RGB(0, 0, 255));

	hFontOld = (HFONT)SelectObject(hDCMem, (HGDIOBJ)hfont);

	//TextOut(hDCMem, (int)(IndGrafYmin / Kover) - 26, rct.top + 1, szStr,strlen(szStr));

	SetTextColor(hDCMem, clrPrevText); 
	SetBkColor(hDCMem, clrPrevBkgnd); 

	SelectObject(hDCMem, hFontOld);
	DeleteObject(hfont);

	SelectObject(hDCMem, hPenOld); 
	DeleteObject(hPen); 
	
	return 0;
}

// ���������� ������
int Graph::DisplayGraphFloat(int nAmount, float* pfData)
{
	if (bIsZoomActive)	return -100;
	
	double dKover;
	float Kgr = 1; // �������� ��� ����� ��������� ��������� float
	
	double GrafYmax = 0, GrafYmin = 0;
	int IndGrafYmax = 0, IndGrafYmin = 0;

	// ����� � ������ ������� ������
	int WndYmax = rct.bottom - rct.top;
	int WndYmin = rct.right - rct.left;
		
	// ���������� ����� ������� � ����� ������� ������ (�� �)
	dKover = ((double)(nAmount - 1) / (double)(rct.right - rct.left));
	
	// !!!!!  ����������� ����� ��������� ����� ��� ZOOM �� �  !!!!!
	if (bIsAutoZOOM)
	{
		nAmount = (rctZOOM.right - rctZOOM.left) * dKover;
		nIn_BeginData = rctZOOM.left * dKover;

		nOut_BeginZOOM = nIn_BeginData;
		nOut_AmountZOOM = nAmount;

			// ���������� ����� ������� � ����� ������� ������ (�� �)
		dKover = ((double)(nAmount - 1) / (double)(rct.right - rct.left));
	}
	else
	{
		nOut_BeginZOOM = 0;
		nOut_AmountZOOM = 0;
	}

	// �������� ������� ������
	FillRect(hDCMem, &rct, (HBRUSH)RGB(0, 0, 255)); //(COLOR_ACTIVECAPTION+1))

	// ���������� �������
	MoveToEx(hDCMem, rct.right, rct.top, NULL);
	LineTo(hDCMem, rct.left, rct.top);
	LineTo(hDCMem, rct.left, rct.bottom);
	LineTo(hDCMem, rct.right, rct.bottom);
	LineTo(hDCMem, rct.right, rct.top);


	// ����������� ������������ � ������������� ��������
	// ���������������
	for (int i = 0; i < nAmount; ++i)
	{
		pfData[i] = pfData[i + nIn_BeginData] * Kgr * (-1);
		
		if(GrafYmax < pfData[i]){GrafYmax = pfData[i];}//IndGrafYmax = i;
		if(GrafYmin > pfData[i]){GrafYmin = pfData[i];IndGrafYmin = i;}
	}
	float nMin = (float)GrafYmax, nMax = (float)GrafYmin;

	// !!!!!  ��������� ������ ZOOM �� �  !!!!!
	if (bIn_IsZoomY)
	{
		GrafYmax = dIn_MaxVal;
		GrafYmin = dIn_MinVal;
	}
	
	dOut_Max = GrafYmin / Kgr * -1.0;

	int nKover;

	if (dKover > 1)
	{
		nKover = (int)ceil(dKover);
		int j;
		for (int i = 0; i < nAmount; ++i)
		{
			// ���� ���������� �������� IsRing, ��������������� ������ �� �
			j = (IsRing)?((i + nAmount / 2) % nAmount):(i);

			if(nMax < pfData[j]){nMax = pfData[j];}
			if(nMin > pfData[j]){nMin = pfData[j];}

			if (i%nKover == 0)
			{
				float X =  ((float)i / (float)nKover);
				int Wmax = (int)((nMax - GrafYmin) / ((GrafYmax - GrafYmin) / (float)WndYmax));
				int Wmin = (int)((nMin - GrafYmin) / ((GrafYmax - GrafYmin) / (float)WndYmax));

				if (i == 0) MoveToEx(hDCMem, (int)X, Wmin, NULL);
				LineTo(hDCMem, (int)X, Wmin);
				LineTo(hDCMem, (int)X, Wmax); 

				nMin = (float)GrafYmax, nMax = (float)GrafYmin;
			}
		}
	}
	else
	{
		nKover = (int) (1. / dKover);

		for (int i = 0; i < nAmount; ++i)
		{
			if (i == 0) MoveToEx(hDCMem, 0, (int)((pfData[0] - GrafYmin) / ((GrafYmax - GrafYmin) / (float)WndYmax)), NULL);
			LineTo(hDCMem, i * nKover, (int)((pfData[i] - GrafYmin) / ((GrafYmax - GrafYmin) / (float)WndYmax)));

			nMin = (float)GrafYmax, nMax = (float)GrafYmin;
		}
		
	}


	//OnPaint();

	IsRing = false;

	return 0;
}

//LRESULT CALLBACK	DlgFunc(HWND hWndOwnerDld, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId;	// ���������� ������� 
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		break;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return TRUE;
//		break;
//	case WM_CLOSE:
//		DestroyWindow(hWndOwnerDld);
//		return TRUE;
//		break;
//
//	case WM_PAINT:
//		PAINTSTRUCT ps;
//		// ���������� ���� ��������� ����
//		BeginPaint(hWndOwnerDld, &ps); 
//		EndPaint(hWndOwnerDld, &ps);
//		return TRUE;
//		break;
//	}
//	return FALSE;
//}
int Graph::DisplayDialogZoom(void)
{
	DLGTEMPLATE dpl;

	dpl.style = 
		DS_SETFONT |
		DS_MODALFRAME | 
		DS_FIXEDSYS | 
		WS_POPUP | 
		WS_CAPTION | 
		WS_SYSMENU;	//DWORD
	dpl.dwExtendedStyle;	//DWORD
	dpl.cdit = 0;	//WORD
	dpl.x = 0;	//short
	dpl.y = 0;	//short
	dpl.cx = 100;	//short
	dpl.cy = 100;	//short
//HWND hZoomWnd;
	// = CreateDialogIndirectParam(
	//	0,
	//	&dpl,
	//	0,
	//	(DLGPROC) DlgFunc,
	//	(LPARAM) 0
	//	);

	//ShowWindow(hZoomWnd, 1);
	//UpdateWindow(hZoomWnd);

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0)) 
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	//delete pCW;


	return 0;
}