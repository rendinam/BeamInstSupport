/*!
*-----------------------------------------------------------------------+
* File         :  cbi_command_status_summary.c                          |
*                                                                       |
* Description  :  Prints a summary of command execution status for each |
*                 instrument what was part of the request.              |
*                                                                       |
* Arguments    :  Stream file handle for directing output.              |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

//ACC-release includes
#include "CESR_platform.h"  // Platform macro definitions
#include "utilities.h"      //
#include "messages.h"       // Status and logging message capabilities

#include "cbi_core_includes.h"


#define MAX_COLUMNS                   2


int cbi_command_status_summary( FILE *fp, char *name ) {

  char *func_name = (char *)__FUNCTION__;

  int iidx, inst_cnt = 0;
  int retval = CBI_F_SUCCESS;
  char active;
  int col, row;
  int entries_per_column;
  int entry, max_rows, row_cnt = 1;
  int entries_in_row;

  int n_online = num_online();
  int i;

  fprintf(fp, "\n                                == %s Command Status Summary ==\n\n", name);
  
    int error_list[50], num_errors;
    
  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    if (CTL_System.p_Module[iidx]->active == CBI_ACTIVE) {
                
      num_errors = cbi_get_errors(CTL_System.p_Module[iidx]->core->cbi_cmd.error, error_list, CTL_System.itype.num_instrument_error_names);
      
      if (num_errors > 0) {
        fprintf(fp, "Error(s) for %s: ",CTL_System.p_Module[iidx]->det.location);
        for( i = 0; i<num_errors; i++) {
          fprintf(fp, "%3i, ", error_list[i]);
        }
        fprintf(fp, "\n");

        for( i = 0; i<num_errors; i++) {
          fprintf(fp, "%2i: %s\n", error_list[i], CTL_System.itype.instrument_error_names + (error_list[i]*CBI_MAX_ROUTINE_ERROR_LENGTH) );
        }
              
        fprintf(fp, "\n");
      }
    }
  }  
    
  max_rows = ceilf((float)(n_online / (float)MAX_COLUMNS));

  if (max_rows < 1) {
    max_rows = n_online;
  }


  for (col = 0; col < MAX_COLUMNS; col++) {
    fprintf(fp, " Instrument    Location    Active            Status           |");
  }
  fprintf(fp, "\n");
  for (col = 0; col < MAX_COLUMNS; col++) {
    fprintf(fp, "--------------------------------------------------------------|");
  }
  fprintf(fp, "\n");


  iidx = 0;
  for (row = 0; row < max_rows; row++) {

    entries_in_row = MAX_COLUMNS;

    for (entry = 0; entry < entries_in_row; entry++) {
      if (inst_cnt < n_online) {
        if( CTL_System.p_Module[iidx+(entry*max_rows)]->active == CBI_ACTIVE) {
          active = '*';
        } else {
          active = ' ';
        }
        // Note convention for storing instrument names.
        fprintf(fp, "  %-11s   %-11s  %c     (%d) %-21s  |",
        CTL_System.p_Module[iidx+(entry*max_rows)]->comm.ethernet.hostname,
        CTL_System.p_Module[iidx+(entry*max_rows)]->det.location,
        active,
        CTL_System.p_Module[iidx+(entry*max_rows)]->status, 
        cbi_return_code_names[CTL_System.p_Module[iidx+(entry*max_rows)]->status] );
      }
      inst_cnt++;
    }
    iidx++;
    fprintf(fp, "\n");


    if (row_cnt == max_rows) {
      break;
    } else {
      row_cnt++;
    }
    

  } // endFOR row
  
  
  return retval;

}








