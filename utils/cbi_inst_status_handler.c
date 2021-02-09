/*!
*-----------------------------------------------------------------------+
* File         :  cbi_inst_status_handler.c                             |
*                                                                       |
* Description  :  Takes appropriate action if the status of a given     |
*                 instrument's command execution is something other     |
*                 than fully successful, this can involve printing an   |
*                 informational message or taking steps to perform a    |
*                 low-level reset of misbehaving hardware.              |
*                                                                       |
* Arguments    :                                                        |
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


int cbi_inst_status_handler( CBI_COMMAND *cmd_list, int *command_index ) {

  char *func_name = (char *)__FUNCTION__;

  int iidx, iidx2, retval, i = 0;
  int recovery_flagged = CBI_FALSE;

  // Function pointer definitions for use below.
  int (*fptr1)()   = NULL;
  int (*fptr2)()   = NULL; // For '_post()' function calls.

  int status;

  printf("\n\nBegin post-command status check/recovery...\n\n");
  
  // Start examining the status of each online instrument
  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {

    // Note if recovery mode is necessary based on first
    // instrument encountered that exhibits an error condition.
    if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE ) {

      status = CTL_System.p_Module[iidx]->status;

      switch (status) {
      case CBI_F_SUCCESS:
	retval = CBI_F_SUCCESS;
	break;
	
	
      case CBI_F_HANDSHAKE_TIMEOUT:
      case CBI_F_COMM_ERROR:
      case CBI_F_HANDSHAKE_ERROR:
	if (!recovery_flagged) {
	  printf("====================================\n");
	  printf("======== INSTRUMENT RECOVERY =======\n");
	  printf("====================================\n");
	  recovery_flagged = CBI_TRUE;
	}
	// Set recovery flag on any instruments showing one
	// of the specified error codes.
	CTL_System.p_Module[iidx]->in_recovery = CBI_TRUE;

	printf("Attempting instrument %s recovery via reset... \n",
	       cbi_return_code_names[status] );
	// Determine index of the command to self-dispatch as a means of recovery.
	// Note: Each platform that needs to be supported should have a command
	//       with the name "RESET AND INIT" so that this code remains general.
	// TODO: Place in developer documentation platform requirements list.
	*command_index = cbi_command_idx_from_name( cmd_list, "RESET AND INIT" );
	// Set dispatch method to 'self-dispatched', to trigger
	// execution of the command specified immediately above.
	CTL_System.cmd_dispatch_method = CBI_SELF_CMD_DISPATCH;
	retval = CBI_F_SUCCESS;
	break;
	

      case CBI_F_FAILURE:
	printf("Prep function returned failure status.\n");
	retval = CBI_F_SUCCESS;
	break;


      default:
	break;
      }

    } //endIF

  } // endWHILE

  return retval;

}








