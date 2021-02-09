/*!
*-----------------------------------------------------------------------+
* File         :  fa_extract_section_order.c                            |
*                                                                       |
* Description  :  Extracts a list of all section names present in a     |
*                 config-style file and stores them in the provided     |
*                 array 'sections'.                                     |
*                 Uses single-access mode of faccess.                   |
*                                                                       |
* Arguments    : -Pointer to open file handle                           |
*                -Pointer to section name string (with delimiter chars) |
*                -Pointer to element name string                        |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int fa_extract_section_order( FILE *fp, char sections[][FA_MAX_STRING_LENGTH] ) {

  int status   = 0;
  int sect_cnt = 0;

  while (status == 0) {
    status = faccess( FA_READ, fp, NULL, NULL, (int)NULL, (int)NULL, STRING, sections[sect_cnt] );
    if (status == 0) {
      sect_cnt++;
    }
  }

  return 0;
}
