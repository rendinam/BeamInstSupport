/*!
*-----------------------------------------------------------------------+
* File         :  load_command.c                                        |
*                                                                       |
* Description  :  Appends a CBI_COMMAND command specification structure |
*                 to the central sequence queue of commands that gets   |
*                 iterated over when executing commands.                |
*                                                                       |
* Arguments    :  Pointer to string                                     |
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


int cbi_load_command( CBI_COMMAND *cmd_list, int command_list_len, char *command_name, int provide_prep_call ) {

  int cspec;
  int debug_level = CTL_System.debug_verbosity;

  // FIXME: finalize platform index (0) in num_platform_commands

  if ( command_name != NULL ) {

    for (cspec = 0; cspec < command_list_len; cspec++) {
      
      if ( strcmp(cmd_list[cspec].name, command_name) == 0 ) {

	if (debug_level > CBI_DEBUG_LEVEL_1) {
	  printf("Adding command %s to load slot\n", cmd_list[cspec].name);
	}
	CTL_System.loaded_command = cmd_list[cspec];

	// If this is a component of a sequence wherein
	// command parameters are provided programmatically,
	// one may skip the prep call normally associated
	// with the command in question.
	if (!provide_prep_call) { 
	  CTL_System.loaded_command.prep_func = NULL;
	}
                                  
      }
      
    }
    
  } else {
    
    CTL_System.loaded_command.command   = CBI_INVALID_CMD_0;
    CTL_System.loaded_command.prep_func = NULL;
    CTL_System.loaded_command.post_func = NULL;

  }

  
  return CBI_F_SUCCESS;
}
  



int cbi_command_idx_from_name( CBI_COMMAND *cmd_list, char *command_name ) {

  char *func_name = (char *)__FUNCTION__;

  int retval, cspec;
  int num_sequence_commands = 0;

  if ( command_name != NULL ) {

    for (cspec = 0; cspec < CTL_System.itype.num_platform_commands; cspec++) {
      if ( strcmp(cmd_list[cspec].name, command_name) == 0 ) {

	retval = cspec;
                            
      }
    }

  } else {
    
    sprintf(message, "ERROR determining command index from name.\n");
    log_message(S_ERROR, func_name, message);
  }

  return retval;

}
