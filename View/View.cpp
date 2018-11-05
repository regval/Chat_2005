// View.cpp : Defines the entry point for the application.
//

#include <WinSock2.h>
#include "View.h"
#include ".\view_class.h"

HINSTANCE hInst;	// current instance
int nIsNewVersion;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Options(HWND, UINT, WPARAM, LPARAM);

void DelSelfAndRun(void)
{
	char modulename[MAX_PATH];		// Имя старого файла
	char pathtomodule[MAX_PATH];	// Путь к рабочей дирктории
	char newmodname[MAX_PATH];		// Имя нового файла
    char batfile[MAX_PATH];			// Имя вспомогательного бат-файла
	
	char batlines[MAX_PATH*4];		// Текст вспомогательного бат-файла
	
	LPSTR tempdir;					// Путь к темп-директории
	char Buf[MAX_PATH];				// 
	char *p;						// 

	char szNewPath[MAX_PATH];

	GetModuleFileName(NULL,modulename,MAX_PATH);
	
	strcpy(pathtomodule, modulename);
	p = strrchr(pathtomodule, '\\');
	*p = '\0'; 

	strcpy(modulename, strrchr(modulename, '\\') + 1);

	strcpy(newmodname, "NewVers.exe");

	tempdir = ((GetEnvironmentVariable(TEXT("TEMP"),
		Buf, MAX_PATH) > 0) ? Buf : NULL);

	
	strcpy(batfile,tempdir);
	strcat(batfile,"\\");
	strcat(batfile,"delself.bat");

	sprintf(batlines, 
		"@echo off\n"
		":try\n"
		"del \"%s\\%s\"\n"
		"if exist \"%s\\%s\" goto try\n"
		"ren \"%s\\%s\" %s\n"
		"start /D \"%s\" /MIN %s\n"
		"del %s", 
		pathtomodule, 
		modulename, 
		pathtomodule, 
		modulename,
		pathtomodule, 
		newmodname,
		modulename,
		pathtomodule, 
		modulename,
		batfile);
	//strcpy(batlines,"@echo off\n:try\ndel ");
	//strcat(batlines,modulename);
	//strcat(batlines,"\nif exist ");
	//strcat(batlines,modulename);
	//strcat(batlines," goto try\n");
	//strcat(batlines,"rename ");
	//strcat(batlines,szNewPath);
	//strcat(batlines," ");
	//strcat(batlines,modulename);
	//strcat(batlines,"\n");
	//strcat(batlines,modulename);
	//strcat(batlines,"\n");
	//strcat(batlines,"del ");
	//strcat(batlines,batfile);

	DWORD NOfBytes;
	HANDLE hbf= CreateFile(batfile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ
		| FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hbf,batlines,strlen(batlines),&NOfBytes, NULL);
	CloseHandle(hbf);

	STARTUPINFO         si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;

	BOOL BR = CreateProcess(
		NULL,
		batfile,
		NULL,
		NULL,
		FALSE,
		IDLE_PRIORITY_CLASS|DETACHED_PROCESS,
		NULL,
		NULL,
		&si,
		&pi );
}

int SetAutorunCopyProgramFiles()
{
	char *p;
	char szPath[MAX_PATH];
	char szNewPath[MAX_PATH];

	BOOL BRes;
	int nRes;

	// Перепись в Програм файлс
	strcpy(szNewPath, "c:\\Program Files\\NewChat\\NewChat.exe");
	GetModuleFileName(NULL,szPath,MAX_PATH);

	if (strncmp(szPath,szNewPath, strlen(szNewPath)) != 0 )
	{
		BRes =  CreateDirectory(
			"c:\\Program Files\\NewChat",
			NULL
			);

		BRes = CopyFile(
			szPath,
			szNewPath,
			FALSE
			);

		// Формирование имени ini файла
		p = strrchr(szPath, '.');	*p = '\0';
		*(long*)p = '.' | 'i' << 8 | 'n' << 16 | 'i' << 24;

		BRes = CopyFile(
			szPath,
			"c:\\Program Files\\NewChat\\NewChat.ini",
			FALSE
			);
		p = strrchr(szPath, '\\');
		*p = '\0'; // Теперь в filename записан путь к файлу

		strcat(szPath, "\\Hstr.txt");

		BRes = CopyFile(
			szPath,
			"c:\\Program Files\\NewChat\\Hstr.txt",
			FALSE
			);
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Запись в автозагрузку
		char *szKeyName = "NewChat";
		HKEY hKey;
		LONG lReg = RegOpenKeyEx(
			HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
			NULL,
			KEY_ALL_ACCESS,
			&hKey
			);

		if (RegQueryValueEx(
			hKey,
			szKeyName,
			NULL,
			NULL ,
			NULL ,
			NULL 
			) != ERROR_SUCCESS)
		{
			RegSetValueEx(
				hKey,
				szKeyName,
				0,
				REG_SZ,
				(BYTE*)szNewPath,
				strlen(szNewPath)
				);
		}

		lReg = RegCloseKey(hKey);

		// Start the child process. 
		if( CreateProcess( NULL, // No module name (use command line). 
			szNewPath, // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 
		{
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );
		}
		return 1;
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	// Включение возможности обновления версии только в отладчике
	if (lpCmdLine[1] == 'd')
	{
		nIsNewVersion = 386;
	}
	else 
	{
		nIsNewVersion = 0;
		if (SetAutorunCopyProgramFiles()) return -1;
	}

	// Кроме 
	
	MSG msg;
	HACCEL hAccelTable;
	const MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VIEW, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_VIEW);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_VIEW;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	RegisterClassEx(&wcex);

	// Perform application initialization:
	HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_VIEW);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if (nIsNewVersion == 578)
	{
		DelSelfAndRun();
	}

	return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool IsInit;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static int ss;
	static View_class* pvc;

	switch (message) 
	{
	case WM_COMMAND:
		{
			pvc->OnCommand(wParam);
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam);

			switch (wmEvent)
			{
			case EN_UPDATE:

				break;
			}

			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

			case IDM_ACT:
				if (nIsNewVersion == 386)
					pvc->SetNewVersion();
				break;

			case IDM_OPT:
				DialogBoxParam(hInst, (LPCTSTR)IDD_OPTIONS, hWnd, (DLGPROC)Options, (LPARAM)pvc);
				break;
			case IDM_SEND:
				pvc->OnSend();
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_CREATE:
		pvc =  new View_class(hWnd);
		break;
	case WM_NOTIFYICONTRAY:
		pvc->OnNotifyTray(lParam);
		return TRUE;  
	case WM_USER + 256:
		Sleep(100);
		delete pvc;

		nIsNewVersion = 578;
		PostQuitMessage(0);
		return TRUE;  
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		pvc->OnPaint(hdc, ps);
		EndPaint(hWnd, &ps);

		break;
	case WM_SIZE:
		pvc->OnResize(wParam, lParam);

		break;
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

	case WM_KEYDOWN:

		if ((wParam == 13)&&(HIWORD(GetKeyState(VK_CONTROL)) == 0xffff))
		{
			pvc->OnSend();
			break;
		}

		pvc->OnKeyDown_it(wParam, lParam);

		//MessageBox(hWnd, szTmp, "parentNotify", 0);

		break;
	case WM_CHAR: 
		pvc->OnChar_it(wParam, lParam);
		break;

		//case WM_PAINT: 
		//	break;
	case WM_SETFOCUS:
		pvc->OnSelFocus_it();

		break; 
	case WM_KILLFOCUS: 
		pvc->OnKillFocus_it();

		break; 
	case WM_CLOSE: 
		//pvc->AddHist("wm_close");SIZE_MINIMIZED
		pvc->OnResize(SIZE_MINIMIZED, -1);

		break; 

	case WM_DESTROY:
		delete pvc;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}	


// Message handler for options box.
LRESULT CALLBACK Options(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	static View_class* pvc;
	static Exchange* exOpt;

	switch (message)
	{
	case WM_INITDIALOG:
		pvc = (View_class*)lParam;
		pvc->OnInitOptDlg(hDlg);
		return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			// Прорисовка всех элементов окна
			BeginPaint(hDlg, &ps); 
			EndPaint(hDlg, &ps);
			return TRUE;
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
			pvc->UpdateData ();
			pvc->OnResize(-1,-1);
			EndDialog(hDlg, LOWORD(wParam));

			break;
		case IDCANCEL:
			//delete exOpt;
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}
	return FALSE;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

