/***************/
/*             */
/* palamedes.h */
/*             */
/***************/

// import dll header, 
// Defines imported functions
#include "../hooker/common.h"

#define VERSION "1.0.0-GCC"


//-----------------------
//     Resources
//-----------------------
#define IDI_ICON_OFF 100
#define IDI_ICON_ON 101
#define IDD_MAIN 102
#define ID_CHECK 1001
#define ID_EXIT  1002
#define ID_OK    1004
#define IDC_STATIC 1005
#define CLICKED 0
#define HOOK_TRAY_ICON_ID 2001
#define MY_TRAY_ICON_MESSAGE WM_APP+1
#define SWM_HIDE WM_APP+2
#define SWM_SHOW WM_APP+3
#define SWM_DISABLE WM_APP+4
#define SWM_ENABLE WM_APP+5
#define ID_HELP WM_APP+6
#define ID_ABOUT WM_APP+7

//-----------------------
// Function Prototypes
//-----------------------
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM,  LPARAM );
void ShowContextMenu(HWND hWnd);


//-----------------------
// Global Vartiables
//-----------------------
HINSTANCE hInstance;
HWND hwnd;
HHOOK hook;
BOOL hooked = 0;
HICON hicon_on, hicon_off; 
NOTIFYICONDATA niData; 


