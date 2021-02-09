/*!
*-----------------------------------------------------------------------+
* File         :  cbi_read_allocation.c                                 |
*                                                                       |
* Description  :  Read in a particular instrument allocation definition |
*                 list.                                                 |
*                                                                       |
* Arguments    :  Pointer to instrument allocation definition file.     |
*                 Pointer to holding array of master control structs to |
*                  populate.                                            |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define    MAX_INST_LINES       10
#define    MAX_INSTS_PER_LINE   10


int cbi_read_allocation( char *filename, char *alloc_scheme, char *alloc_name ) {
  
  char* func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int i, sect_cnt, matched, rval, numBPMs = 0;
  int alloc_idx, retcode = 0;
  char instring[50];

  // Local list for gathering instrument names
  char inst_list[CBI_MAX_INSTRUMENTS][10]; 
  // Local list for gathering parameter file section names
  char sections[FA_MAX_FILE_SECTIONS][FA_MAX_STRING_LENGTH];


  // Initialize name slots in master struct array, detector substructure.
  for (i = 0; i < CBI_MAX_INSTRUMENTS; i++) {
    strcpy( CTL_System.p_Module[i]->det.name, "" );
  }


  FILE *fp, *fpm;
  char error[200];
  fp = fopen(filename, "r");
  if (fp == NULL) {
    sprintf(error, "Error opening allocation file %s ", filename);
    perror( error );
    exit(1);
  }


  //------------------------------------------
  // Initialize incoming instrument list and
  // file sections list.
  //------------------------------------------
  for (i = 0; i < CBI_MAX_INSTRUMENTS; i++) {
   strcpy( inst_list[i], "");
  }
  for (i = 0; i < FA_MAX_FILE_SECTIONS; i++) {
    strcpy(sections[i], "");
  }

  printf("Looking in file %s\n", filename);

  int  element = 1;
  char name_token[10] = {""};
  while( retcode == 0 ) {
    sprintf( name_token, "NAME_%d", element );
    retcode = faccess( FA_READ, fp, alloc_scheme, name_token, 1, 1, STRING, instring );
    if ( retcode == 0 ) {
      if ( strcmp(instring, alloc_name) == 0 ) {
	break;
      }
      element++;
    } else {
      printf("Error reading allocation file.  Please check that the allocations are defined correctly.\n");
    }
  }
  rewind(fp);

  char list_token[10] = {""};
  sprintf( list_token, "LIST_%d", element );

  printf("Reading in the allocation  \"%s:%s\" ... ", alloc_scheme, alloc_name );
  rval = faccess(FA_READ, fp, alloc_scheme, list_token, MAX_INST_LINES, MAX_INSTS_PER_LINE, STRING, inst_list );
  if (rval == 1 || rval == 2) {
    printf("faccess returned: %d  when reading in allocation.  Check the file syntax and the spelling of the\n", rval);
    printf("allocation name and try again.\n");
    exit(1);
  }
  if ( fclose(fp) != 0 ) {
    perror("fclose: ");
  }
  printf("done.\n");


  //------------------------------------------
  // Extract and store detector parameter 
  // file section order
  //------------------------------------------
  fpm = fopen(appconfig.det_param_file, "r");
  fa_extract_section_order(fpm, sections );
  if ( fclose(fpm) != 0) {
    perror("fclose: ");
  }
  for (sect_cnt = 0; sect_cnt < FA_MAX_FILE_SECTIONS; sect_cnt++) {
    replace(sections[sect_cnt], '[', ' ');
    replace(sections[sect_cnt], ']', ' ');
    strip(sections[sect_cnt]);
  }
  if (debug_level >= CBI_DEBUG_LEVEL_5) {
    for (sect_cnt = 0; sect_cnt < FA_MAX_FILE_SECTIONS; sect_cnt++) {
      if (strcmp(sections[sect_cnt], "") != 0) {
	printf("%s\n", sections[sect_cnt]);
      }
    }
    printf("\n\n");
  }


  //------------------------------------------
  // For each instrument successfully read...
  //------------------------------------------
  for (i = 0; i < CBI_MAX_INSTRUMENTS; i++) {
    if (strcmp(inst_list[i], "") != 0) {

      matched = FALSE;
      
      for (sect_cnt = 0; sect_cnt < FA_MAX_FILE_SECTIONS; sect_cnt++) {
	if ( strcmp(sections[sect_cnt], "") != 0 &&
	     strcmp(sections[sect_cnt], inst_list[i]) == 0) {
	  if (debug_level > CBI_DEBUG_LEVEL_4) {
	    printf("%s matched!  --  Copying name to master struct array. \n", inst_list[i]);
	  }
	  strcpy( CTL_System.p_Module[i]->det.name, inst_list[i] );
	  matched = TRUE;
	  numBPMs++;
	}
      }
      
    } //endIF strcmp(inst_list[i])
  } //endFOR i


  //-------------------------------------------------------------------------  
  // All went well, assign an MPM control array identifier to this
  // program instance to distinguish it from other servers if it ends up
  // registering itself as a server process in that array.
  // This starts numbering CBPM-II program instances at 10.
  //-------------------------------------------------------------------------  
  CTL_CBPM_Intf_Monitor.ProcessID = (element-1) + 10;
  printf("\nThis program instance will become server ID [%d] if placed in server mode.\n",
	 CTL_CBPM_Intf_Monitor.ProcessID );


  return numBPMs;

}
