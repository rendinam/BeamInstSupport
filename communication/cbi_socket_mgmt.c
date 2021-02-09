/*!
*-----------------------------------------------------------------------+
* File         :  cbi_socket_mgmt.c                                     |
*                                                                       |
* Description  :  Socket management functions.                          |
*                 Two functions:                                        |
*                 - cbi_open_sockets_active() : Opens a socket to all   |
*                   active instruments, storing the socket identifier   |
*                   in the central management struct for later use.     |
*                                                                       |
*                 - cbi_close_sockets() : For every instrument with an  |
*                   open socket identifier stored in the central mgmt.  |
*                   structure, close the remote end, and then the local |
*                   portion of the socket connection.                   |
*                                                                       |
* Arguments    :                                                        |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_open_sockets_active( void ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int iidx;

  // Only perform socket opens if operating in a maintenance
  // mode lower than level 3 (no-comms mode).
  if (CTL_System.maintenance_mode < 3) {
    //----------------------------------------------------------
    // Open a socket to each instrument that is to take part in
    // the command.
    //----------------------------------------------------------
    char *error_buf;
    int socketfd = 0;
    for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
      if (CTL_System.p_Module[iidx]->active == CBI_ACTIVE) {
	cbi_net_clr_error(); 
	if (debug_level > CBI_DEBUG_LEVEL_3) {
	  sprintf(message, "Opening socket to [%s]...\n", CTL_System.p_Module[iidx]->comm.ethernet.hostname);
	  log_message( S_INFO, message, func_name );
	}
	socketfd = cbi_net_fdopen( CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	if (socketfd > 0) {
	  // Save the socket number in the central structure tree for state management.
	  CTL_System.p_Module[iidx]->comm.ethernet.socket_ID = socketfd;
	} else {
	  CTL_System.p_Module[iidx]->comm.ethernet.socket_ID = CBI_SOCKET_CLOSED;
	  error_buf = cbi_net_get_error();
	  sprintf(message, "Bad socket fd. Unable to connect to remote host - %s \n   cbi_net_error_buf: %s", 
		  CTL_System.p_Module[iidx]->comm.ethernet.hostname,
		  error_buf );
	  cbi_net_clr_error();
	  log_message( S_ERROR, func_name, message );
	}
      }
    } //endFOR 
  }

  return CBI_F_SUCCESS;

}


int cbi_close_sockets( void ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int iidx, commstatus;

  if (CTL_System.maintenance_mode < 3) {
    //----------------------------------------------------------
    // Instrument initializations complete, close all
    // communication sockets that were opened for that process
    // and that are still open.
    //----------------------------------------------------------
    for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
      if (CTL_System.p_Module[iidx]->comm.ethernet.socket_ID != CBI_SOCKET_CLOSED) {
	
	// Close remote end of socket
	commstatus = cbi_net_close_socket(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);
	if (commstatus == 0) {
	  printf("ERROR closing remote end of connection to  [%s]!\n", 
		 CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	}
	// Close local end of socket
	cbi_net_net_close(CTL_System.p_Module[iidx]->comm.ethernet.socket_ID);
	if (debug_level > CBI_DEBUG_LEVEL_3) {
	  sprintf(message, "Closing socket %d to %s", 
		  CTL_System.p_Module[iidx]->comm.ethernet.socket_ID, 
		  CTL_System.p_Module[iidx]->comm.ethernet.hostname   );
	  log_message( S_INFO, message, func_name );
	}

	CTL_System.p_Module[iidx]->comm.ethernet.socket_ID = CBI_SOCKET_CLOSED;

      }
      
    } //endFOR iidx
    
  }

  return CBI_F_SUCCESS;

}
