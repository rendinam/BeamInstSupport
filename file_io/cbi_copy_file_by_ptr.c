/*!-----------------------------------------------------------------------+
* File         :  cbi_copy_file_by_ptr.c                                  |
*                                                                         |
* Description  :  Simply copies the contents of a file on disk using      |
*                 pointers to already opened files.  Rewinds the file     |
*                 pointers for each file but does not close the files at  |
*                 the conclusion of the copy operation.                   |
*                                                                         |
* Arguments    :  -Pointer to destination file                            |
*                 -Pointer to source file                                 |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_copy_file_by_ptr(FILE *fpd, FILE *fps) {

  char filename[CBI_MAX_STRING_LENGTH];
  char ch;

  while(!feof(fps)) {
    ch = fgetc(fps);
    if(ferror(fps)) {
      printf("Error reading source file.\n");
      exit(1);
    }
    if(!feof(fps)) fputc(ch, fpd);
    if(ferror(fpd)) {
      printf("Error writing destination file.\n");
      exit(1);
    }
  }

  rewind(fpd);
  rewind(fps);

  return F_SUCCESS;

}

