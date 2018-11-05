#pragma once

#include "..\SockNet\SockNet.h"
#include "..\inputtextclass\inputtext.h"

#include "View.h"



class View_class: public SockNet, public InputText
{
public:
	int nBorderSmall; // ������ ����������
	float fPersentEdit; // ������� ������� ��������������

	HWND hWndHist;
	//HWND hWndEdit;

private:

	RECT rctHist;
	RECT rctEdit;

	// * ��������� ����������� ������� ��� ��������� * //
	HBRUSH brBlack;
	HBRUSH brWhite;

	POINT pnWnd; // ������ ���������� �������
	
	//RECT rctClient; // ������� ������ ��������� �������
	//RECT rctGrafik; // ������� ������� ��� ��������


public:
	View_class(HWND hWnd_);
	~View_class(void);

	void SetNewVersion();

	// ��� ��������� �������
	void OnResize(WPARAM wParam, LPARAM lParam);

private:
	// // �������� ��������� ��������� 
	void InitControls(void);
public:
	// // ��������� �������� ��������� ����������
	void ResizeControl(void);
	// // ��� ��;���� Send
	int OnSend(void);
	// ������� �������� ����������
	void DeleteControls(void);

	bool bIsInit;

	// ��������� WM_PAINT
	int OnPaint(HDC hdc, PAINTSTRUCT ps);
};
