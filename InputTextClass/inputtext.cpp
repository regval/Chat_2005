//#include "StdAfx.h"
#include "inputtext.h"

InputText::InputText()
{
#ifdef _INS_INPUT_TEXT_H_
	pTextMatrix = NULL;
#else
	pTxtOnly = NULL;
	nTxtLen = 0;
	nTxtSize = 0;
#endif //_INS_INPUT_TEXT_H_	

	return;
}
InputText::~InputText()
{

	return;
}
// Действия, выполняемые при OnCreate окна
int InputText::OnCreate_it()
{
	// Выбор фиксированной ширины системного фонта и получение его текстовой матрицы.
	hdc = GetDC(hWnd_it); 
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
	GetTextMetrics(hdc, &tm); 
	ReleaseDC(hWnd_it, hdc); 
	
	// Сохранение средней высоты и ширины символов. 
	nCharX = tm.tmAveCharWidth; 
	nCharY = tm.tmHeight; 

	nCaretPosX = 0;	// x-координата каретки
	nCaretPosY = 0;	// y-координата каретки 

	return 0;
}

// Действия при изменении размера окна
int InputText::OnSize_it(RECT rctText_)
{
	memcpy (&rctText, &rctText_, sizeof(RECT));
	rctText.left = rctText_.left + 1;
	rctText.top = rctText_.top + 1;
	rctText.right = rctText_.right - 1;
	rctText.bottom = rctText_.bottom - 1;

	// Определение ширины рабочей области, в количестве символов. 
	nWindowCharsX = max(1, (rctText.right - rctText.left)/nCharX);//nWindowX 
	
	// Определение высоты рабочей области, в количестве символов. 
	nWindowCharsY = max(1, (rctText.bottom - rctText.top)/nCharY); //nWindowY
	
#ifdef _INS_INPUT_TEXT_H_
	OnCreate_it();
	// Очистка буфера, который содержит введенный текст. 
	if (pTextMatrix != NULL) 
		free(pTextMatrix);
	
	// Если имеется достаточно памяти, распределите пространство для буфера ввода текста.
	pTextMatrix = (char*)malloc(nWindowCharsX * nWindowCharsY); 
	if (pTextMatrix == NULL) 
		MessageBox (hWnd_it, "Not enough memory.", "Error", 0); 
	else 
		for (y = 0; y < nWindowCharsY; y++) 
			for (x = 0; x < nWindowCharsX; x++) 
				TEXTMATRIX(x, y) = ' '; 
#else
	if (pTxtOnly != NULL) 
		free(pTxtOnly); 

	// Создание буфера для текстовой строки.
	pTxtOnly = (char*)malloc(80); 
	if (pTxtOnly == NULL) 
		MessageBox (hWnd_it, "Not enough memory.", "Error", 0); 
	else 
	{
		nTxtSize = 80;
		memset(pTxtOnly, 0, 80);
	}
#endif

	// Переместим каретку в начало координат
	SetCaretPos(rctText.left + nCaretPosX , rctText.top + nCaretPosY); 
	return 0;
}

// Действия при нажатии кнопки клавиатуры
int InputText::OnKeyDown_it(WPARAM wParam, LPARAM lParam)
{
	#ifdef _INS_INPUT_TEXT_H_
    	
	switch (wParam) 
	{ 
	case VK_HOME:	// в начало 
		nCaretPosX = 0; 
		break; 
	case VK_END:	// в конец 
		nCaretPosX = nWindowCharsX - 1; 
		break; 
	case VK_PRIOR:	// предыдущая страница 
		nCaretPosY = 0; 
		break; 
	case VK_NEXT:	// следующая страница 
		nCaretPosY = nWindowCharsY -1; 
		break; 
	case VK_LEFT:	// перемещение стрелкой влево 
		nCaretPosX = max(nCaretPosX - 1, 0); 
		break; 
	case VK_RIGHT:	// перемещение стрелкой вправо
		nCaretPosX = min(nCaretPosX + 1, nWindowCharsX - 1); 
		break; 
	case VK_UP:	// перемещение стрелкой вверх 
		nCaretPosY = max(nCaretPosY - 1, 0); 
		break; 
	case VK_DOWN:	// перемещение стрелкой вниз
		nCaretPosY = min(nCaretPosY + 1, nWindowCharsY - 1); 
		break; 
	case VK_DELETE:	// удалить 
		// Переместите все символы, которые следовали за удаленным символом (на той же самой строке) на один
		// пробел обратно (влево) в матрице
		for (x = nCaretPosX; x < nWindowCharsX; x++) 
			TEXTMATRIX(x, nCaretPosY) = TEXTMATRIX(x + 1, nCaretPosY);
		
		// Замена последнего символа в строке с пробелом. 
		TEXTMATRIX(nWindowCharsX - 1, nCaretPosY) = ' '; 
		
		// Прикладная программа будет рисовать вне обработки сообщения
		// WM_PAINT, так скрывают каретку. 
		HideCaret(hWnd_it); 
		
		// Перерисовка строки для удаленного символа. 
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left + nCaretPosX * nCharX, 
			rctText.top + nCaretPosY * nCharY, 
			&TEXTMATRIX(nCaretPosX, nCaretPosY), 
			nWindowCharsX - nCaretPosX); 
		ReleaseDC(hWnd_it, hdc); 
		
		// Показать каретку. 
		ShowCaret(hWnd_it); 
		break; 
	} 
#else
	switch (wParam) 
	{ 
	case VK_LEFT:	// перемещение стрелкой влево 
		nCaretPosX = max(nCaretPosX - 1, 0); 
		break; 
	case VK_RIGHT:	// перемещение стрелкой вправо
		nCaretPosX = min(nCaretPosX + 1, nTxtLen); 
		break; 
	case VK_DELETE:	// удалить 
		if (nCaretPosX < nTxtLen)
		{
			for (int i = nCaretPosX; i < nTxtLen; ++i)
			{
				*(i + pTxtOnly) = *(i + 1 + pTxtOnly);
			}
			--nTxtLen;
		}


		HideCaret(hWnd_it); 
		// Прорисовка символа на экране
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left, 
			rctText.top, 
			pTxtOnly, nTxtLen); 
		ReleaseDC(hWnd_it, hdc); 
		// Отобразим каретку
		ShowCaret(hWnd_it); 
		break; 


	} 


#endif
	// Корректировать позицию каретки, основанную при обработке виртуальной клавиши
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	return 0;
}
// действия при событии WM_CHAR
int InputText::OnChar_it(WPARAM wParam, LPARAM lParam)
{
#ifdef _INS_INPUT_TEXT_H_
	switch (wParam) 
	{ 
	case 0x08:	// Обратный Пробел (Back Space) 
		// Перемещение каретки на один пробел, а затем обработка этого действия подобно клавише DEL. 
		if (nCaretPosX > 0) 
		{ 
			nCaretPosX--; 
			SendMessage(hWnd_it, WM_KEYDOWN, VK_DELETE, 1L); 
		} 
		break; 
	case 0x09:	// табуляция (Tab) 
		// Табуляция останавливается, проходя каждые четыре пробела, таким образом добавляются пробелы, 
		// до тех пор, пока пользователь нажимает следующий раз на клавишу табуляции. 
		do 
		{ 
			SendMessage(hWnd_it, WM_CHAR, ' ', 1L); 
		} while (nCaretPosX % 4 != 0); 
		break; 

	case 0x0D:	// возврат каретки 
		// Переход к началу следующей строки. Нижняя строка переносится по кругу в верхнюю часть.
		nCaretPosX = 0; 
		if (++nCaretPosY == nWindowCharsY) 
			nCaretPosY = 0; 
		break; 

	case 0x1B:	// Escape 
	case 0x0A:	// перевод строки 
		MessageBeep((UINT) -1); 
		break; 
	default: 
		// Добавление символа в буфер текста. 
		TEXTMATRIX(nCaretPosX, nCaretPosY) = (char) wParam; 
		// Прикладная программа будет рисовать вне обработки сообщения WM_PAINT; так скрывают каретку. 
		HideCaret(hWnd_it); 
		// Прорисовка символа на экране
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left + nCaretPosX * nCharX, 
			rctText.top + nCaretPosY * nCharY, 
			&TEXTMATRIX(nCaretPosX, nCaretPosY), 1); 
		ReleaseDC(hWnd_it, hdc); 
		// Отобразим каретку
		ShowCaret(hWnd_it); 
		// Подготовка переноса по словам с начала до конца, если Вы достигли конца строки
		if (++nCaretPosX == nWindowCharsX) 
		{ 
			nCaretPosX = 0; 
			if (++nCaretPosY == nWindowCharsY) 
				nCaretPosY = 0; 
		} 
		break; 
	} 
#else
	
	switch (wParam) 
	{ 
	case 0x0D:	// возврат каретки 
		if (nTxtLen < nTxtSize)
		{
			if (nCaretPosX < nTxtLen++)
			{
				for (int i = nTxtLen - 1; i > nCaretPosX - 1 ; --i)
				{
					*(i + pTxtOnly) = *(i - 1 + pTxtOnly);
				}
			}
			*(nCaretPosX++ + pTxtOnly) = '\n';
			*(nTxtLen + pTxtOnly) = '/r';
			*(nTxtLen + pTxtOnly + 1) = 0;

			MessageBox(hWnd_it, pTxtOnly, "dedug", 0);
		}
		break; 
	default: 
		if (nTxtLen < nTxtSize)
		{
			if (nCaretPosX < nTxtLen++)
			{
				for (int i = nTxtLen - 1; i > nCaretPosX - 1 ; --i)
				{
					*(i + pTxtOnly) = *(i - 1 + pTxtOnly);
				}
			}
			*(nCaretPosX++ + pTxtOnly) = (char) wParam;
			*(nTxtLen + pTxtOnly) = 0;
		}
		break;}

	HideCaret(hWnd_it); 
	// Прорисовка символа на экране
	hdc = GetDC(hWnd_it); 
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
	TextOut(hdc, rctText.left, 
		rctText.top, 
		pTxtOnly, nTxtLen); 
	ReleaseDC(hWnd_it, hdc); 
	// Отобразим каретку
	ShowCaret(hWnd_it); 


#endif
	// Регулировка позиции каретки, основанной на обработке символа. 
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	return 0;
}
// Обработка сообщения WM_PAINT
int InputText::OnPaint_it(HDC hdc, PAINTSTRUCT ps)
{
	// Прорисовка всех символов в буфере, строка за строкой. 
	//hdc = BeginPaint(hWnd, &ps); 
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
#ifdef _INS_INPUT_TEXT_H_
	for (y = 0; y < nWindowCharsY; y++) 
		TextOut(hdc, rctText.left, rctText.top + y * nCharY, &TEXTMATRIX(0, y), nWindowCharsX); 
#else 
	TextOut(hdc, rctText.left, 
		rctText.top, 
		pTxtOnly, nTxtLen); 
#endif
	//EndPaint(hWnd, &ps); 

	return 0;
}

// при получении сообщения WM_SETFOCUS
int InputText::OnSelFocus_it(void)
{
	
	// Окно имеет фокус ввода. Загрузка каретки определенной ресурсом программного приложения. 
	//hCaret = LoadBitmap(hinst, MAKEINTRESOURCE(120)); 
	// Создание каретки
	CreateCaret(hWnd_it, /*hCaret*/(HBITMAP)NULL, 2, tm.tmHeight); 
	// Регулировка позиции каретки
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	// Изображение каретки. 
	ShowCaret(hWnd_it);

	return 0;
}

// При получении события WM_KILLFOCUS
int InputText::OnKillFocus_it(void)
{
	// Окно теряет фокус ввода, так что каретка уничтожается. 
	DestroyCaret(); 

	return 0;
}
// Возвращает длину текстового буфера
int InputText::GetSendTextLen(void)
{
#ifdef _INS_INPUT_TEXT_H_
	return nWindowCharsX * nWindowCharsY;
#else
	return nTxtLen;
#endif
}
// Копирует текстовый буфер по принятому адресу 
int InputText::GetSendText(char *szSend)
{
#ifdef _INS_INPUT_TEXT_H_
	char *szStr = (char*)malloc(nWindowCharsX * nWindowCharsY);

	//strset(szStr, 0);
	*szStr = '\0';

	int nChY = nWindowCharsY;
	int nChX;

	for (y = 0; y < nWindowCharsY; ++y)
	{
		nChX = nWindowCharsX;
		for (x = nWindowCharsX - 1; x >=0; --x)
		{
			if (TEXTMATRIX(x, y) == ' ')
				--nChX;
			else
				break;
		
		}
		if ((y > 0) && (nChX))
			strcat(szStr, "\r\n");

		strncat(szStr, &(TEXTMATRIX(0, y)), nChX);
	}

	strcpy(szSend, szStr);
	free(szStr);

	// Очистка текста и перевод курсора в начало
	for (y = 0; y < nWindowCharsY; y++) 
		for (x = 0; x < nWindowCharsX; x++) 
			TEXTMATRIX(x, y) = ' '; 
	// Переместим каретку в начало координат
	nCaretPosX = 0;	// x-координата каретки
	nCaretPosY = 0;	// y-координата каретки 
	SetCaretPos(rctText.left + nCaretPosX , rctText.top + nCaretPosY); 
	InvalidateRect(hWnd_it, 0, FALSE);

#else
	strcpy(szSend, pTxtOnly);
#endif

	return 0;
}
