/*!
*-----------------------------------------------------------------------+
* File         :  cbi_init_command.c                                    |
*                                                                       |
* Description  :  Initialize the core command structure for executing   |
*                 a new command.  MUST be called before requesting the  |
*                 exectution of every command.                          |
*                                                                       |
* Arguments    :  -Pointer to master control struct                     |
*                 -Value of command to execute on the DSP (int)         |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"
#include "cbi_core_includes.h"


int cbi_init_command( int BPM_IDX, int cmd ) {
  
  CTL_System.p_Module[BPM_IDX]->core->cbi_cmd.cmd        = cmd;
  CTL_System.p_Module[BPM_IDX]->core->cbi_cmd.handshake  = CBI_CMD_NEW_HANDSHAKE;
  CTL_System.p_Module[BPM_IDX]->core->cbi_cmd.cmd_status = CBI_ILLEGAL_HANDSHAKE_0;
  
  return 0;

}

