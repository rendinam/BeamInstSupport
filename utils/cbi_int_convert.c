//-----------------------------------------------------------------------+
// File         :  cbi_int_convert.c                                     |
//                                                                       |
// Description  :  Converts between integer xbus data buffer and         |
//                 integer or unsigned integer types for communications  |
//                 (READ/WRITE)                                          |
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

#include "cbi_core_includes.h"


int *cbi_int_convert(int mode, int *p_buf, int dtype, int dcount, 
		     int offset, void *p_data){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = "cbi_dsp_int_convert";
   
   int               *p_int   = NULL;
   unsigned int      *p_uint  = NULL;

   union {
         int   ival; 
         float uval;
   } temp;

   
   int i;
   int idummy;
   
//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+
   
// Set up data pointer 
   if (dtype == CBI_INT) {
      p_int   = ((int *) p_data + dcount * offset);
   } else if (dtype == CBI_UINT) {
      p_uint  = ((unsigned int *) p_data + dcount * offset);
   } else {
      idummy = sprintf(message, 
                       "Invalid data type specified:  %d\n",dtype);
      log_message(S_ERROR,func_name,message);
      return NULL;
   }
   
   if (mode == WRITE) {
      for (i = 0; i < dcount; i++) {
         if (dtype == CBI_INT) {
            *(p_buf++) = *(p_int++);
         } else if (dtype == CBI_UINT) {
            temp.uval  = (unsigned int) *(p_uint++); // explicit cast for integer types
            *(p_buf++) = temp.ival;
         } 
      }
      
   } else if (mode == READ) {
      
      for (i = 0; i < dcount; i++) {   
         if (dtype == CBI_INT) {
            *(p_int++) = *(p_buf++);
         } else if (dtype == CBI_UINT) {
            temp.ival   = (int) *(p_buf++);   
            *(p_uint++) = temp.uval;
         } 
      }  
   }
   return p_buf;
} 
//-----------------------------------------------------------------------+
// End function cbi_dsp_int_convert                                         |
//-----------------------------------------------------------------------+






