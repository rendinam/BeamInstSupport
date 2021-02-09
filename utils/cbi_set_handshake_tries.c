//-----------------------------------------------------------------------+
// File         : cbi_set_handshake_tries.c                              |
//                                                                       |
// Description  : Sets the number of handshake check attempts that will  |
//                be made when executing the next command.               |
//                                                                       |
// Arguments    : Number of attempts to make when checking for command   |
//                handshake error or completion.                         |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"

#include "cbi_core_includes.h"


int cbi_set_handshake_tries( int tries ) {
  
  int retval = CBI_F_SUCCESS;

  CTL_System.handshake_timeout_tries = tries;

  return retval;
  
}
