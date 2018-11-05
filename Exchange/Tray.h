/*
Tray.h
============================================================================
Файл объявления  класса работы с Треем 

Coded by 
ШУБИН ВАЛЕРИЙ СЕРГЕЕВИЧ, 2005, Санкт-Петербург
E-mail: Valeriy_Shubin@nwgsm.ru, тел.: +7(921)-984-17-53
============================================================================
*/


#pragma once

#ifndef TRAY_INTERFACE_FROM_SHUBIN_H
#define TRAY_INTERFACE_FROM_SHUBIN_H

#include <Windows.h>
#include "..\view\resource.h"

	// tray.h
	//
	// Minimize window in Tray with popup menu with icons.  
	// This code is free, you may use it without any warranty or too much expectations...
	// If you find something wrong then post your corrections or suggestions on the web.
	// If you think to do better than me simply you do not use this code but send to me the your :)
	//
	// 2003/2004 by Massimiliano Miani (imianz@tiscalinet.it)

	//About message
#define APP_ABOUT "Minimize in tray + menu with icons example\n\n(c)2003/2004 by Imianz"
	//Other message
#define MY_MSG "Minimize this window in tray and click with \n\nleft button on icon to see a 'button effect'"

	//Application icon, Tray icon and Popup menu bitmaps
#define APP_ICON       IDI_VIEW//90 //app icon
#define TRAY_ICON      IDI_SMALL//91 //tray icon

#define WM_NOTIFYICONTRAY (WM_USER + 156)//tray message
	//Popup Menu items
#define IDM_MINIMIZE   421 							
#define IDM_TEXT_MINIMIZE " Minimize to tray"			
#define IDM_TEXT_RESTORE " Restore"

//#define IDM_ABOUT	   422							
#define IDM_TEXT_ABOUT " About"
//
#define IDM_CLOSE	   423						
#define IDM_TEXT_CLOSE " Close"
//
#define IDM_ONTOP	   424						
#define IDM_TEXT_ONTOP " Always on top"


class Tray
{
public:

	//Functions
	void  TRAY_Init(HWND hwnd);             //Create tray and its popup menu 
	BOOL  TRAY_IsInTray(void);              //Is window minimized in tray?
	void  TRAY_Menu_Show(void);             //Show popup menu
	void  OnNotifyTray(LPARAM lParam);      //Tray message (mouse clicks on tray icon)
	BOOL  OnSizeTray(WPARAM wParam);        //WM_SIZE message
	void  OnDestroyTray(void);              //WM_DESTROY message 
	BOOL  OnCommandTrayMenu(WPARAM wParam); //Popup menu events


	// To start just put theese 5 line of code in your WindowProcedure (see main.cpp):

	/*
	LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
	...

	case WM_CREATE:{//(1)        
	TRAY_Init(hwnd);               //<<< Create tray icon and popup menu                  
	}break;
	case WM_SIZE:
	OnSizeTray(wParam);            //<<< Minimize in tray / Restore
	break;            
	case WM_NOTIFYICONTRAY:
	OnNotifyTray(lParam);          //<<< Tray message (mouse clicks on tray icon)
	return TRUE;       
	case WM_COMMAND:
	OnCommandTrayMenu(wParam);     //<<< Popup menu message (mouse clicks)
	break; 	                   
	case WM_DESTROY:
	OnDestroyTray();               //<<< Clean Tray related before close the app.

	...
	*/


protected:
private:
	BOOL  LoadAppIcon();                            //Application icon
	void  CreateItemList(HMENU hMenu);              //Crate Popup menu
	BOOL  SetIcon(PSTR pszTip, int nAction);        //Change tray icon
	void  SetBitmaps(HMENU hMenu);                  //Set bitmaps for popup menu items
	DWORD CheckMenuItem(UINT IDItem, BOOL Checked); //Check/uncheck an item in popup menu
	BOOL  IsItemChecked(UINT IDItem);               //Is popup item checked?

	//POPUP MENU
	HMENU hMenu;                                    // menu handle
	HWND  hwnd;                                     // main window handle
	//TRAY
	bool  bTray;                                    // tray state (is window minimized in tray?)

};

#endif// TRAY_INTERFACE_FROM_SHUBIN_H
