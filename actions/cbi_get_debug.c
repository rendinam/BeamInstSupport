//-----------------------------------------------------------------------+
// File         :  cbi_get_debug.c                                       |
//                                                                       |
// Description  :  Retrieves and prints the STAT and DEBUG structures'   |
//                 contents.                                             |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_get_debug_post( int iidx ) {
  
  int stat;

  stat = cbi_gp_struct(READ, CTL_System.comm_method, CBI_STAT_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT );
  cbi_print_stat_struct( iidx, stdout );
  stat = cbi_gp_struct(READ, CTL_System.comm_method, CBI_DEBUG_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT );
  cbi_print_debug_struct( iidx, stdout );

  return CBI_F_SUCCESS;

}
