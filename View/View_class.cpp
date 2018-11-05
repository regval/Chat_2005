//#include "StdAfx.h"
#include ".\view_class.h"
#include <shellapi.h>
#include <strsafe.h>

#include <stdio.h>

View_class::View_class(HWND hWnd_)
{
	hWnd = hWnd_;
	hWndMain_net = hWnd_;
	hWnd_it = hWnd_;

	AddValue(&nBorderSmall,EXCHANGE_INT,IDC_BORDER, true, "App", "Размер бордюра");
	AddValue(&fPersentEdit,EXCHANGE_FLOAT,IDC_PEREDIT, true, "App", "Процент области редактирования");

	AddValue(&szDSP_name,EXCHANGE_SZ,IDC_COMP_NAME, true, "Net", "Имя компьютера", MAX_PATH);
	AddValue(&nListenPort,EXCHANGE_INT,IDC_PORT_INPUT, true, "Net", "Порт приема");
	AddValue(&nSendPort,EXCHANGE_INT,IDC_PORT_OUTPUT, true, "Net", "Порт передачи");

	AddValue(&szKey, EXCHANGE_SZ, IDC_KEY, true, "Protect", "Password", 80);

	//IDC_COMP_NAME

	OnInitApp(hWnd);

	// ***** Подготовка основных кистей для рисования
	brWhite = CreateSolidBrush(RGB(255,255,255));
	brBlack = CreateSolidBrush(RGB(0,0,0));
	// *****

	InitControls();

	// Начальные действия для ввода текста
	OnCreate_it();

	BindAddr();
	Listen();

	if (SendBack(outText, " *** ДОБРО ПОЖАЛОВАТЬ! *** ",28) == NULL)
		AddHist(" *** ДОБРО ПОЖАЛОВАТЬ! *** ", "<", 1);
	else
		AddHist(" *** ДОБРО ПОЖАЛОВАТЬ! *** ", "~", 1);

	//char szPath[MAX_PATH];
	//GetModuleFileName(NULL,szPath,MAX_PATH);
	//AddHist(szPath);

	//AddHist(" НОВАЯ ВЕРСИЯ ", "~", 1);
}

View_class::~View_class(void)
{
	if (SendBack(outText, " *** ВСЕГО ДОБРОГО! *** ",25) == NULL)
		AddHist(" *** ВСЕГО ДОБРОГО! *** ", "<", 1);
	else
		AddHist(" *** ВСЕГО ДОБРОГО! *** ", "~", 1);
		
	DeleteControls();

	// ***** Удаление основных кистей для рисования
	DeleteObject(brWhite);
	DeleteObject(brBlack);
	// *****

	OnDestroyApp();

}

//#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

int CopyNewVersion(void * Data, int nLen);
void ModelNewVer()
{

		//	// Запись новой версии программы
		//	CopyNewVersion(Data, nSizeFile);

		//// Копирование 

	//}
}



void View_class::SetNewVersion()
{
	FILE * fONV;
	int nSizeFile = 0;
	int nres;

	// Определение имени файла
	char szPath[MAX_PATH];
	GetModuleFileName(NULL,szPath,MAX_PATH);

	// открытие файла для передачи
	if ((fONV = fopen(szPath, "rb")) != NULL)
	{
		// Определение размера файла
		nres = fseek(fONV, 0, SEEK_END);
		nSizeFile = ftell(fONV);
		nres = fseek(fONV, 0, SEEK_SET);

		// Выделение памяти
		void * Data = malloc(nSizeFile); 
		if (Data != NULL) 
		{
			// считывание файла в буфер
			nres = fread(Data, sizeof( char ), nSizeFile, fONV);
		}		
		// Закрытие файла
		nres = fclose(fONV);
		
		if (SendBack(keyNewV, (char*)Data,nSizeFile) == NULL)
			AddHist("", ":)", 2);
		else
			AddHist("", ":(", 2);

		// Освобождение буфера
		free(Data);
	}

	return;
}

// при изменении размера
void View_class::OnResize(WPARAM wParam, LPARAM lParam)
{
	if (lParam != -1)
	{
		pnWnd.x = LOWORD(lParam); 
		pnWnd.y = HIWORD(lParam); 
	}

	int nHist = pnWnd.y - (int)((float)pnWnd.y * fPersentEdit / 100.0);

	rctHist.top = nBorderSmall;
	rctHist.left = nBorderSmall;
	rctHist.right = -2 * nBorderSmall + pnWnd.x;
	rctHist.bottom = -2 * nBorderSmall + nHist;

	rctEdit.top = nHist;
	rctEdit.left = nBorderSmall;
	rctEdit.right = -nBorderSmall + pnWnd.x;
	rctEdit.bottom = -nBorderSmall + pnWnd.y;// - nHist;
	
	OnSize_it(rctEdit);

	ResizeControl();

	OnSize( wParam);
}

// // Создание начальных элементов 
void View_class::InitControls(void)
{
	char name[25];

	strcpy(name, "History");
	hWndHist = CreateWindow(
		"EDIT",
		name, 
		ES_AUTOHSCROLL | ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
		rctHist.left,
		rctHist.top,
		rctHist.right,
		rctHist.bottom,
		hWnd, 
		NULL, 
		NULL, 
		NULL);

	hWndHist_net = hWndHist;

	return;
}

// // Изменение размеров элементов управления
void View_class::ResizeControl(void)
{
	SetWindowPos(
		hWndHist,
		HWND_TOP,
		rctHist.left,
		rctHist.top,
		rctHist.right,
		rctHist.bottom,
		NULL);
}

// При на;атии Send
int View_class::OnSend(void)
{
	int Len = GetSendTextLen();

	char *szM = new char [Len+1];

	GetSendText(szM);
	if (SendBack(outText, szM,strlen(szM)+1) == NULL)
		AddHist(szM, "<", 2);
	else
		AddHist(szM, "~", 2);

	delete [] szM;

	return 0;
}

// Удаляет элементы управления
void View_class::DeleteControls(void)
{
	DestroyWindow(hWndHist);
}


// Обработка WM_PAINT
int View_class::OnPaint(HDC hdc, PAINTSTRUCT ps)
{
	OnPaint_it(hdc, ps);

	POINT pPoint;   // old current position

	// Рисуем рамку
	MoveToEx(hdc, rctEdit.left, rctEdit.top, &pPoint);
	LineTo(hdc,	rctEdit.right,	rctEdit.top);
	LineTo(hdc,	rctEdit.right,	rctEdit.bottom);
	LineTo(hdc,	rctEdit.left,	rctEdit.bottom);
	LineTo(hdc,	rctEdit.left,	rctEdit.top);
	
	return 0;
}
