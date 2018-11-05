//#include "StdAfx.h"

#include "Tray.h"
#include <windows.h>

/**************************************************************************************************/
/**************************************************************************************************/
// Popup menu events
// Return TRUE if an event is processed, otherwise FALSE;

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

BOOL Tray::OnCommandTrayMenu(WPARAM wParam)
{
	switch(LOWORD(wParam))
	{
		//POPUP MENU ******************************************************//
	case IDM_ONTOP: //ALWAYS ON TOP
		{   
			HWND HWND_ORDER;
			BOOL Checked;

			//If menu item is checked put window on top
			if (IsItemChecked(LOWORD(wParam))){ 
				HWND_ORDER = HWND_NOTOPMOST;  // 
				Checked=false;
			}else{
				HWND_ORDER = HWND_TOPMOST;
				Checked=true;
			}

			CheckMenuItem(LOWORD(wParam),Checked);
			SetWindowPos(hwnd, HWND_ORDER, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

			//HWND hwndToolTips = CreateWindow(TOOLTIPS_CLASS,
			//	NULL,
			//	WS_POPUP | TTS_NOPREFIX | TTS_BALLOON,
			//	0, 0,
			//	0, 0,
			//	NULL, NULL,
			//	g_hinst,
			//	NULL);
			//if (hwndToolTips)
			//{
			//	// Do the standard ToolTip coding. 
			//	TOOLINFO ti;

			//	ti.cbSize = sizeof(ti);
			//	ti.uFlags = TTF_TRANSPARENT | TTF_CENTERTIP;
			//	ti.hwnd = hwnd;
			//	ti.uId = 0;
			//	ti.hinst = NULL;
			//	ti.lpszText = LPSTR_TEXTCALLBACK;

			//	GetClientRect(hwnd, &ti.rect);
			//	SendMessage(hwndToolTips, TTM_ADDTOOL, 0, (LPARAM) &ti );
			//}


		}return TRUE;		      
		//POPUP MENU ******************************************************//
	case IDM_MINIMIZE:{ //minimize/restore window

		//NOTE: SW_SHOWMINIMIZED and SW_RESTORE cause WM_SIZE event
		//(see also OnSizeTray())
		if (!bTray){ 
			ShowWindow(hwnd, SW_SHOWMINIMIZED);//minimize window in tray

		}else{       			
			ShowWindow(hwnd, SW_RESTORE);//restore window 
		}	
					  }return TRUE;
		//POPUP MENU ******************************************************//
	case IDM_ABOUT:{
		MessageBox(hwnd,APP_ABOUT,"Info",MB_ICONINFORMATION);
				   }return TRUE;	
		//POPUP MENU ******************************************************//
	case IDM_CLOSE: {//CHIUDE L'APPLICAZIONE
		SendMessage(hwnd, WM_DESTROY, 0, 0);
					}return TRUE;

	}//end switch(LOWORD(wParam))	

	return FALSE; //No event match
}
/**************************************************************************************************/
/**************************************************************************************************/
//Gestisce il ridimensionamento in tray ed il ripristino dalla tray
BOOL Tray::OnSizeTray(WPARAM wParam)
{
	if (wParam == SIZE_MINIMIZED)
	{   
		ModifyMenu(hMenu, IDM_MINIMIZE, MF_BYCOMMAND, IDM_MINIMIZE, IDM_TEXT_RESTORE);//CAMBIA IL TESTO DEL MENU
		SetIcon("Чат Тани и Валеры", 1);
		ShowWindow(hwnd, SW_HIDE);
		bTray = true;
	}else{
		ModifyMenu(hMenu, IDM_MINIMIZE, MF_BYCOMMAND, IDM_MINIMIZE, IDM_TEXT_MINIMIZE);//CAMBIA IL TESTO DEL MENU
		SetIcon("", 0);
		bTray = false;
	}

	return bTray;
}
/**************************************************************************************************/
/**************************************************************************************************/
// Mouse clicks on tray
void Tray::OnNotifyTray(LPARAM lParam)
{
	// Right click, show the menu
	switch(lParam) 
	{
	case WM_RBUTTONDOWN:
		{
			// load menu
			POINT pt;// point structure
			GetCursorPos(&pt);// get the cursors position
			SetForegroundWindow(hwnd);// set the foreground window
			TrackPopupMenu(hMenu,TPM_RIGHTALIGN,pt.x,pt.y,0,hwnd,NULL);// track the popup			              
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetIcon("Чат Тани и Валеры", 2);//imposta la nuova icona
		}
		break;
	case WM_LBUTTONUP:
		{
			SetIcon("Чат Тани и Валеры", 3);//imposta la nuova icona
		}
		break;

	//case :
	//	break;

	//case :
	//	break;

	//default:
	}
	//if(lParam == WM_RBUTTONDOWN) 
	//{
	//}
	//else
	//{
		// Left click, button effect :)
		//if(lParam == WM_LBUTTONDOWN)
		//{
		//	SetIcon("ingrandisci", 2);//imposta la nuova icona
		//}
		//else
		//{	
	//		if(lParam == WM_LBUTTONUP)
	//		{
	//			SetIcon("ingrandisci", 3);//imposta la nuova icona
	//		}
	//	}    			
	//}
	PostMessage(hwnd,WM_NULL,0,0);// post a null message
}
/**************************************************************************************************/
/**************************************************************************************************/
// Bye bye...
void Tray::OnDestroyTray()
{
	//Destroy popup menu
	DestroyMenu(hMenu);
	//Ensure to remove tray icon
	SetIcon("", 0);
}
/**************************************************************************************************/
/**************************************************************************************************/
// Return tray state, TRUE if the app. is minimized to tray, otherwise FALSE.
BOOL Tray::TRAY_IsInTray(){ return bTray; }

/**************************************************************************************************/
/**************************************************************************************************/
//Show popup menu
void Tray::TRAY_Menu_Show()
{			
	POINT pt;// point structure
	GetCursorPos(&pt);// get the cursors position
	SetForegroundWindow(hwnd);// set the foreground window
	TrackPopupMenu(hMenu,TPM_RIGHTALIGN,pt.x,pt.y,0,hwnd,NULL);// track the popup
	PostMessage(hwnd,WM_NULL,0,0);// post a null message	    
}
/**************************************************************************************************/
/**************************************************************************************************/
//Initialize tray
void Tray::TRAY_Init(HWND hWnd)
{
	hwnd = hWnd;
	hMenu=CreatePopupMenu();
	CreateItemList(hMenu); 
	LoadAppIcon();
} 
/**************************************************************************************************/
/**************************************************************************************************/


/*************
*************
** PRIVATE **
*************
*************/


/**************************************************************************************************/
/**************************************************************************************************/
//Set bitmaps for popup menu items
void Tray::SetBitmaps(HMENU hMenu)
{    
	//Checked item bitmap
	HBITMAP hBitmapChecked = LoadBitmap((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
		MAKEINTRESOURCE(IDB_MENU_CHECK));
	SetMenuItemBitmaps(hMenu, IDM_ONTOP, MF_BYCOMMAND, NULL/*hBitmapUnchecked*/, 
		hBitmapChecked); 
	//Close bitmap
	hBitmapChecked = LoadBitmap((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), 
		MAKEINTRESOURCE(IDB_MENU_CLOSE));
	SetMenuItemBitmaps(hMenu, IDM_CLOSE, MF_BYCOMMAND, hBitmapChecked, NULL);

	//About bitmap  
	hBitmapChecked = LoadBitmap((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), 
		MAKEINTRESOURCE(IDB_MENU_ABOUT));
	SetMenuItemBitmaps(hMenu, IDM_ABOUT, MF_BYCOMMAND, hBitmapChecked, NULL);    
}
/**************************************************************************************************/
/**************************************************************************************************/
//Create menu items
void Tray::CreateItemList(HMENU hMenu)
{
	int ret=0; // buffer for errors

	// CREATE POPUP MENU ( on the fly )
	//hMenu = CreatePopupMenu(); // create the popupmenu (See BOOL TRAY_Init()...)
	AppendMenu(hMenu,MF_STRING ,IDM_ONTOP,IDM_TEXT_ONTOP);// append menu    
	AppendMenu(hMenu,MF_STRING ,IDM_MINIMIZE,IDM_TEXT_MINIMIZE);// append menu
	AppendMenu(hMenu,MF_SEPARATOR,0,0);// append menu
	AppendMenu(hMenu,MF_STRING ,IDM_CLOSE,IDM_TEXT_CLOSE);// append menu 
	AppendMenu(hMenu,MF_SEPARATOR,0,0);// append menu
	AppendMenu(hMenu,MF_STRING,IDM_ABOUT,IDM_TEXT_ABOUT);// append menu  

	SetBitmaps(hMenu); 
}
/**************************************************************************************************/
/**************************************************************************************************/
//Load app. icon
BOOL Tray::LoadAppIcon()
{
	// Load icon from resource
	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(APP_ICON), 
		IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	if (hIcon) {
		// Associate icon to the app.
		SendMessage(hwnd, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (hIcon));// App. icon
		SendMessage(hwnd, WM_SETICON, WPARAM (ICON_BIG), LPARAM (hIcon));  //ALT+TAB icon
		return true;
	}

	return false;
}
/**************************************************************************************************/
/**************************************************************************************************/
// Add/Set tray icon
// nAction - 1 = Create Tray Icon
//           0 = Remove Tray Icon
//           n = Change Tray Icon
// pszTip = Tray icon ToolTip text
BOOL Tray::SetIcon(PSTR pszTip, int nAction) 
{              
	UINT  uIcon; // Icon ID buffer
	DWORD dMsg;  // Message buffer (ADD/DELETE/MODIFY)

	NOTIFYICONDATA ni;
	ni.cbSize = sizeof(ni);
	ni.uID = 0;
	lstrcpyn(ni.szTip, pszTip, sizeof(ni.szTip)); 
	ni.hWnd = hwnd;
	ni.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON; // We want to use icon, tip, and callback message
	ni.uCallbackMessage = WM_NOTIFYICONTRAY;      // Our custom callback message (WM_APP + 1)

	switch (nAction){
		case 0:// Remove tray icon
			return Shell_NotifyIcon(NIM_DELETE, &ni);
			break; 
		case 1://Add icon in tray
			uIcon=APP_ICON; // Set tray icon
			dMsg=NIM_ADD; // Add tray icon        
			break;
		case 2:// Modify current icon in tray
			uIcon=TRAY_ICON; // Set new tray icon	
			dMsg=NIM_MODIFY; // Modify tray icon     
			break;               
		case 3:// Modify current icon in tray
			uIcon=APP_ICON; // Set new tray icon	
			dMsg=NIM_MODIFY; // Modify tray icon      
			break;                                
	}

	//Load selected icon
	ni.hIcon = (HICON)LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 
		MAKEINTRESOURCE(uIcon));	
	// Windooows, ohohooo, we have changed icon! :)                                                
	return Shell_NotifyIcon(dMsg, &ni);       

}
/**************************************************************************************************/
/**************************************************************************************************/
// Set checked/unchecked state of an item in the popup menu
// Return value: MF_CHECKED, MF_UNCHECKED or -1 if item not found.
DWORD Tray::CheckMenuItem(UINT IDItem,BOOL Checked)
{
	UINT uCheck;
	uCheck = Checked ? MF_CHECKED : MF_UNCHECKED; //retrieve state (checked/unchecked)

	return ::CheckMenuItem(hMenu, IDItem, MF_BYCOMMAND|uCheck);
}
/**************************************************************************************************/
/**************************************************************************************************/
//Is popup item checked?
BOOL Tray::IsItemChecked(UINT IDItem)
{
	DWORD uCheck;

	uCheck = ::CheckMenuItem(hMenu, IDItem, MF_BYCOMMAND);

	if (uCheck != -1){
		return (uCheck == MF_CHECKED) ? 1 : 0; //retrieve state (checked/unchecked)
	}

	return 0;
}