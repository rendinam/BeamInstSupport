//-----------------------------------------------------------------------+
// File         : cbi_socket_cleanup.c                                   |
//                                                                       |
// Description  : Clean up communications connection in a graceful way.  |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_socket_cleanup( int iidx ) {

  // Tell server to close its end of socket
  cbi_net_close_socket(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);
     
  // Close local end of socket
  cbi_net_net_close(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);

  return F_SUCCESS;

}
