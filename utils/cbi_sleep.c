//-----------------------------------------------------------------------+ 
// File         :  cbi_sleep.c                                           |
//                                                                       |
// Description  :  make calls to appropriate system sleep fn's           |
//                                                                       |
// Arguments    :  int milli- number of miliseconds to sleep     	 |
//                                                                       |
// Return Value :  F_SUCCESS                                   		 |
//                                                                       |
// Author       :  B. Rock                                               |
//-----------------------------------------------------------------------+


//-----------------------------------------------------------------------+
// Include Files -                                                       |
//                                                                       |
//   NOTE - By convention, include files should not specify a PATH for   |
//          ready portability to non-UNIX platforms.                     |
//                                                                       |
// System includes:                                                      |
// stdio.h             -  Provide IO capability.                         |
//                                                                       |
// CESR includes:                                                        |
// CESR_platform.h     -  CESR processor/system configuration file.      |
//                                                                       |
// Includes from the c_utils library:                                    |
// utilities.h         -  Some useful and generally used definitions     |
//                        and enumerations.                              |
// messages.h          -  Standard error message logging header file     |
//                                                                       |
// Local includes:                                                       |
// cbpm_params.h       -  CBPM parameters                                |
// cbpm_types.h        -  CBPM type definitions                          |
// cbpm_ctl_types.h    -  CBPM control system type definitions           |
// cbpm_ctl_system.h   -  CBPM main control system data structures       |
//-----------------------------------------------------------------------+
#include <unistd.h>
#include <stdio.h>

#include "CESR_platform.h"
#include "utilities.h"
#include "messages.h"

#include "cbi_net_common.h"
#include "cbi_net_client.h"


int cbi_sleep( int millisec ){
   
//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+
   char *func_name = "cbi_sleep";

   int sec   = 0;
   int micro = 0;

   // convert milliseconds to seconds and microseconds
   sec   = millisec/1000;
   micro = 1000*(millisec % 1000); 
   sleep(sec);
   usleep(micro);
       
   return F_SUCCESS;

}
