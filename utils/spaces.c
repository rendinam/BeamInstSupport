/*!
*-------------------------------------------------------------------------+
* File         :  spaces.c                                                |
*                                                                         |
* Description  :  Prints the requested number of spaces into the open     |
*                 file pointed to by fp.                                  |
*                                                                         |
* Arguments    :  FILE  fp      - Pointer to open file                    |
*                 int   padding - Number of spaces to print into file     |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int spaces( FILE *fp, int padding ) {
  
  int i;
  for ( i = 0; i < padding; i++) {
    fprintf(fp, " ");
  }
  return CBI_F_SUCCESS;
}
