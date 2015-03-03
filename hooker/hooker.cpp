/*******************************************/
/* hooker.cpp                              */
/* hook, unhook and hook handler functions */
/*******************************************/

#define HOOK_EXPORTS
#include "hooker.h"

/*-----------------------------------------------------------------------------
 *                        DLL entry point.   
 *-----------------------------------------------------------------------------*/
  // Uncomment if in Visual Studio
  // BOOL APIENTRY DllMain( HANDLE hModule, 
int main( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    hinstance = (HINSTANCE)hModule;
    hook = NULL;
    return TRUE;
}  

/*-----------------------------------------------------------------------------
 *                        Hook the keyboard. 
 *-----------------------------------------------------------------------------*/
HOOKDLL_API HHOOK installhook()
{
    hook = SetWindowsHookEx(
           WH_KEYBOARD_LL,                   // Low Level keyboard hook
           hookproc,                         // The hook hander procedure
           // This param could be just  "hinstance" 
           // but it returns no handle in XP.
           // Thus we pass the handle of an already 
           // loaded module by invoking LoadLibrary. 
           LoadLibrary(TEXT("user32.dll")),  // this is needed or it fails in XP
           0);
    return hook;
}

/*-----------------------------------------------------------------------------
 *                      Unhook the keyboard. 
 *-----------------------------------------------------------------------------*/
HOOKDLL_API BOOL removehook(HHOOK hook)
{
	BOOL unhooked = UnhookWindowsHookEx(hook);
	return unhooked;
}

/******************************************************************************/
/*                                                                            */
/*                         The Hook Handler Function                          */
/*                                                                            */
/******************************************************************************/
HOOKDLL_API LRESULT CALLBACK hookproc(int ncode,WPARAM wparam,LPARAM lparam)
{   
  HKL localeID = NULL;
  HWND target_window = NULL;
  GUITHREADINFO lpgui;
  KBDLLHOOKSTRUCT* key = (KBDLLHOOKSTRUCT*)lparam;
  if ((ncode == HC_ACTION) && ((wparam == WM_SYSKEYDOWN) || (wparam == WM_KEYDOWN)))      
	{ 
      DWORD vkey = key->vkCode;
      DWORD tid = GetWindowThreadProcessId(GetForegroundWindow(),0);
      lpgui.cbSize = sizeof(GUITHREADINFO);
      GetGUIThreadInfo( tid, &lpgui );
      target_window = lpgui.hwndFocus;

      //----------------------------------
      // What keyboard is on?
      // Return unless Greek keyboard
      //----------------------------------
      localeID = GetKeyboardLayout(tid);
      if (LOBYTE(LOWORD((DWORD)localeID)) != LANG_GREEK)  
        return  CallNextHookEx(hook,ncode,wparam,lparam);
      
      //----------------------------------
      // Begin processing key
      //----------------------------------
      GetKeyState(0);                      // Needed because of an API bug.
      GetKeyboardState(ks);

      //----------------------------------
      // hooked = 0: Not a trapped key.
      // --//-- = 1: Vowel or special symbol. 
      // --//-- = 2: an post placed accent key.
      // --//-- = 3: A movement key.
      //----------------------------------
      int hooked = hooked_key(vkey);

      //----------------------------------
      // Return unless vkey is one of 
      // the trapped keys
      //----------------------------------
      if (!hooked)
        return  CallNextHookEx(hook,ncode,wparam,lparam);

      //----------------------------------
      // If a movement key character buffer 
      // has no meaning.
      // So reset temp char buffer and return.
      //----------------------------------
      if (hooked == 3)    // 3 = a movement key
      {
        w[1] = w[0] = '\0';
        return  CallNextHookEx(hook,ncode,wparam,lparam);
      }
      
      //----------------------------------
      // Test for dead keys. (;, : or W)
      // If a dead key, store the accent and return.
      // It will be put on the next character, if pertinent.
      //
      // ; -> 1   Tonos
      // : -> 2   Dialytika
      // W -> 3   Dialytika - Tonos
      //----------------------------------
      if (vkey == VK_OEM_1)                            // Test for ; or :
      {   
        (ks[VK_SHIFT] & 0X80)? dead_key = 2: dead_key = 1;
        return 1;                                      // Gobble up the key.
      }
      if ((vkey==0X57) && (ks[VK_SHIFT] & 0X80))       // test for SHIFT-W
      {     
        dead_key = 3;
        return 1;
      }

      //----------------------------------
      // If a dead key has been stored
      // emit the appropriate accented vowel
      // or special symbol
      //----------------------------------
      if (dead_key)                                    // If accent key has
      {                                                // has been pressed,
        BOOL shifted = (ks[VK_SHIFT] & 0X80)           // Capital? 
                            || 
                      (ks[VK_CAPITAL] & 0X01);        
        switch (vkey)                                  // Put accent on vowel.
        {
          case 0X41 : shifted ? w[0] = 0X386 : w[0] = 0X3AC; break;             // A
          case 0X45 : shifted ? w[0] = 0X388 : w[0] = 0X3AD; break;             // E
          case 0X48 : shifted ? w[0] = 0X389 : w[0] = 0X3AE; break;             // H
          case 0X49 : switch (dead_key)                                         // I
                      {                                
                        case 1: shifted ? w[0] = 0X38A : w[0] = 0X3AF; break;
                        case 2: shifted ? w[0] = 0X3AA : w[0] = 0X3CA; break;
                        case 3: w[0] = 0X390; break;
                      } break;
          case 0X4f : shifted ? w[0] = 0X38C : w[0] = 0X3CC; break;             // O
          case 0X59 : switch (dead_key)                                         // Y
                      {
                        case 1: shifted ? w[0] = 0X38E : w[0] = 0X3CD; break;
                        case 2: shifted ? w[0] = 0X3AB : w[0] = 0X3CB; break;
                        case 3: w[0] = 0X3B0; break;
                      } break;
          case 0X56 : shifted ? w[0] = 0X38F : w[0] = 0X3CE; break;     // V (Omega)
          //----------------------------------------------
          // Special symbols entered with the dead key.
          //----------------------------------------------
          case 0X51 : shifted ? w[0] = 0X3Df : w[0] = 0X3D9; break;  // Q (kopa)
          case 0X53 : shifted ? w[0] = 0X3DA : w[0] = 0X3DB; break;  // S (stigma)
          case 0X4B : w[0] = 0X3D7; break;                           // K (kai)
          case 0X50 : shifted ? w[0] = 0X3E1 : w[0] = 0X3E0; break;  // P (sampi)
          case 0X46 : shifted ? w[0] = 0X3DC : w[0] = 0X3DD; break;  // F (digamma)
          case 0X34 : w[0] = 0X20AC; break;                          // $ (Euro)
          case 0XDE : w[0] = 0X1FBD; break;                        // ' Apostrophos
                    // next key (. or >) (this key is for ano teleia and  R. eisag )
          case 0XBE : shifted ? w[0] = 0XBB : w[0] = 0X387; break;   
          case 0X33 : shifted ? w[0] = 0X375 : w[0] = 0X374; break;  // # (keraies)
          case 0XBD : shifted ? w[0] = 0X2014 : w[0] = 0X2013; break; // - (pavles)
          case 0XBC : if(shifted) w[0] = 0XAB; else  return 0; break; //< (L eisag.)
          default:    dead_key = 0; return 0;
        }
        PostMessage ( target_window, 
            WM_CHAR, (WCHAR)w[0], MakeLp(key->scanCode, key->flags));
        dead_key = 0;      // Reset dead key flag and print character
        // Save accented character in case is needed
        //         to add further decorations
        w[1] = w[0];       
        return 1;
      }
      
      //--------------------------------------------------
      //      Deal with accent keys.
      //--------------------------------------------------
      if (hooked == 2)    // If hooked key is an post placed accent
      {
        WCHAR accent_char[2];
        ToUnicodeEx(vkey, key->scanCode, ks, (LPWSTR)accent_char, 1, 0, localeID);
        accent_char[1]=L'\0';
        //FIXME remove type casting.
        w[0] = put_accent ( (unsigned int)w[1], (unsigned char)accent_char[0] );  
        if (w[0] == w[1]) return 1;       // Could not put the accent, do nothing.
        backspace();                      // Delete previous character.
        PostMessage (target_window,               
            WM_CHAR, (WCHAR)w[0], MakeLp(key->scanCode, key->flags));
        w[1] = w[0];
        return 1;
      }

      //--------------------------------------------------
      //      Process the vowels.
      //--------------------------------------------------
      if (hooked == 1)      
      {
        // Convert to unicode 0Xhhhh.
        ToUnicodeEx(vkey, key->scanCode, ks, (LPWSTR)w, 1, 0, localeID);
        // PostMessage (target_window,               
        //      WM_CHAR, (WCHAR)w[0], MakeLp(key->scanCode, key->flags));
        w[1] = w[0];
        return 0;
      }
	}
	// Key is of no interest to us.
	// Pass key back to OS to do as it pleases.
	return  CallNextHookEx(hook,ncode,wparam,lparam);
} // --- End of hook handler

/*-----------------------------------------------------------------------------
 *  
 *     Chek if character is in the hooked group                   
 *     Return: hooked_key                                         
 *     0: Not a trapped key.                                      
 *     1: Vowel or special symbol. ( See hooker.h)                
 *     2: A post placed accent key. (/, \, =, ~, ], [, |, \", -, ^)
 *     3: A movement key.                                         
 *
 *-----------------------------------------------------------------------------*/
int hooked_key(DWORD test_key)
{
  int i = 0;
  do {
        if ( vowels[i] == test_key) return 1;
        i++;
  } while (vowels[i] != 0X0);

  i=0;
  do {
        if ( accent_keys[i] == test_key ) 
        {
          if ( (test_key == 0X36) || (test_key == 0XC0) )
          { 
            if (ks[VK_SHIFT] & 0X80)
            { 
              return 2; 
            }
            else
            {
              break;
            }
          }
          return 2;
        }
        i++;
  } while (accent_keys[i] != 0X0);

  i=0;
  do {
        if ( movement[i] == test_key) return 3;
        i++;
  } while (movement[i] != VK_DELETE);
  return 0; 
}

/*-----------------------------------------------------------------------------
 *     Delete previous character 
 *-----------------------------------------------------------------------------*/
BOOL backspace()
{
   keybd_event( VK_BACK, 0X0E, 0, 0);  // key down
   keybd_event( VK_BACK, 0X0E, 2, 0);  // key up
   Sleep (1);                          // Need some delay or it fails.  
   return 0;
}

/*-----------------------------------------------------------------------------
 *         Construct a valid lParam    
 *-----------------------------------------------------------------------------*/
LPARAM MakeLp(DWORD scancode, DWORD flags)
{
  DWORD lp;
  lp = ((scancode & 0xFF ) << 16)  | ((flags & 0xFF ) << 24);
  return (LPARAM) lp;
}

