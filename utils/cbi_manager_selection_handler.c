/*!
*-----------------------------------------------------------------------+
* File         :  cbi_manager_selection_handler.c                       |
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


int cbi_manager_selection_handler( int num_backends, char sel_specs[][CBI_SELEC_SPEC_LENGTH] ) {

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

  typedef struct {
    int  backend_ID;
    int  local_iidx;
    char inst_name[10];
  } SELECTION_INFO;
  
  SELECTION_INFO assignments[CBI_MAX_INSTRUMENTS];

  // Local list for gathering parameter file section names
  char sections[FA_MAX_FILE_SECTIONS][FA_MAX_STRING_LENGTH];

  char *filename;
  filename = (char *)&(appconfig.inst_alloc_file);

  FILE *fp, *fpm;
  char error[200];


  int input, backend;


  //--------------------------------------------------------
  // Display list of online instruments from collected info
  //--------------------------------------------------------
  inst_count = 0;
  while ( strcmp(mgrconfig.assignments[inst_count].inst_name, "") != 0 ) {
    printf("%3d)  %s\n", inst_count+1, mgrconfig.assignments[inst_count].inst_name);
    inst_count++;
  }


  int idx1;
  printf("Instrument activation:  (a)ll, (n)one, (r)ange toggle, (i)nvert all, (s)ingle toggle, (q)uit : ");
  input = cbi_get_char_local();


  switch (input) {

  case (int)'a':    case (int)'A':
    printf("Activating ALL online instruments\n");
    for (backend = 0; backend < num_backends; backend++) {
      strcpy( sel_specs[backend], "a\0" );
    }
    break;

  case (int)'n':    case (int)'N':
    printf("Deactivating ALL online instruments\n");
    for (backend = 0; backend < num_backends; backend++) {
      strcpy( sel_specs[backend], "n\0" );
    }
    break;

  
    // Calculate mapping here for range of instruments onto the subsets
    // for which each back-end is responsible.

/*   case (int)'r':    case (int)'R': */
/*     printf("Specify beginning index : "); */
/*     idx1 = cbi_get_int(); */
/*     if (idx1 < 0 || idx1 > num_online() ) { */
/*       printf("\nValid input is in the range 0-%d\n\n", num_online() ); */
/*       cbi_sleep(1.25); */
/*       break; */
/*     } */
/*     printf("Specify ending index    : "); */
/*     idx2 = cbi_get_int(); */
/*     if (idx2 < idx1) { */
/*       printf("\nSecond index needs to be greater than the first.\n\n"); */
/*       cbi_sleep(1.25); */
/*       break; */
/*     } */
/*     if (idx2 < 0 || idx2 > num_online() ) { */
/*       printf("\nValid input is in the range 0-%d\n\n", num_online() ); */
/*       cbi_sleep(1.25); */
/*       break; */
/*     } */
/*     for (inst = idx1; inst <= idx2; inst++) { */
/*       if (CTL_System.p_Module[inst]->active == CBI_INACTIVE) { */
/* 	CTL_System.p_Module[inst]->active = CBI_ACTIVE; */
/* 	continue; */
/*       } */
/*       if (CTL_System.p_Module[inst]->active == CBI_ACTIVE) { */
/* 	CTL_System.p_Module[inst]->active = CBI_INACTIVE; */
/* 	continue; */
/*       } */
/*     } */
/*     break; */

  case (int)'i':    case (int)'I':
    for (backend = 0; backend < num_backends; backend++) {
      strcpy( sel_specs[backend], "i\0" );
    }
    break;


  case (int)'s':    case (int)'S':
    printf("Enter index of instrument to toggle: ");
    idx1 = cbi_get_int_local();
    printf("\n item %d   =    \"%s\" (iidx %d) on back-end #%d \n", 
	   idx1, 
	   mgrconfig.assignments[idx1].inst_name,
	   mgrconfig.assignments[idx1].local_iidx,
	   mgrconfig.assignments[idx1].backend_ID );
    idx1--;
    for (backend = 0; backend < num_backends; backend++) {
      if (backend+1 == mgrconfig.assignments[idx1].backend_ID) {
	sprintf( sel_specs[backend], "-s-%d\0", mgrconfig.assignments[idx1].local_iidx );
      } else {
	strcpy( sel_specs[backend], "*\0" );
      }
      printf("%s\n", sel_specs[backend]);
    }
    break;


  case (int)'q':    case (int)'Q':
    for (backend = 0; backend < num_backends; backend++) {
      strcpy( sel_specs[backend], "q\0" );
    }
    break;

  default:
    for (backend = 0; backend < num_backends; backend++) {
      strcpy( sel_specs[backend], "q\0" );
    }
    break;
  }



  return CBI_F_SUCCESS;

}
