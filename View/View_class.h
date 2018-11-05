#pragma once

#include "..\SockNet\SockNet.h"
#include "..\inputtextclass\inputtext.h"

#include "View.h"



class View_class: public SockNet, public InputText
{
public:
	int nBorderSmall; // Размер бордюрчика
	float fPersentEdit; // Процент области редактирования

	HWND hWndHist;
	//HWND hWndEdit;

private:

	RECT rctHist;
	RECT rctEdit;

	// * Параметры виртуальной области для рисования * //
	HBRUSH brBlack;
	HBRUSH brWhite;

	POINT pnWnd; // Размер клиентской области
	
	//RECT rctClient; // Размеры честно поделеной области
	//RECT rctGrafik; // Размеры области для графиков


public:
	View_class(HWND hWnd_);
	~View_class(void);

	void SetNewVersion();

	// при изменении размера
	void OnResize(WPARAM wParam, LPARAM lParam);

private:
	// // Создание начальных элементов 
	void InitControls(void);
public:
	// // Изменение размеров элементов управления
	void ResizeControl(void);
	// // При на;атии Send
	int OnSend(void);
	// Удаляет элементы управления
	void DeleteControls(void);

	bool bIsInit;

	// Обработка WM_PAINT
	int OnPaint(HDC hdc, PAINTSTRUCT ps);
};
