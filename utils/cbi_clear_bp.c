//-----------------------------------------------------------------------+
// File         :  clear_bp.c                                            |
//                                                                       |
// Description  :  Writes all zeros to the control-side copy of an       |
//                 instrument's bunch pattern register set, passed in as |
//                 an integer array.                                     |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_clear_bp( int pattern_array[] ) {

  int rcnt;

  for (rcnt = 0; rcnt < 40; rcnt++) { // FIXME: hardcoded for series-2
    pattern_array[rcnt] = 0;
  }

  return F_SUCCESS;

}
