/*
 * ============================================================================
 *
 *       Filename:  hooker.h
 *
 *    Description:  DLL header 
 *                  Defines Macros, and variables for hooker.cpp
 *
 *        Version:  1.0.1
 *        Created:  10/03/2015 
 *       Compiler:  gcc
 *
 * ============================================================================
 */

#include "common.h"

//-----------------
//    Macros
//-----------------
#define NEXT_HOOK  CallNextHookEx(hook,ncode,wparam,lparam)

// Prints x on the active window
#define PRINT(x) PostMessage ( target_window, WM_CHAR, (WCHAR)x, MakeLp(key->scanCode, key->flags))

// returns UCS (0XHHHH) value of a key pressed.
#define UCS(z) ToUnicodeEx(vkey, key->scanCode, ks, (LPWSTR)z, 1, 0, localeID);

// Checks if SHIFT or CAPS Lock is on
// #define SHIFT ( (ks[VK_SHIFT] & 0X80)   || (ks[VK_CAPITAL] & 0X01) )        

#define NO_KEY 1

//-----------------
//   Prototypes
//-----------------
int put_accent (unsigned int char_index_in, unsigned char accent);
LPARAM MakeLp(DWORD scancode, DWORD flags);
BOOL backspace();
 
//-----------------
//   Variables
//-----------------
HINSTANCE hinstance = NULL;
BYTE ks[256];
const int str_size = 4;  // number of chars. FIXME Too many, may need only 2. 
WCHAR accent_char[2];          
int i;

/************************************/
/* Temporary character buffer.      */
/* w[0] current char, to be printed */
/* w[1] previous char               */
/************************************/
WCHAR w[str_size];      

unsigned char dead_key = 0;  // 1,2,3 = tonos, dialytika, both

unsigned char accent_keys[] = {
  /*  /     \     "     [     ]     ~     =     -     ^      */
    0XBF, 0XDC, 0XDE, 0XDB, 0XDD, 0XC0, 0XBB, 0XBD, 0X36    };

unsigned char vowels[] = {
  // This list contains the vowel keys 
  // and the keys for special symbols 
  // SPACE is trapped to enter accents on their own.
  /*  A     E     H     I     O     Y     V     R      SP    Q            */
     0X41, 0X45, 0X48, 0X49, 0X4F, 0X59, 0X56, 0X52,  0x20, 0X51,
  /*  K     S     P     F     $     .<    #     ,<     '     -            */   
     0X4B, 0X53, 0X50, 0X46, 0X34, 0XBE, 0X33, 0XBC, 0XDE, 0XBD  };

unsigned char movement[] = { 
  // Movement keys are trapped to reset the temp buffer w[]
  // when the cursor is moved away from the previously
  // typed character, to prevent stupid results.
  //
  // However, if attempt to change an accent after the cursor 
  // has been moved by the mouse, we get eroneous results.
  // Can only prevent this by trapping the mouse too. FIXME
  //
  /* VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_NEXT, VK_PRIOR, */
      0x26,   0x28,   0x25,     0x27,     0x22,   0x21,
  /* VK_HOME, VK_END, VK_DELETE */ 
       0x24,   0X23,   0x2E  };


//---------------------------------
// Shared data among all instances
//---------------------------------

//  Uncoment for gcc
HHOOK hook __attribute__((section(".HOOKDATA"), shared)) = NULL;
HWND hwnd  __attribute__((section(".HOOKDATA"), shared)) = NULL;

// Uncoment for Visual Studio
//
// #pragma data_seg(".HOOKDATA")     
// HHOOK hook = NULL;
// HWND hwnd = NULL;
// #pragma data_seg()
// // linker directive for shared data
// #pragma comment(linker, "/SECTION:.HOOKDATA,RWS")  


