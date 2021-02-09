/*!-----------------------------------------------------------------------+
* File         :  cbi_copy_file.c                                         |
*                                                                         |
* Description  :  Simply copies a file on disk using filenames            |
*                                                                         |
* Arguments    :  -Pointer to string with file name to copy               |
*                 -Pointer to string with destination file name           |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_copy_file(char *dest, char *from) {

  char filename[CBI_MAX_STRING_LENGTH];
  char ch;
  FILE *fpf, *fpd;

  fpf = fopen(from, "r");
  fpd = fopen(dest, "w");

  while(!feof(fpf)) {
    ch = fgetc(fpf);
    if(ferror(fpf)) {
      printf("Error reading source file.\n");
      exit(1);
    }
    if(!feof(fpf)) fputc(ch, fpd);
    if(ferror(fpd)) {
      printf("Error writing destination file.\n");
      exit(1);
    }
  }
  
  if(fclose(fpf)==EOF) {
    printf("Error closing source file.\n");
    exit(1);
  }

  if(fclose(fpd)==EOF) {
    printf("Error closing destination file.\n");
    exit(1);
  }
  
  return F_SUCCESS;

}

