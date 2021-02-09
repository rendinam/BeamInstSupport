/*!-----------------------------------------------------------------------+
* File         :  cbi_sig_handlers.c                                      |
*                                                                         |
* Description  :  Signal handling routines                                |
*                                                                         |
* Arguments    :  POSIX Signal number to handle (integer)                 |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


//--------------------------------------------------------
// Checks every online instrument for open sockets and
// closes them.
//--------------------------------------------------------
int cleanup_sockets(void) {

  int iidx;

  // Activate all instruments
  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    CTL_System.p_Module[iidx]->active = CBI_ACTIVE;
  }

  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE ) {
      if (CTL_System.p_Module[iidx]->comm.ethernet.socket_ID != CBI_SOCKET_CLOSED) {

	printf("Sending Idle Command to Instrument Index %i\n",iidx);
	cbi_init_command(iidx, CBI_IDLE_CMD);
	cbi_execute_command(iidx);
	
	printf("Cleaning up <%s>... ", CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	// Tell server to close its end of socket
	cbi_net_close_socket(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);
	
	// Close local end of socket
	cbi_net_net_close(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);
	CTL_System.p_Module[iidx]->comm.ethernet.socket_ID = CBI_SOCKET_CLOSED;
	
	printf("Done.\n");
      }
    } //endIF active
  } //endFOR
  
  return F_SUCCESS;
}


//--------------------------------------------------------
// For handling a CTRL-C event during normal operation
//--------------------------------------------------------
void std_sigint_handler( int signum ) {

  printf("\n\nCaught CTRL-C\n\n");

  cleanup_sockets();

  exit(0);

}
