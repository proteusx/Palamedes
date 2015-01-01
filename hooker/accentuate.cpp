/******************************************************************/
/* accentuate.cpp                                                 */
/*                                                                */
/* Put a specified accent on a given character                    */
/* Inputs: U+0XHHHH , accent                                      */
/* Return: Accented character if exists.                          */
/*         Same character unchanged if input is not recognized or */
/*         if no appropriate accented character found.            */
/******************************************************************/

#define _CRT_SECURE_NO_WARNINGS

// VS only
// #pragma once

#include <string.h>
#include "gr_map.h"

int put_accent ( unsigned int char_index_in, unsigned char accent)
{
  ucs_struct char_tmp;
  char_tmp.index = 0;
  int i,j;
  // Find character with the given UCS index
  for (i=0; i < 273; i++)
  {
    // if found decorate it with the the given accent
    if (ucs_array[i].index == char_index_in)
    {
      char_tmp = ucs_array[i];
      for (j=0; j < 10; j++)
      {
        if ( accents[j].symbol == accent)
        {
          switch (accents[j].pos)
          {
            case 2: strcpy(char_tmp.pneuma,  accents[j].name); break;
            case 3: strcpy(char_tmp.tonos,   accents[j].name); break;
            case 4: strcpy(char_tmp.ypo,     accents[j].name); break;
            // Unknown accent position, return original character.        
            default: return char_index_in;  
          }
          break;
        }
      }
      break;
    }  
  }
  // Character not recognized, return original character
  if (char_tmp.index == 0) return char_index_in;

  // Find the UCS index of the new accented character.
  for (i=0; i < 273; i++)
  {
    if ( 
         (!strcmp(ucs_array[i].name, char_tmp.name)) 
                           &&
        (ucs_array[i].shift == char_tmp.shift) 
                          &&
        (!strcmp(ucs_array[i].pneuma, char_tmp.pneuma)) 
                          &&
        (!strcmp(ucs_array[i].tonos, char_tmp.tonos)) 
                         &&
        (!strcmp(ucs_array[i].ypo, char_tmp.ypo) )
      )
    {
      return ucs_array[i].index;
    }
  }
  // No accented char found, return original char.
  return char_index_in;          
}
 
