/*!
*-------------------------------------------------------------------------+
* File         :  cbi_execute_command.c                                   |
*                                                                         |
* Description  :  Simple function for iterating over a collection of      |
*                 instruments and executing the command that has been set |
*                 up by any parameter downloads and a init_command() call |
*                                                                         |
* Arguments    :  (int) Index of instrument upon which to initiate        |
*                 the command.                                            |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"
#include "messages.h"

#include "cbi_core_includes.h"


int cbi_execute_command( int BPM_IDX ) { 

  char *func_name = (char *)__FUNCTION__;
  int status, retval = CBI_F_SUCCESS;

  status = cbi_gp_struct(WRITE, CTL_System.comm_method, CBI_CMD_TAG, CTL_System.p_Module[BPM_IDX], CBI_FULL_STRUCT);
  if (status != F_SUCCESS) {
    sprintf(message, "Command execution error for instrument %s @ %s", 
	    CTL_System.p_Module[BPM_IDX]->comm.ethernet.hostname,
	    CTL_System.p_Module[BPM_IDX]->det.location);
    log_message(S_ERROR, func_name, message);
    retval = CBI_F_COMM_ERROR;
  }

  return retval;

}
