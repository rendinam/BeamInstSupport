/*!-----------------------------------------------------------------------+
* File         :  cbi_new_file_idx.c                                      |
*                                                                         |
* Description  :  Obtains a unique sequential index number to use in      |
*                 naming an output file.                                  |
*                                                                         |
* Arguments    :  -Flag to determine if the external index counter gets   |
*                    incremented during the function call.                |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_new_file_idx(int dirspec, int bump) {

  int index, err;
  int status;

  char idx_fname[CBI_MAX_STRING_LENGTH];

  // Create full file path for index tracking
  // file from app configruation info.
  //------------------------------------------------------
  FILE *fpi = NULL;
  strcpy( idx_fname, "" );
  strcat( idx_fname, appconfig.directories[dirspec] );
  strcat( idx_fname, "/file.idx" );


  // Test for file existence
  fpi = fopen(idx_fname, "r");
  
  if ( fpi == (int)NULL ) {
    printf("Index counter file does not exist.\n");
    printf("Creating the file \'%s\' now... ", idx_fname);
    fpi = fopen(idx_fname, "w");
    fprintf(fpi, "0");
    printf("Done\n");
  }
  if (fclose(fpi) == EOF) {
    perror("fclose: ");
  }
  //------------------------------------------------------



  // File exists, open with r+ for write blocking and extract
  // index, bump if necessary.
  int attempts = 5;
  int attempt;
  for (attempt = 0; attempt < attempts; attempt++) {
    fpi = fopen(idx_fname, "r+");    

    if (fpi == NULL) {
      if (fclose(fpi) == EOF) {
	perror("fclose: ");
      }
      cbi_sleep( 100 );  
      continue;
    }
    // grab index value
    err = fscanf(fpi, "%d", &index);
    break;
  }


  // If requested, bump index and save.
  if (bump) {
    rewind(fpi);
    fprintf(fpi, "%d\n", index+1);
  }

  if (fclose(fpi) == EOF) {
    perror("fclose: ");
  }

  return index;

}

