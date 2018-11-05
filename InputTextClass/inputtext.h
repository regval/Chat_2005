#pragma once

#ifndef _INPUTTEXT_H
#define _INPUTTEXT_H

// Указатель на текстовую матрицу из 
#define TEXTMATRIX(x, y) *(pTextMatrix + (y * nWindowCharsX) + x)  
#define _INS_INPUT_TEXT_H_

#include <Windows.h>

class InputText
{
	//
	
	// Глобальные переменные
	HINSTANCE hinst;		// текущий экземпляр программы 
	HBITMAP hCaret;		// точечный рисунок каретки 
	HDC hdc;			// контекст устройства  
	PAINTSTRUCT ps;		// информация о прорисовке рабочей области 
	char *pTextMatrix;	// указатель на текстовую матрицу 

	char *pTxtOnly; // Указатель на текстовую строку
	int nTxtLen, nTxtSize; // Количество символов в строке, размер строки
	int
		nCharX,		// ширина символа в логических единицах 
		nCharY,		// высота символа в логических единицах 
		//nWindowX,		// ширина рабочей области 
		//nWindowY,		// высота рабочей области 
		nWindowCharsX,	// ширина рабочей области в символах 
		nWindowCharsY,	// высота рабочей области в символах 
		nCaretPosX,	// x-координата каретки
		nCaretPosY;	// y-координата каретки 
	RECT rctText; // Диапазон рабочей области

	static UINT uOldBlink;	// предыдущая частота мерцания 
	int x, y;			// x и y координаты, используемые в текстовой матрице 
	TEXTMETRIC tm;		// информация о фонте 

public:


	InputText();
	~InputText();

	//*******************************************************************

	// HWND окна
	HWND hWnd_it;

	// Действия, выполняемые при OnCreate окна
	int OnCreate_it();
	// Действия при изменении размера окна
	int OnSize_it(RECT rctText);
	// Действия при нажатии кнопки клавиатуры
	int OnKeyDown_it(WPARAM wParam, LPARAM lParam);
	// действия при событии WM_CHAR
	int OnChar_it(WPARAM wParam, LPARAM lParam);
	// Обработка сообщения WM_PAINT
	int OnPaint_it(HDC hdc, PAINTSTRUCT ps);
	// при получении сообщения WM_SETFOCUS
	int OnSelFocus_it(void);
	// При получении события WM_KILLFOCUS
	int OnKillFocus_it(void);

	// Возвращает длину текстового буфера
	int GetSendTextLen(void);
	// Копирует текстовый буфер по принятому адресу 
	int GetSendText(char *);
};

#endif // _INPUTTEXT_H
