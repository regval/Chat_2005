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
// ��������, ����������� ��� OnCreate ����
int InputText::OnCreate_it()
{
	// ����� ������������� ������ ���������� ����� � ��������� ��� ��������� �������.
	hdc = GetDC(hWnd_it); 
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
	GetTextMetrics(hdc, &tm); 
	ReleaseDC(hWnd_it, hdc); 
	
	// ���������� ������� ������ � ������ ��������. 
	nCharX = tm.tmAveCharWidth; 
	nCharY = tm.tmHeight; 

	nCaretPosX = 0;	// x-���������� �������
	nCaretPosY = 0;	// y-���������� ������� 

	return 0;
}

// �������� ��� ��������� ������� ����
int InputText::OnSize_it(RECT rctText_)
{
	memcpy (&rctText, &rctText_, sizeof(RECT));
	rctText.left = rctText_.left + 1;
	rctText.top = rctText_.top + 1;
	rctText.right = rctText_.right - 1;
	rctText.bottom = rctText_.bottom - 1;

	// ����������� ������ ������� �������, � ���������� ��������. 
	nWindowCharsX = max(1, (rctText.right - rctText.left)/nCharX);//nWindowX 
	
	// ����������� ������ ������� �������, � ���������� ��������. 
	nWindowCharsY = max(1, (rctText.bottom - rctText.top)/nCharY); //nWindowY
	
#ifdef _INS_INPUT_TEXT_H_
	OnCreate_it();
	// ������� ������, ������� �������� ��������� �����. 
	if (pTextMatrix != NULL) 
		free(pTextMatrix);
	
	// ���� ������� ���������� ������, ������������ ������������ ��� ������ ����� ������.
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

	// �������� ������ ��� ��������� ������.
	pTxtOnly = (char*)malloc(80); 
	if (pTxtOnly == NULL) 
		MessageBox (hWnd_it, "Not enough memory.", "Error", 0); 
	else 
	{
		nTxtSize = 80;
		memset(pTxtOnly, 0, 80);
	}
#endif

	// ���������� ������� � ������ ���������
	SetCaretPos(rctText.left + nCaretPosX , rctText.top + nCaretPosY); 
	return 0;
}

// �������� ��� ������� ������ ����������
int InputText::OnKeyDown_it(WPARAM wParam, LPARAM lParam)
{
	#ifdef _INS_INPUT_TEXT_H_
    	
	switch (wParam) 
	{ 
	case VK_HOME:	// � ������ 
		nCaretPosX = 0; 
		break; 
	case VK_END:	// � ����� 
		nCaretPosX = nWindowCharsX - 1; 
		break; 
	case VK_PRIOR:	// ���������� �������� 
		nCaretPosY = 0; 
		break; 
	case VK_NEXT:	// ��������� �������� 
		nCaretPosY = nWindowCharsY -1; 
		break; 
	case VK_LEFT:	// ����������� �������� ����� 
		nCaretPosX = max(nCaretPosX - 1, 0); 
		break; 
	case VK_RIGHT:	// ����������� �������� ������
		nCaretPosX = min(nCaretPosX + 1, nWindowCharsX - 1); 
		break; 
	case VK_UP:	// ����������� �������� ����� 
		nCaretPosY = max(nCaretPosY - 1, 0); 
		break; 
	case VK_DOWN:	// ����������� �������� ����
		nCaretPosY = min(nCaretPosY + 1, nWindowCharsY - 1); 
		break; 
	case VK_DELETE:	// ������� 
		// ����������� ��� �������, ������� ��������� �� ��������� �������� (�� ��� �� ����� ������) �� ����
		// ������ ������� (�����) � �������
		for (x = nCaretPosX; x < nWindowCharsX; x++) 
			TEXTMATRIX(x, nCaretPosY) = TEXTMATRIX(x + 1, nCaretPosY);
		
		// ������ ���������� ������� � ������ � ��������. 
		TEXTMATRIX(nWindowCharsX - 1, nCaretPosY) = ' '; 
		
		// ���������� ��������� ����� �������� ��� ��������� ���������
		// WM_PAINT, ��� �������� �������. 
		HideCaret(hWnd_it); 
		
		// ����������� ������ ��� ���������� �������. 
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left + nCaretPosX * nCharX, 
			rctText.top + nCaretPosY * nCharY, 
			&TEXTMATRIX(nCaretPosX, nCaretPosY), 
			nWindowCharsX - nCaretPosX); 
		ReleaseDC(hWnd_it, hdc); 
		
		// �������� �������. 
		ShowCaret(hWnd_it); 
		break; 
	} 
#else
	switch (wParam) 
	{ 
	case VK_LEFT:	// ����������� �������� ����� 
		nCaretPosX = max(nCaretPosX - 1, 0); 
		break; 
	case VK_RIGHT:	// ����������� �������� ������
		nCaretPosX = min(nCaretPosX + 1, nTxtLen); 
		break; 
	case VK_DELETE:	// ������� 
		if (nCaretPosX < nTxtLen)
		{
			for (int i = nCaretPosX; i < nTxtLen; ++i)
			{
				*(i + pTxtOnly) = *(i + 1 + pTxtOnly);
			}
			--nTxtLen;
		}


		HideCaret(hWnd_it); 
		// ���������� ������� �� ������
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left, 
			rctText.top, 
			pTxtOnly, nTxtLen); 
		ReleaseDC(hWnd_it, hdc); 
		// ��������� �������
		ShowCaret(hWnd_it); 
		break; 


	} 


#endif
	// �������������� ������� �������, ���������� ��� ��������� ����������� �������
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	return 0;
}
// �������� ��� ������� WM_CHAR
int InputText::OnChar_it(WPARAM wParam, LPARAM lParam)
{
#ifdef _INS_INPUT_TEXT_H_
	switch (wParam) 
	{ 
	case 0x08:	// �������� ������ (Back Space) 
		// ����������� ������� �� ���� ������, � ����� ��������� ����� �������� ������� ������� DEL. 
		if (nCaretPosX > 0) 
		{ 
			nCaretPosX--; 
			SendMessage(hWnd_it, WM_KEYDOWN, VK_DELETE, 1L); 
		} 
		break; 
	case 0x09:	// ��������� (Tab) 
		// ��������� ���������������, ������� ������ ������ �������, ����� ������� ����������� �������, 
		// �� ��� ���, ���� ������������ �������� ��������� ��� �� ������� ���������. 
		do 
		{ 
			SendMessage(hWnd_it, WM_CHAR, ' ', 1L); 
		} while (nCaretPosX % 4 != 0); 
		break; 

	case 0x0D:	// ������� ������� 
		// ������� � ������ ��������� ������. ������ ������ ����������� �� ����� � ������� �����.
		nCaretPosX = 0; 
		if (++nCaretPosY == nWindowCharsY) 
			nCaretPosY = 0; 
		break; 

	case 0x1B:	// Escape 
	case 0x0A:	// ������� ������ 
		MessageBeep((UINT) -1); 
		break; 
	default: 
		// ���������� ������� � ����� ������. 
		TEXTMATRIX(nCaretPosX, nCaretPosY) = (char) wParam; 
		// ���������� ��������� ����� �������� ��� ��������� ��������� WM_PAINT; ��� �������� �������. 
		HideCaret(hWnd_it); 
		// ���������� ������� �� ������
		hdc = GetDC(hWnd_it); 
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
		TextOut(hdc, rctText.left + nCaretPosX * nCharX, 
			rctText.top + nCaretPosY * nCharY, 
			&TEXTMATRIX(nCaretPosX, nCaretPosY), 1); 
		ReleaseDC(hWnd_it, hdc); 
		// ��������� �������
		ShowCaret(hWnd_it); 
		// ���������� �������� �� ������ � ������ �� �����, ���� �� �������� ����� ������
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
	case 0x0D:	// ������� ������� 
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
	// ���������� ������� �� ������
	hdc = GetDC(hWnd_it); 
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); 
	TextOut(hdc, rctText.left, 
		rctText.top, 
		pTxtOnly, nTxtLen); 
	ReleaseDC(hWnd_it, hdc); 
	// ��������� �������
	ShowCaret(hWnd_it); 


#endif
	// ����������� ������� �������, ���������� �� ��������� �������. 
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	return 0;
}
// ��������� ��������� WM_PAINT
int InputText::OnPaint_it(HDC hdc, PAINTSTRUCT ps)
{
	// ���������� ���� �������� � ������, ������ �� �������. 
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

// ��� ��������� ��������� WM_SETFOCUS
int InputText::OnSelFocus_it(void)
{
	
	// ���� ����� ����� �����. �������� ������� ������������ �������� ������������ ����������. 
	//hCaret = LoadBitmap(hinst, MAKEINTRESOURCE(120)); 
	// �������� �������
	CreateCaret(hWnd_it, /*hCaret*/(HBITMAP)NULL, 2, tm.tmHeight); 
	// ����������� ������� �������
	SetCaretPos(rctText.left + nCaretPosX * nCharX, rctText.top + nCaretPosY * nCharY); 
	// ����������� �������. 
	ShowCaret(hWnd_it);

	return 0;
}

// ��� ��������� ������� WM_KILLFOCUS
int InputText::OnKillFocus_it(void)
{
	// ���� ������ ����� �����, ��� ��� ������� ������������. 
	DestroyCaret(); 

	return 0;
}
// ���������� ����� ���������� ������
int InputText::GetSendTextLen(void)
{
#ifdef _INS_INPUT_TEXT_H_
	return nWindowCharsX * nWindowCharsY;
#else
	return nTxtLen;
#endif
}
// �������� ��������� ����� �� ��������� ������ 
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

	// ������� ������ � ������� ������� � ������
	for (y = 0; y < nWindowCharsY; y++) 
		for (x = 0; x < nWindowCharsX; x++) 
			TEXTMATRIX(x, y) = ' '; 
	// ���������� ������� � ������ ���������
	nCaretPosX = 0;	// x-���������� �������
	nCaretPosY = 0;	// y-���������� ������� 
	SetCaretPos(rctText.left + nCaretPosX , rctText.top + nCaretPosY); 
	InvalidateRect(hWnd_it, 0, FALSE);

#else
	strcpy(szSend, pTxtOnly);
#endif

	return 0;
}
