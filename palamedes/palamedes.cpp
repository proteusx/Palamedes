/*
 * =============================================================================
 *
 *       Filename:  palamedes.cpp
 *
 *    Description:  Polytonic Greek Typing Utility
 *
 *        Version:  1.0.0
 *        Created:  28/04/2013 
 *        Modified  10/03/2015
 *       Compiler:  gcc
 *
 * =============================================================================
 */


#include "palamedes.h"
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	                  LPSTR lpCmdLine, int nCmdShow
	                )
{
  MSG msg;
  if (FindWindow(NULL, LPCSTR("Palamedes"))) return 0;
  hwnd = CreateDialog(  GetModuleHandle(NULL), 
                        MAKEINTRESOURCE(IDD_MAIN), NULL, 
                        DlgProc   );
  hicon_off = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_OFF));
  hicon_on = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_ON));
  SendMessage (hwnd, WM_SETICON, (WPARAM) ICON_SMALL, (LPARAM) hicon_off);

  // SetWindowLong(hwnd, GWL_STYLE, 0);  // Remove all decorations

  //------------------------------------------------------
  //  Prepare notification data structure (for systray)
  //------------------------------------------------------
  ZeroMemory(&niData,sizeof(NOTIFYICONDATA));
  niData.cbSize = sizeof(NOTIFYICONDATA);
  niData.uID = HOOK_TRAY_ICON_ID;  
  niData.uFlags = NIF_ICON|NIF_MESSAGE | NIF_TIP;
  niData.hIcon = hicon_off; 
  niData.hWnd = hwnd;
  niData.uCallbackMessage = MY_TRAY_ICON_MESSAGE; 
	// tooltip message
  lstrcpyn(niData.szTip, 
      LPCSTR("Palamedes\nPolytonic Keyboard Modifier"), 
      sizeof(niData.szTip)/sizeof(TCHAR));
  Shell_NotifyIcon(NIM_ADD,&niData);
  //------------------------------------------------------

  // ShowWindow(hwnd, nCmdShow); // SW_SHOW);
  // UpdateWindow(hwnd);
  while(GetMessage(&msg, NULL, 0, 0))
      {
          if(!IsDialogMessage(hwnd, &msg))
          {
              TranslateMessage(&msg);
              DispatchMessage(&msg);
          }
      }
  return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  int buttonID, event;
	switch(Message)
	{
    // case WM_INITDIALOG: break;
    // case WM_MOVE: return 0; break;
    case MY_TRAY_ICON_MESSAGE:
      switch(lParam)
      {
        case WM_LBUTTONDBLCLK: ShowWindow(hwnd, SW_RESTORE);
            break;
        case WM_RBUTTONDOWN:
        case WM_CONTEXTMENU:  ShowContextMenu(hwnd);
      }
      break;
    case WM_SYSCOMMAND:
      if((wParam & 0xFFF0) == SC_MINIMIZE)
      {
        ShowWindow(hwnd, SW_HIDE);
        return 1;
      }
    case WM_COMMAND:
    {
      buttonID = LOWORD (wParam);
      event = HIWORD (wParam);
      switch (buttonID)
      {
        case ID_EXIT: DestroyWindow(hwnd); break;
        //------------------------------------------------------------
        //           Check Button 
        //  Install or uninstall keyboard hook.     
        //------------------------------------------------------------
        case ID_CHECK: 
        case SWM_DISABLE:
        case SWM_ENABLE:              
          if (!hooked)
          {
            hook = installhook();
            if (!hook)               // Error: Failed to install hook
            {
              MessageBox(NULL,LPCSTR("Unable to install hook"),LPCSTR("Error!"),MB_OK);
              CheckDlgButton(hwnd, ID_CHECK, BST_UNCHECKED); 
              return 0;
            }
            CheckDlgButton(hwnd, ID_CHECK, BST_CHECKED); 
            niData.hIcon = hicon_on; 
            Shell_NotifyIcon(NIM_MODIFY,&niData);
            SendMessage (hwnd, WM_SETICON, (WPARAM) ICON_SMALL, (LPARAM) hicon_on);
            hooked = 1;
          }
          else 
          { 
            hooked = !removehook(hook);
            if (hooked)                // Error: Failed to remove hook
            {
                MessageBox(NULL,LPCSTR("Unable to remove hook"),LPCSTR("Error!"),MB_OK);
                CheckDlgButton(hwnd, ID_CHECK, BST_CHECKED); 
                return 0;
            }
            CheckDlgButton(hwnd, ID_CHECK, BST_UNCHECKED); 
            niData.hIcon = hicon_off; 
            Shell_NotifyIcon(NIM_MODIFY,&niData);
            SendMessage (hwnd, WM_SETICON, (WPARAM) ICON_SMALL, (LPARAM) hicon_off);
            hooked = 0;
          }
          break;
        case SWM_SHOW: ShowWindow(hwnd, SW_RESTORE); break;
        case SWM_HIDE:
        case ID_OK:   ShowWindow(hwnd, SW_HIDE); break;
        case ID_ABOUT:               
          {
            // The compiler concatenates succesive strings "......""......"
            // So VERSION will be concatenate with the rest of the message.
            MessageBox(NULL,LPCSTR("     Palamedes\n\nPolytonic Typing Aid\n\nVersion: "VERSION),LPCSTR("About Palamedes"),MB_OK);
            return 0;
          }
        case ID_HELP: ShellExecute(NULL, NULL, 
                          LPCSTR("C:\\Program Files\\palamedes\\table-gr.pdf"), 
                          NULL, NULL, SW_SHOWNORMAL); 
                      return 0;
      } //end switch buttonID
    } break;
    case WM_CLOSE: DestroyWindow(hwnd); return 1; break;
    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &niData);
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        break;
    default:	return FALSE;
	}
	return 0;
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if(hMenu)
	{
		if( hooked )
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_DISABLE, LPCSTR("Disable"));
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ENABLE, LPCSTR("Enable"));
    if( IsWindowVisible(hWnd) )
      InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, LPCSTR("Hide"));
    else
      InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, LPCSTR("Show"));
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(hMenu, -1, MF_BYPOSITION, ID_HELP, LPCSTR("Help"));
    InsertMenu(hMenu, -1, MF_BYPOSITION, ID_ABOUT, TEXT("About"));
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(hMenu, -1, MF_BYPOSITION, ID_EXIT, LPCSTR("Exit"));

		// NOTE:	must set window to the foreground or the
		//			  menu won't disappear when it should
		SetForegroundWindow(hWnd);
		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			              pt.x, pt.y, 0, hWnd, NULL );
		DestroyMenu(hMenu);
	}
}

