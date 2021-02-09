//-----------------------------------------------------------------------+
// File         :  cbi_online_instruments.c                              |
//                                                                       |
// Description  :  Functions to allow printing a summary of all online   |
//                 instruments with various critical status information. |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_list_online_instruments( void ) {

  cbi_print_online_list( stdout );

  return F_SUCCESS;
  
}



int cbi_toggle_active_instruments( void ) {

  int status;

  status = cbi_print_online_list( stdout );
  fflush(stdout);
  while ( cbi_query_active() != -1 ) {
    status = cbi_print_online_list( stdout );
    fflush(stdout);
  }

  return F_SUCCESS;

}
