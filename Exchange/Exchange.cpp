/*
Exchange.cpp
============================================================================
Файл объявления базового класса окна диалога

Coded by 
ШУБИН ВАЛЕРИЙ СЕРГЕЕВИЧ, 2005, Санкт-Петербург
E-mail: Valeriy_Shubin@nwgsm.ru, тел.: +7(921)-984-17-53
============================================================================
*/

//#include "StdAfx.h"
#include ".\exchange.h"

#include <stdio.h>
//#include <string.h>
Exchange::Exchange(void)
{
	// Указатель на первый элемент
	pLastValue = 0;

	// Формирование имени ini файла
	GetModuleFileName(NULL,szIniFileName,MAX_PATH);
	char *p = strrchr(szIniFileName, '.');	*p = '\0';
	*(long*)p = '.' | 'i' << 8 | 'n' << 16 | 'i' << 24;
}

Exchange::~Exchange(void)
{
	DeleteValueList();// Удаление связанного списка объектов
	tray.OnDestroyTray();               //<<< Clean Tray related before close the app.
}

//---- OnInitDialog - должна выполнятся при инициализации окна диалога
void Exchange::OnInitApp(HWND hWnd_)
{
	hWnd = hWnd_;	// Сохранение HWND

	UpdateDataIni(true); // Считывание данных из файла
	//UpdateData(false); // Заполнение элементов полей диалога

	// Инициализация графиков
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if  (EXCHANGE_GRAPH == pLastTmpInt->ucType)	((Graph*)pLastTmpInt->pValue)->InitGraph(hWnd, pLastTmpInt->nIDDlgItem);

		pLastTmpInt = pLastTmpInt->pPrevious;
	};
	tray.TRAY_Init(hWnd);//<<< Create tray icon and popup menu 
}
// При создании диалога свойств
int Exchange::OnInitOptDlg(HWND hDlg)
{
	hDlgOpt = hDlg;
	this->UpdateData (false); // Заполнение элементов полей диалога

	return 0;
}

//---- OnDestroy - должна выполнятся при закрытии программы
void Exchange::OnSize(WPARAM wParam)
{
	tray.OnSizeTray(wParam);            //<<< Minimize in tray / Restore
}
void Exchange::OnNotifyTray(LPARAM lParam)
{
	tray.OnNotifyTray(lParam);          //<<< Tray message (mouse clicks on tray icon)
}
void Exchange::OnCommand(WPARAM wParam)
{
	tray.OnCommandTrayMenu(wParam);//<<< Popup menu message (mouse clicks)
}
void Exchange::OnDestroyApp(void)
{
	// UpdateData(true); // Считывание данных из полей диалога
	UpdateDataIni(false); // Сохранение данных в файл

	// Удаление графиков
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->DestroyGraph();
		pLastTmpInt = pLastTmpInt->pPrevious;
	};
}

//---- OnPaint -  должна выполнятся при получении события WM_PAINT
void Exchange::OnPaint(void)
{
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->OnPaint();

		pLastTmpInt = pLastTmpInt->pPrevious;
	};
}
// При приходе сообщения WM_LBUTTONDOWN
int Exchange::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->OnLButtonDown( wParam,  lParam);

		pLastTmpInt = pLastTmpInt->pPrevious;
	};

	return 0;
}
// При приходе сообщения WM_MOUSEMOVE
int Exchange::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->OnMouseMove( wParam,  lParam);

		pLastTmpInt = pLastTmpInt->pPrevious;
	};

	return 0;
}
// При приходе сообщения WM_LBUTTONUP
int Exchange::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->OnLButtonUp( wParam,  lParam);

		pLastTmpInt = pLastTmpInt->pPrevious;
	};

	return 0;
}
// При приходе сообщения WM_LBUTTONDBLCLK
int Exchange::OnLButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	ValueList *pLastTmpInt = pLastValue;

	while(pLastTmpInt)
	{
		if (EXCHANGE_GRAPH == pLastTmpInt->ucType) ((Graph*)pLastTmpInt->pValue)->OnLButtonDblClk( wParam,  lParam);

		pLastTmpInt = pLastTmpInt->pPrevious;
	};

	return 0;
}

bool Exchange::bIsTray()				// Находится ли программа в Трее
{
	return tray.TRAY_IsInTray();
}


//---- DeleteValueList -  Удаление связанного списка указателей
void Exchange::DeleteValueList()
{ 
	ValueList *pTmpInt;

	while(pLastValue)
	{
		pTmpInt = pLastValue->pPrevious;
		delete pLastValue;
		pLastValue = pTmpInt;
	};
}

//// При создании диалога свойств, для внесение в его поля текущих величин
//void Exchange::InitOptionsBox(HWND hDlg)
//{
//}

void Exchange::UpdateData(bool fromWnd, int nIDDlgItem)
{
	ValueList *pTmpInt, *pLastTmpInt = pLastValue;
	char szTmp[15];
	if (fromWnd)
	{// Из окна в переменную
		while(pLastTmpInt)
		{
			if ((0 == nIDDlgItem) || (pLastTmpInt->nIDDlgItem == nIDDlgItem))
			{
				switch(pLastTmpInt->ucType)
				{
				case EXCHANGE_INT:
					*((int*)pLastTmpInt->pValue) = GetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, 0, 1);
					break;
				case EXCHANGE_FLOAT:
					GetDlgItemText(hDlgOpt, pLastTmpInt->nIDDlgItem, szTmp, 15);
					*((float*)pLastTmpInt->pValue) = atof(szTmp);
					break;
				case EXCHANGE_UINT:
					*((unsigned int*)pLastTmpInt->pValue) = GetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, 0, 1);
					break;
				case EXCHANGE_USHORT:
					*((unsigned short*)pLastTmpInt->pValue) = GetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, 0, 1);
					break;
				case EXCHANGE_SZ:
					GetDlgItemText(hDlgOpt, pLastTmpInt->nIDDlgItem, (LPSTR)pLastTmpInt->pValue, pLastTmpInt->unLength);
					break;
				case EXCHANGE_BOOL_CHECK:
					*((bool*)pLastTmpInt->pValue) = IsDlgButtonChecked(hDlgOpt, pLastTmpInt->nIDDlgItem);
					break;
				case EXCHANGE_SZ_MULTY:
					break;
					/*default:*/
				}
			}
			pLastTmpInt = pLastTmpInt->pPrevious;
		};
	}
	else
	{// В окно из переменной 
		while(pLastTmpInt)
		{
			if ((0 == nIDDlgItem) || (pLastTmpInt->nIDDlgItem == nIDDlgItem))
			{
				switch(pLastTmpInt->ucType) 
				{
				case EXCHANGE_INT:
					SetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, *((int*)pLastTmpInt->pValue), 0 );
					break;
				case EXCHANGE_FLOAT:
					sprintf(szTmp, "%3.3f", *((float*)pLastTmpInt->pValue));
					SetDlgItemText(hDlgOpt, pLastTmpInt->nIDDlgItem , szTmp);
					break;
				case EXCHANGE_UINT:
					SetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, *((unsigned int*)pLastTmpInt->pValue), 0 );
					break;
				case EXCHANGE_USHORT:
					SetDlgItemInt(hDlgOpt, pLastTmpInt->nIDDlgItem, *((unsigned short*)pLastTmpInt->pValue), 0 );
					break;
				case EXCHANGE_SZ:
					SetDlgItemText(hDlgOpt, pLastTmpInt->nIDDlgItem, (LPSTR)pLastTmpInt->pValue);
					break;
				case EXCHANGE_BOOL_CHECK:
					CheckDlgButton(hDlgOpt, pLastTmpInt->nIDDlgItem, *((bool*)pLastTmpInt->pValue));
					break;
				case EXCHANGE_SZ_MULTY:

					break;
					/*case :
					break;
					default:*/
				}
			}

			pLastTmpInt = pLastTmpInt->pPrevious;
		};
	}
}
void Exchange::UpdateDataIni(bool fromFile)
{
	ValueList *pTmpInt, *pLastTmpInt = pLastValue;
	char szTmp[15];
	if (fromFile)
	{// Из файла в переменную
		while(pLastTmpInt)
		{
			if (pLastTmpInt->bIsSaveToIni)
			{
				switch(pLastTmpInt->ucType) 
				{
				case EXCHANGE_INT:
					*((int*)pLastTmpInt->pValue) = 
						GetPrivateProfileInt(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, 0, szIniFileName);
					break;
				case EXCHANGE_FLOAT:
					GetPrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, "0", szTmp, 15, szIniFileName);
					*((float*)pLastTmpInt->pValue) = atof(szTmp);
					break;
				case EXCHANGE_UINT:
					*((unsigned int*)pLastTmpInt->pValue) = 
						GetPrivateProfileInt(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, 0, szIniFileName);
					break;
				case EXCHANGE_USHORT:
					*((unsigned short*)pLastTmpInt->pValue) = 
						GetPrivateProfileInt(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, 0, szIniFileName);
					break;
				case EXCHANGE_SZ:
					GetPrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, "", (LPSTR)pLastTmpInt->pValue, pLastTmpInt->unLength, szIniFileName);
					break;
				case EXCHANGE_BOOL_CHECK:
					*((bool*)pLastTmpInt->pValue) = 
						GetPrivateProfileInt(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, 0, szIniFileName);
					break;
				case EXCHANGE_SZ_MULTY:
					//char szFileNum[9];
					//for (int i = 0; i < pLastTmpInt->unLength; ++i)
					//{
					//	strcpy(szFileNum, pLastTmpInt->lpKeyName);
					//	char *p = szFileNum + strlen(szFileNum);

					//	*p = (unsigned char)(i + 48);
					//	*(++p) = 0;

					//	GetPrivateProfileString(pLastTmpInt->lpAppName,szFileNum,szFileNum,(char*)((char*)pLastTmpInt->pValue + i * MAX_PATH),MAX_PATH,	szIniFileName);
					//}
					break;

				}
			}

			pLastTmpInt = pLastTmpInt->pPrevious;
		};
	}
	else
	{// В файл из переменной 
		while(pLastTmpInt)
		{ 
			if (pLastTmpInt->bIsSaveToIni)
			{
				switch(pLastTmpInt->ucType) 
				{
				case EXCHANGE_INT:
					sprintf(szTmp, "%d", *((int*)pLastTmpInt->pValue));
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, szTmp, szIniFileName);
					break;
				case EXCHANGE_FLOAT:
					sprintf(szTmp, "%f", *((float*)pLastTmpInt->pValue));
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, szTmp, szIniFileName);
					break;
				case EXCHANGE_UINT:
					sprintf(szTmp, "%d", *((unsigned int*)pLastTmpInt->pValue));
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, szTmp, szIniFileName);
					break;
				case EXCHANGE_USHORT:
					sprintf(szTmp, "%d", *((unsigned short*)pLastTmpInt->pValue));
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, szTmp, szIniFileName);
					break;
				case EXCHANGE_SZ:
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, (LPSTR)pLastTmpInt->pValue, szIniFileName);
					break;
				case EXCHANGE_BOOL_CHECK:
					sprintf(szTmp, "%d", *((bool*)pLastTmpInt->pValue));
					WritePrivateProfileString(pLastTmpInt->lpAppName, pLastTmpInt->lpKeyName, szTmp, szIniFileName);
					break;
				case EXCHANGE_SZ_MULTY:
					//char szFileNum[MAX_PATH]; // Имя параметра имени файла
					//for (int i = 0; i < pLastTmpInt->unLength; ++i)
					//{
					//	strcpy(szFileNum, pLastTmpInt->lpKeyName);
					//	char *p = szFileNum + strlen(szFileNum);
					//	*p = (unsigned char)(i + 48);
					//	*(++p) = 0;

					//	WritePrivateProfileString(pLastTmpInt->lpAppName, szFileNum, ((char*)pLastTmpInt->pValue + i * MAX_PATH), szIniFileName);
					//	//etPrivateProfileString(pLastTmpInt->lpAppName,szFileNum,szFileNum,,MAX_PATH,	szIniFileName);
					//}

					break;

				}
			}

			pLastTmpInt = pLastTmpInt->pPrevious;
		};
	}
}


void Exchange::AddValue(
						void* pValue,			// Указатель на переменную
						unsigned char ucType,		// Тип переменной
						int nIDDlgItem,			// Идентификатор переменной в окне(для отображаемых)
						bool bIsSaveToIni,		// Признак записи в Ини файл
						char *lpAppName,		// Имя раздела в Ини файле
						char *lpKeyName,		// Имя переменной в Ини файл
						unsigned int unLength	// Длина буфера переменной (для текстовых)
						)
{
	pLastValue = new ValueList(pLastValue);

	pLastValue->pValue = pValue;
	pLastValue->ucType = ucType;
	pLastValue->nIDDlgItem = nIDDlgItem;
	pLastValue->bIsSaveToIni = bIsSaveToIni;
	pLastValue->unLength = unLength;

	if (bIsSaveToIni)
	{
		if(lpAppName)	{strncpy(pLastValue->lpAppName, lpAppName, EX_LEN_NM); *(pLastValue->lpAppName + EX_LEN_NM) = 0;}
		else bIsSaveToIni = false;
		if(lpKeyName)	{strncpy(pLastValue->lpKeyName, lpKeyName, EX_LEN_NM); *(pLastValue->lpKeyName + EX_LEN_NM) = 0;}
		else bIsSaveToIni = false;
	}
}

int ConvertFileNamesFromMS(int *nCount,		// Указатель на число файлов
						   char *szMSFiles,	// Указатель на архив имен файлов
						   char *szPath,		// Указатель на директорию
						   char *szFiles,		// Двумерный массив имен файлов ВТОРАЯ РАЗМЕРНОСТЬ - MAX_PATH
						   int nCountMax)	// Если 0 - From MS, != 0 - N_files_max
{
	if (nCountMax != 0)// Из MS в массив
	{
		int nCnt = 0; // Счетчик числа файлов
		char *p = szMSFiles;

		while (*(p++) != 0) {} // Перебор до конца первого слова

		if (*(p) == 0)			// Если после первого слова - 2 "0"
		{
			nCnt = 1;		// Число файлов = 0

			p = strrchr(szMSFiles, '\\');
			*p = '\0'; // Теперь в filename записан путь к файлу

			if (szPath) strcpy(szPath, szMSFiles);
			if (szFiles) strcpy(szFiles, (p + 1));
		}
		else // Если после первого слова - 1 "0"
		{
			nCnt = 0;
			if (szPath) strcpy(szPath, szMSFiles);
			do {
				if (szFiles) strcpy((char*)(szFiles + MAX_PATH * nCnt ), p);
				while (*(p++) != 0) {}
				if ( ++nCnt >= nCountMax ) break;
			} 
			while(*(p) != 0);
		}
		*nCount = nCnt;
	}
	else // Из массива в MS 
	{
		char *p = szMSFiles;
		strcpy(p, szPath);

		if (*nCount == 1)
		{
			strcat(p, "\\");
			strcat(p, szFiles);
		}
		else
		{
			for (int i = 0; i < *nCount; ++i)
			{
				p += strlen(p);	
				strcpy((p + (strlen(p++))), (char*)(szFiles + MAX_PATH * i));
			}
		}
		p += strlen(p)+1;	
		*p = '\0';
	}

	return 0;
}
int Exchange::AddHist(char* szTxt, char *szExTxt, int nTimeMode) // nTimeMode: 1-полный, 2-только время
{
	SYSTEMTIME st;
	GetLocalTime(&st);         // gets current time
	char szTime[50];
	char szTmpHst[LEN_HIST_MAX];

	switch (nTimeMode)
	{
	case 1:
		sprintf(szTime, "%4d-%2.2d-%2.2d - %2.2d:%2.2d:%2.2d,%3.3d  ",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		break;
	case  2:
		sprintf(szTime, "%2.2d:%2.2d:%2.2d  ",
			st.wHour, st.wMinute, st.wSecond);
		break;
	}
	if (szExTxt != NULL)
		strcpy(szTmpHst, szExTxt);
	else 
		*szTmpHst = 0;
	strcat(szTmpHst, szTime);
	strcat(szTmpHst, szTxt);
	strcat(szTmpHst, "\r\n");

	FILE *fHst = fopen ("Hstr.txt","ab");
	fseek(fHst, 0, SEEK_END);
	fwrite(szTmpHst, strlen(szTmpHst), 1, fHst);
	fclose(fHst);

	int a = (int)strlen(szTmpHst);
	nLenHist += a;

	if (nLenHist >= LEN_HIST_MAX)
	{
		// Буфер заполнен
		memset(szHistory, 0, LEN_HIST_MAX), nLenHist = (int)strlen(szTmpHst);
	}

	strcat(szTmpHst, szHistory);
	strcpy(szHistory,szTmpHst );

	SetWindowText(hWndHist_net, szHistory);

	//SetDlgItemText(hWnd, IDC_ED_HISTORY, szHistory);
	return 0;
}