//-----------------------------------------------------------------------+
// File         :  cbi_f_ctl2dsp.c                                       |
//                                                                       |
// Description  :  Manipulates a VMS floating point number's bits to     |
//                 yield a SHARC/TigerSHARC DSP's float                  |
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


// NOTE: vms, linux and OSF may have different floating point representations.
//       This function may need to be altered to reflect that.

int cbi_f_ctl2dsp(int vms_float) {

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = "cbi_dsp_f_ctl2dsp";
   
   int dsp_float;
   int left16, right16;

//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+
   right16   = vms_float >> 16;
   right16   = right16 & 0x0000ffff;
   left16    = vms_float & 0x0000ffff;
   if (vms_float != 0) left16   -= 256;
   left16    = left16 << 16;
   left16    = left16 & 0xffff0000;
	 dsp_float = right16 + left16;

   return dsp_float;
} 
//-----------------------------------------------------------------------+
// End function cbi_dsp_f_ctl2dsp                                        |
//-----------------------------------------------------------------------+

