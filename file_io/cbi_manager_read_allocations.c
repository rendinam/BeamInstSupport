/*!
*-----------------------------------------------------------------------+
* File         :  cbi_manager_read_allocations.c                        |
*                                                                       |
* Description  :  Responsible for displaying the menu of all online     |
*                 instruments assigned to all back-end servers the the  |
*                 present manager session.  Processes user input for    |
*                 selecting instrument activity and formats those       |
*                 requests for propagation to the various back-ends.    |
*                                                                       |
* Arguments    :  - Number of back-end processes that need selection    |
*                   support.                                            |
*                 - 2D array of selection specifiers to provide to the  |
*                   input handler for sending to the back-ends.         |
*                                                                       |
*          TODO: Read in all allocations upon application startup, not  |
*                each time a selection is requested.                    |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define    MAX_INST_LINES       10
#define    MAX_INSTS_PER_LINE   10


int cbi_manager_read_allocations( int num_backends ) {

  char* func_name = (char *)__FUNCTION__;
  int   debug_level = CTL_System.debug_verbosity;

  char alloc_scheme[CBI_MAX_STRING_LENGTH];
  char alloc_name[CBI_MAX_STRING_LENGTH];

  int  inst_count = 0;

  int  inst, sect_cnt, matched, rval;
  int  alloc_idx, retcode = 0;
  char instring[50];
  int local_iidx;

  // Local list for gathering instrument names
  char inst_list[CBI_MAX_BACKENDS][CBI_MAX_INSTRUMENTS][10];

/*   typedef struct { */
/*     int  backend_ID; */
/*     int  local_iidx; */
/*     char inst_name[10]; */
/*   } SELECTION_INFO; */
  
/*   SELECTION_INFO assignments[CBI_MAX_INSTRUMENTS]; */

  // Local list for gathering parameter file section names
  char sections[FA_MAX_FILE_SECTIONS][FA_MAX_STRING_LENGTH];

  char *filename;
  filename = (char *)&(appconfig.inst_alloc_file);

  FILE *fp, *fpm;
  char error[200];


  int input;



  //------------------------------------------
  // Initialize incoming instrument list and
  // file sections list.
  //------------------------------------------
  int backend;
  // Initialize each back-end's inst name arrays
  for (backend = 0; backend < CBI_MAX_BACKENDS; backend++) {
    for (inst = 0; inst < CBI_MAX_INSTRUMENTS; inst++) {
      strcpy( inst_list[backend][inst], "");
    }
  }
  // Initialize mergelist struct array
  for (inst = 0; inst < CBI_MAX_INSTRUMENTS; inst++) {
    mgrconfig.assignments[inst].backend_ID = 0;
    strcpy( mgrconfig.assignments[inst].inst_name, "" );
  }


  // Initialize for file section navigation
  for (inst = 0; inst < FA_MAX_FILE_SECTIONS; inst++) {
    strcpy(sections[inst], "");
  }


  for (backend = 0; backend < num_backends; backend++) {

    local_iidx = 1;
    mgrconfig.num_instruments[backend] = 0;

    //printf("Opening allocation file...\n");
    fp = fopen(filename, "r");
    if (fp == NULL) {
      sprintf(error, "Error opening allocation file %s ", filename);
      perror( error );
      return CBI_F_FAILURE;
    }

    strcpy(alloc_scheme, mgrconfig.alloc_schemes[backend] );
    strcpy(alloc_name,   mgrconfig.alloc_names[backend] );


    //printf("Looking in file %s\n", filename);
    
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
    
    //printf("Reading in the allocation  \"%s:%s\" ... ", alloc_scheme, alloc_name );
    rval = faccess(FA_READ, fp, alloc_scheme, list_token, MAX_INST_LINES, MAX_INSTS_PER_LINE, STRING, inst_list[backend] );
    if (rval == 1 || rval == 2) {
      printf("faccess returned: %d  when reading in allocation.  Check the file syntax and the spelling of the\n", rval);
      printf("allocation name and try again.\n");
      exit(1);
    }
    if ( fclose(fp) != 0 ) {
      perror("fclose: ");
    }
    //printf("done.\n");
    
    
    //------------------------------------------
    // Extract and store file section order
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
	//if (strcmp(sections[sect_cnt], "") != 0) {
	//  printf("%s\n", sections[sect_cnt]);
	//}
      }
      printf("\n\n");
    }
    
    
    //------------------------------------------
    // For each instrument successfully read...
    //------------------------------------------
    for (inst = 0; inst < CBI_MAX_INSTRUMENTS; inst++) {
      if (strcmp(inst_list[backend][inst], "") != 0) {
	
	matched = FALSE;
	
	for (sect_cnt = 0; sect_cnt < FA_MAX_FILE_SECTIONS; sect_cnt++) {
	  if ( strcmp(sections[sect_cnt], "") != 0 &&
	       strcmp(sections[sect_cnt], inst_list[backend][inst]) == 0) {
	    if (debug_level > CBI_DEBUG_LEVEL_3) {
	      printf("%s matched! \n", inst_list[backend][inst]);
	    }
	    
	    mgrconfig.assignments[inst_count].backend_ID = backend+1;
	    mgrconfig.assignments[inst_count].local_iidx = local_iidx;
	    mgrconfig.num_instruments[backend]++;
	    local_iidx++;
	    strcpy( mgrconfig.assignments[inst_count].inst_name, inst_list[backend][inst] );
	    inst_count++;
	    
	    matched = TRUE;
	  }
	}
	
      } //endIF strcmp(inst_list[backend][inst])
    } //endFOR i
    

  } // endFOR num_backends



  //--------------------------------------------------------
  // Display list of online instruments from collected info
  //--------------------------------------------------------
  inst_count = 0;
  while ( strcmp(mgrconfig.assignments[inst_count].inst_name, "") != 0 ) {
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("%3d)  %s\n", inst_count+1, mgrconfig.assignments[inst_count].inst_name);
    }
    inst_count++;
  }


  return CBI_F_SUCCESS;

}
