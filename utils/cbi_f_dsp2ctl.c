//-----------------------------------------------------------------------+
// File         :  cbi_f_dsp2ctl.c                                       |
//                                                                       |
// Description  :  Manipulates a SHARC/TigerSHARC DSP's float bits to    |
//                 yield a VMS (VAX/Alpha?) float                        |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  J.Moffitt/M. Palmer                                   |
//-----------------------------------------------------------------------+


//-----------------------------------------------------------------------+
// Include Files -                                                       |
//                                                                       |
//   NOTE - By convention, include files should not specify a PATH for   |
//          ready portability to non-UNIX platforms.                     |
//                                                                       |
// System includes -                                                     |
//                                                                       |
// stdio.h             -  Provide IO capability.                         |
// string.h            -  Provide string manipulation capability.        |
// math.h              -  Provide mathematical function capability.      |
//                                                                       |
// CESR includes   -                                                     |
//                                                                       |
// CESR_platform.h     -  CESR processor/system configuration file.      |
// utilities.h         -  Some useful and generally used definitions     |
//                        and enumerations.                              |
// messages.h          -  Standard error message logging header file     |
// Mnet_client_c.h     -  MPMnet header file for C clients               |
//                                                                       |
// Local includes  -                                                     |
//-----------------------------------------------------------------------+
#include <stdio.h>
#include <string.h>
#include <math.h>

// CESR includes
#include "CESR_platform.h"
#include "utilities.h"
#include "messages.h"
#include "Mnet_client_c.h"

// Local includes
#include "cbi_core_includes.h"


int cbi_f_dsp2ctl(int dsp_float){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = "cbi_dsp_f_dsp2ctl";
   
   int vms_float;
   int left16, right16;

//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+

// protect against -0
   if (dsp_float == 0x80000000) dsp_float = 0;

// protect against the SHARC DSP equivalent of NaN
   if (dsp_float == 0xffffffff) {
      sprintf(message,"Invalid float encountered - setting to 0.0");
      log_message(S_ERROR, func_name, message);
      dsp_float = 0;
   }      

   right16   = dsp_float >> 16;
   right16   = right16 & 0x0000ffff;
   left16    = dsp_float << 16;
   left16    = left16 & 0xffff0000;
   vms_float = left16 + right16;

// protect against floating overflows
   if (dsp_float == 0xff800000) {
      sprintf(message,"DSP floating overflow encountered - setting to 4.0E10");
      log_message(S_ERROR, func_name, message);
      vms_float = 0x501502f9;
   }

   return vms_float;
} 
//-----------------------------------------------------------------------+
// End function cbi_dsp_f_dsp2ctl                                           |
//-----------------------------------------------------------------------+

