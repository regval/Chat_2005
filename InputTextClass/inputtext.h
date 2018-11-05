#pragma once

#ifndef _INPUTTEXT_H
#define _INPUTTEXT_H

// ��������� �� ��������� ������� �� 
#define TEXTMATRIX(x, y) *(pTextMatrix + (y * nWindowCharsX) + x)  
#define _INS_INPUT_TEXT_H_

#include <Windows.h>

class InputText
{
	//
	
	// ���������� ����������
	HINSTANCE hinst;		// ������� ��������� ��������� 
	HBITMAP hCaret;		// �������� ������� ������� 
	HDC hdc;			// �������� ����������  
	PAINTSTRUCT ps;		// ���������� � ���������� ������� ������� 
	char *pTextMatrix;	// ��������� �� ��������� ������� 

	char *pTxtOnly; // ��������� �� ��������� ������
	int nTxtLen, nTxtSize; // ���������� �������� � ������, ������ ������
	int
		nCharX,		// ������ ������� � ���������� �������� 
		nCharY,		// ������ ������� � ���������� �������� 
		//nWindowX,		// ������ ������� ������� 
		//nWindowY,		// ������ ������� ������� 
		nWindowCharsX,	// ������ ������� ������� � �������� 
		nWindowCharsY,	// ������ ������� ������� � �������� 
		nCaretPosX,	// x-���������� �������
		nCaretPosY;	// y-���������� ������� 
	RECT rctText; // �������� ������� �������

	static UINT uOldBlink;	// ���������� ������� �������� 
	int x, y;			// x � y ����������, ������������ � ��������� ������� 
	TEXTMETRIC tm;		// ���������� � ����� 

public:


	InputText();
	~InputText();

	//*******************************************************************

	// HWND ����
	HWND hWnd_it;

	// ��������, ����������� ��� OnCreate ����
	int OnCreate_it();
	// �������� ��� ��������� ������� ����
	int OnSize_it(RECT rctText);
	// �������� ��� ������� ������ ����������
	int OnKeyDown_it(WPARAM wParam, LPARAM lParam);
	// �������� ��� ������� WM_CHAR
	int OnChar_it(WPARAM wParam, LPARAM lParam);
	// ��������� ��������� WM_PAINT
	int OnPaint_it(HDC hdc, PAINTSTRUCT ps);
	// ��� ��������� ��������� WM_SETFOCUS
	int OnSelFocus_it(void);
	// ��� ��������� ������� WM_KILLFOCUS
	int OnKillFocus_it(void);

	// ���������� ����� ���������� ������
	int GetSendTextLen(void);
	// �������� ��������� ����� �� ��������� ������ 
	int GetSendText(char *);
};

#endif // _INPUTTEXT_H
