//-----------------------------------------------------------------------+
// File         :  cbi_float_convert.c                                   |
//                                                                       |
// Description  :  Converts between integer xbus data buffer and         |
//                 VMS/DSP floats for communications (READ/WRITE)        |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  M. Palmer                                             |
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



int *cbi_float_convert(int mode, int *p_buf, int dtype, int dcount, 
		       int offset, void *p_data){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = "cbi_float_convert";
   
   float *p_float = NULL;
   union {
         int   ival; 
         float fval;
   } temp;

   int i;
   int idummy;
   int MYLIN;
   int MYVMS;
//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+
   if (dtype == CBI_FLOAT) {
     p_float = ((float *) p_data + dcount * offset);
   } else {
     idummy = sprintf(message, 
		      "Invalid data type specified:  %d\n",dtype);
     log_message(S_ERROR,func_name,message);
     return NULL;
   }
   if (mode == WRITE) {
     if(MYVMS == 1){
       printf("Float conversion routine for writes not yet tested on VMS.");
       for (i = 0; i < dcount; i++) {   
         temp.fval = *p_float++;
         *p_buf++  = cbi_f_ctl2dsp(temp.ival);
       }
     }
     if(MYLIN == 1){
       for (i = 0; i < dcount; i++) {   
         temp.fval = *p_float++;
         *p_buf++  = temp.ival;
       }
     }

   } else if (mode == READ) {
     if(MYVMS == 1){
       printf("Float conversion routine for reads not yet tested on VMS.");
       for (i = 0; i < dcount; i++) {   
         temp.ival  = cbi_f_dsp2ctl(*p_buf++);
         *p_float++ = 4 * temp.fval;  // 4* shifts the mantissa for IEEE/VAX
//         printf("FLOAT_CON:  %f\n",temp.fval);
       }
     }
     if(MYLIN == 1){
       for (i = 0; i < dcount; i++) {   
         temp.ival  = *p_buf++;
         *p_float++ =  temp.fval;
       }
     }
   }

   return p_buf;
} 
//-----------------------------------------------------------------------+
// End function cbi_float_convert                                        |
//-----------------------------------------------------------------------+






