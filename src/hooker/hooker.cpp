/*
 * ============================================================================
 *
 *       Filename:  hooker.cpp
 *
 *    Description:  hook, unhook and keyboard handler functions
 *
 *        Version:  1.0.0
 *        Created:  10/03/2015 
 *       Revision:  none
 *       Compiler:  gcc
 *
 * ============================================================================
 */

#define HOOK_EXPORTS
#include "hooker.h"

/*-----------------------------------------------------------------------------
 *                        DLL entry point.   
 *----------------------------------------------------------------------------*/
  // Uncomment if in Visual Studio
  // BOOL APIENTRY DllMain( HANDLE hModule, 
int main( int hModule, 
                       char **  ul_reason_for_call, 
                       char ** lpReserved )
{
    hinstance = (HINSTANCE)hModule;
    hook = NULL;
    return TRUE;
}  

/*-----------------------------------------------------------------------------
 *                        Hook the keyboard. 
 *----------------------------------------------------------------------------*/
HOOKDLL_API HHOOK installhook()
{
    hook = SetWindowsHookEx(
           WH_KEYBOARD_LL,                   // Low Level keyboard hook
           hookproc,                         // The hook hander procedure
           // Next parameter should be just  "hinstance" 
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
      DWORD tid = GetWindowThreadProcessId(GetForegroundWindow(),0);
      lpgui.cbSize = sizeof(GUITHREADINFO);
      GetGUIThreadInfo( tid, &lpgui );
      target_window = lpgui.hwndFocus;

      //----------------------------------
      // Return unless Greek keyboard
      //----------------------------------
      localeID = GetKeyboardLayout(tid);
      if (LOBYTE(LOWORD((HKL)localeID)) != LANG_GREEK)  
        return  NEXT_HOOK;
      
      //----------------------------------
      // Begin processing key
      //----------------------------------
      DWORD vkey = key->vkCode;
      GetKeyState(0);                      // Needed because of an API bug.
      GetKeyboardState(ks);                // Read the keyboard state  


      //----------------------------------
      //      Movement keys 
      //----------------------------------
      // If moved away from the typed character 
      // our character buffer has no meaning.
      // Reset temp char buffer and return
      //----------------------------------
      for (i=0; i < 9 ; i++)
      {
        if ( movement[i] == vkey)
          {
            w[1] = w[0] = '\0';
            return NEXT_HOOK;
          }
      }

      //----------------------------------
      // Test for dead keys (;, : or W)
      //----------------------------------
      // If a dead key, store the accent and return.
      // It will be put on the next character, if pertinent.
      //
      // ; -> 1   Tonos
      // : -> 2   Dialytika
      // W -> 3   Dialytika and Tonos
      //----------------------------------
      if (vkey == VK_OEM_1)                                 // Test for ; or :
      {   
        (ks[VK_SHIFT] & 0X80)? dead_key = 2: dead_key = 1;
        return NO_KEY;                                      // Gobble up the key
      }
      if ((vkey==0X57) && (ks[VK_SHIFT] & 0X80))            // test for SHIFT-W
      {     
        dead_key = 3;
        return NO_KEY;
      }

      //--------------------------------------------------
      //          Test for vowels.
      //--------------------------------------------------
      for (i=0; i < 20 ; i++)
      {
        if ( vowels[i] == vkey) 
        {
          //----------------------------------------------------------------
          // Break out of the loop if the key ' (") or "-"
          // do not follow the dead key (;) 
          // They are post placed accents (dialytika or macron)
          // and they dealt with by the next loop.
          //----------------------------------------------------------------
          if ( ( (vkey == 0XDE) || (vkey == 0XBD) ) && (!dead_key) ) break;
          
          //----------------------------------
          // If a dead key has been stored
          // from the previous keypress
          // emit the appropriate accented vowel
          // or special symbol
          //----------------------------------
          if (dead_key)                                   
          {                                              
            // SHIFT key or CAPS LOCK has been pressed?
            // Set caps flag 
            BOOL caps = (ks[VK_SHIFT] & 0X80)   || (ks[VK_CAPITAL] & 0X01); 
            switch (vkey)                                  
            {
              case 0X41 : caps ? w[0] = 0X386 : w[0] = 0X3AC; break;  // A
              case 0X45 : caps ? w[0] = 0X388 : w[0] = 0X3AD; break;  // E
              case 0X48 : caps ? w[0] = 0X389 : w[0] = 0X3AE; break;  // H
              case 0X4f : caps ? w[0] = 0X38C : w[0] = 0X3CC; break;  // O
              case 0X56 : caps ? w[0] = 0X38F : w[0] = 0X3CE; break;  // V (Omega)
                        
                          // The next two may take dialytika or/and tonos           
              case 0X49 : switch (dead_key)                           // I
                          {                                
                            case 1: caps ? w[0] = 0X38A : w[0] = 0X3AF; break;
                            case 2: caps ? w[0] = 0X3AA : w[0] = 0X3CA; break;
                            case 3: w[0] = 0X390; break;
                          } break;
              case 0X59 : switch (dead_key)                           // Y
                          {
                            case 1: caps ? w[0] = 0X38E : w[0] = 0X3CD; break;
                            case 2: caps ? w[0] = 0X3AB : w[0] = 0X3CB; break;
                            case 3: w[0] = 0X3B0; break;
                          } break;
              //----------------------------------------------
              // Special symbols entered with the dead key.
              //----------------------------------------------
              case 0X51 : caps ? w[0] = 0X3Df : w[0] = 0X3D9; break;  // Q (kopa)
              case 0X53 : caps ? w[0] = 0X3DA : w[0] = 0X3DB; break;  // S (stigma)
              case 0X4B : w[0] = 0X3D7; break;                        // K (kai)
              case 0X50 : caps ? w[0] = 0X3E1 : w[0] = 0X3E0; break;  // P (sampi)
              case 0X46 : caps ? w[0] = 0X3DC : w[0] = 0X3DD; break;  // F (digamma)
              case 0X34 : w[0] = 0X20AC; break;                       // $ (Euro)
                        // next key (. or >) is for ano teleia or right eisag 
              case 0XBE : caps ? w[0] = 0XBB : w[0] = 0X387; break;   
              case 0X33 : caps ? w[0] = 0X375 : w[0] = 0X374; break;  // # (keraies)
              case 0XBC : if(caps) w[0] = 0XAB; else  return NEXT_HOOK; break; //< (L eisag.); 
              case 0XDE : w[0] = 0X1FBD; break;                        // ' Apostrophos
              case 0XBD : caps ? w[0] = 0X2014 : w[0] = 0X2013; break; // - (pavles)
                       // Return hook if no appropriate key is found
              default:    dead_key = 0; return NEXT_HOOK;  
            } // -- End case
            PRINT(w[0]);
            dead_key = 0;       // Reset dead key flag and print character
            // Save accented character in case is needed
            //         to add further decorations
            w[1] = w[0];       
            return NO_KEY;  // We have allready printed our character 
          } //-- End if dead_key
          // If no dead key is on, it must be an ordinary voweel.
          // Convert vowel to UCS 0XHHHH.
          UCS(w);
          // Store vowel in case we need to decorate it
          w[1] = w[0];         
          return NEXT_HOOK;
        } // -- End if vowel[i]
      } // -- end for i 
      
      //--------------------------------------------------
      //       Test for accent keys.
      //  If key is a post placed accent
      //--------------------------------------------------
      for (i=0; i < 9 ; i++)
      {
        if ( accent_keys[i] == vkey ) 
        {
          /* keys '^' and '~' must be shifted or we loose '6' and '`' */
          if ( 
              ( (vkey == 0X36) || (vkey == 0XC0)) 
                                   && 
                        ( !(ks[VK_SHIFT] & 0X80) )
             ) return NEXT_HOOK;

          // Macro Converts vkey to UCS (0XHHHH) accent_char.
          UCS(accent_char);
          accent_char[1]=L'\0';
          
          //FIXME remove type casting.
          w[0] = put_accent ( (unsigned int)w[1], (unsigned char)accent_char[0] );  
          if (w[0] == w[1])
          {
            return NO_KEY;              // Could not put the accent, do nothing
          } 
          else
          {
            backspace();                // Delete previous character.
            PRINT(w[0]);                // and print accented character
            w[1] = w[0];
            return NO_KEY;      // We have printed our own do not pass key to OS
          }
        } // -- end if accent_keys
      } // -- end for i
      // Key is of no interest to us.
      // Pass key back to OS to do as it pleases.
      return NEXT_HOOK; 
	} // End if HC_KEY
	return 0;  // Needed to stop the stupid gcc compiler warnings.
} // --- End of hook handler


/*-----------------------------------------------------------------------------
 *     Delete previous character 
 *----------------------------------------------------------------------------*/
BOOL backspace()
{
   keybd_event( VK_BACK, 0X0E, 0, 0);  // key down
   keybd_event( VK_BACK, 0X0E, 2, 0);  // key up
   Sleep (1);                          // Need some delay or it fails.  
   return 0;
}

/*-----------------------------------------------------------------------------
 *         Construct a valid lParam    
 *----------------------------------------------------------------------------*/
LPARAM MakeLp(DWORD scancode, DWORD flags)
{
  DWORD lp;
  lp = ((scancode & 0xFF ) << 16)  | ((flags & 0xFF ) << 24);
  return (LPARAM) lp;
}

