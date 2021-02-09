/*!
*-----------------------------------------------------------------------+
* File         :  cbi_write_param_file_header.c                         |
*                                                                       |
* Description  : Should be called prior to writing any changes to the   |
*                instrument sections of the parameter file.  This will  |
*                grab and increment a centralized index, kept in a file |
*                the path of which is specified in the application      |
*                config file and then write this new index to the       |
*                operational paramter file header.                      |
*                This will then also make a backup copy of the          |
*                parameter file in use, appending its (old) internal    |
*                index number to the filename in the current working    |
*                directory.                                             |
*                                                                       |
* Arguments    : FILE pointer to file to modify                         |
*                INT  index of file to modify                           |
*                BOOL Whether or not to perform this write as part of a |
*                     performance optimized file access session (TRUE)  |
*                     or an isolated file access call (FALSE).          |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_write_param_file_header(FILE *fp, int file_idx, int session_based) {

  char *header_section = "CONFIG_FILE_AUTOHEADER";

  int cidx, file_idx_in = 0;
  int status;

  struct tm  *loctime;
  time_t curtime;


  // Generate timestamp
  char mod_date[25];
  curtime = time(NULL);
  loctime = localtime(&curtime);
  strcpy( mod_date, asctime(loctime) );
  strip( mod_date );

  if (session_based == CBI_TRUE) {
    printf("Accessing file section...\n");
    faccess( FA_WRITE, fp, header_section, NULL, 0, 0, 0, NULL); 
  }

  status = faccess( FA_WRITE, fp, header_section, "LAST_MODIFIED", 1, 1, STRING, &mod_date );
  status = faccess( FA_WRITE, fp, header_section, "FILE_ID",       1, 1, INT,    &file_idx );

  return F_SUCCESS;
}



