/**********************************/
/* hooker.h. dll header.          */
/* Define functions and variables */
/**********************************/

#include "common.h"

// Prototypes
int put_accent (unsigned int char_index_in, unsigned char accent);
LPARAM MakeLp(DWORD scancode, DWORD flags);
int hooked_key(DWORD test_key);
BOOL backspace();
 
// Variables
HINSTANCE hinstance = NULL;
BYTE ks[256];
const int str_size = 4;  // number of chars

/************************************/
/* Temporary character buffer.      */
/* w[0] current char to be printed. */
/* w[1] previous char               */
/************************************/
WCHAR w[str_size];      

unsigned char dead_key = 0;  // 1,2,3 = tonos, dialytika, both

unsigned char accent_keys[] = {
  /*  /     \     "     [     ]     ~     =     -     ^      */
    0XBF, 0XDC, 0XDE, 0XDB, 0XDD, 0XC0, 0XBB, 0XBD, 0X36, 0X0 };

unsigned char vowels[] = {
  // Not all are vowels.
  // This list contains all non accent trapped keys
  // Non vowels are needed for the special symbols.
  /*  ;     A     E     H     I     O     Y     V     R     W     SP    Q       */
    0XBA, 0X41, 0X45, 0X48, 0X49, 0X4F, 0X59, 0X56, 0X52, 0X57, 0x20, 0X51,
  /* K     S     P     F     $     .<    #     ,<         */   
    0X4B, 0X53, 0X50, 0X46, 0X34, 0XBE, 0X33, 0XBC, 0X0 };

unsigned char movement[]={ 
  // Movement keys are traped to reset the temp buffer w[]
  // when the cursor is moved away from the previously
  // typed character, to prevent stupid results.
  // However, if attempt to change an accent after the cursor 
  // has been moved by the mouse, we get eroneous results.
  // Can only prevent this by trapping the mouse too. FIXME
  VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_NEXT, VK_PRIOR,
  VK_HOME, VK_END, VK_DELETE, VK_BACK, VK_DELETE };


//---------------------------------
// Shared data among all instances.
//---------------------------------

//  Uncoment for gcc
HHOOK hook __attribute__((section(".HOOKDATA"), shared)) = NULL;
HWND hwnd  __attribute__((section(".HOOKDATA"), shared)) = NULL;

// Uncoment for Visual Studio
// #pragma data_seg(".HOOKDATA")     
// HHOOK hook = NULL;
// HWND hwnd = NULL;
// #pragma data_seg()
// // linker directive for shared data
// #pragma comment(linker, "/SECTION:.HOOKDATA,RWS")  


