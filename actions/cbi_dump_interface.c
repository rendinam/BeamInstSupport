//-----------------------------------------------------------------------+
// File         :  cbi_dump_interface.c                                  |
//                                                                       |
// Description  :  Prints out a formatted version of the MPM control     |
//                 system interface array.                               |
//                                                                       |
// Arguments    :                                                        |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_dump_interface( void ) {

  CTL_INTERFACE mpm_interface;

  cbi_check_cesr_intf( &mpm_interface );
  cbi_print_mpm_iface_contents( &mpm_interface );

  return CBI_F_SUCCESS;
  
}
