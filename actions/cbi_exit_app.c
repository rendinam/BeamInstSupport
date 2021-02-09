//-----------------------------------------------------------------------+
// File         : cbi_exit_app.c                                         |
//                                                                       |
// Description  : Function(s) for cleanly exiting a control appliaction  |
//                                                                       |
// Arguments    : None                                                   |
//                                                                       |
// Author       : M. Rendina                                             |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_exit_app( void ) {

  char *func_name = (char*)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;


  int iidx, commstatus;

  //----------------------------------------------------------
  // Due to centralized socket management for communication 
  // with individual instruments, the dispatch sequencer
  // handles spawning sockets, but the exit command is a 
  // special case as it kills the application before the
  // dispatch sequencer can clean up the open sockets, hence
  // the cleanup pass done here. 
  //
  //  Close the CBInet socket to each instrument 
  //----------------------------------------------------------
  printf("Closing instrument sockets... ");

  // Activate all instruments
  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    CTL_System.p_Module[iidx]->active = CBI_ACTIVE;
  }

  
  cbi_close_sockets();


  printf("done.\n");


  cbi_close_mpmnet_connection();


  exit(0);
  
}


