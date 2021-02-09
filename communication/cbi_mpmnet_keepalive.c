/*!
*-----------------------------------------------------------------------+
* File         :  cbi_mpmnet_keepalive.c                                |
*                                                                       |
* Description  :  Keeps an MPMnet connection open as long as desired.   |
*                 Once called, it will perform a simple one-word read   |
*                 of an element in the CBPM CONTROL arrays in the MPM   |
*                 every ten seconds in order to generate enough traffic |
*                 to prevent the MPMnet server from killing off the     |
*                 connection due to inactivity.                         |
*                 Ideally used in a thread of its own for silent        |
*                 background operation.                                 |
*                                                                       |
* Arguments    :  None                                                  |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

int cbi_MPMnet_keepalive( int *keepalive ) {

  char *func_name = (char *)__FUNCTION__;

  //int total_wait_seconds = 10;
  //int ms_wait_per_loop   = 500;
  //int loop;
  //int number_of_loops = total_wait_seconds / (ms_wait_per_loop*1000);
  //printf("number of loops = %d  \n", number_of_loops);
  int status, ele[2], incoming;

  ele[0] = ele[1] = 1;

  while ( *(keepalive) ) {

    //printf("Performing simple MPMnet request...\n");
    status = vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], &incoming);
    if (status != MPM_SUCCESS) {
      sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
      log_message(S_ERROR, func_name, message);
    }
    sleep(1);
    
  } //endWHILE

  return CBI_F_SUCCESS;
}
