//-----------------------------------------------------------------------+
// File         :  cbi_close_mpmnet_connection.c                         |
//                                                                       |
// Description  :  Closes a connection to an MPMnet server.              |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_close_mpmnet_connection(void) {

   char *func_name = (char *)__FUNCTION__;
   int chkerr;

   chkerr = Mnet_disconnect(); // NEW METHOD
   if (chkerr != MPM_SUCCESS) {
     printf("\nMnet_closeServer returned: %i \n", chkerr);
   }

   return F_SUCCESS;

}
