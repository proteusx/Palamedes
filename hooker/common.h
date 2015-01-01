/************************************************************/
/* common.h                                                 */
/* Used by hooker.h to define the exported functions        */
/* and by palamedes.h to specify ths sameimported functions */
/************************************************************/

// Better define in makefile
// windows XP or better.
// #define WINVER 0x0501    
#pragma once
#include <windows.h>
//----------------------------------------------------------
// If compilling the dll source #DEFINE HOOKDLL_EXPORTS 
// If importing the functions do nothing, 
//----------------------------------------------------------
#ifdef HOOK_EXPORTS
#define HOOKDLL_API __declspec(dllexport)
#else
#define HOOKDLL_API __declspec(dllimport)
#endif

// This function installs the Keyboard hook.
HOOKDLL_API HHOOK installhook();

//This function removes the previously installed hook.
HOOKDLL_API BOOL removehook(HHOOK hook);

//----------------------------------------------------------
//
//                  Hook Handler 
// This function is called when the keyboard is operated.
//
//----------------------------------------------------------
HOOKDLL_API LRESULT CALLBACK hookproc(int ncode,WPARAM wparam,LPARAM lparam);


