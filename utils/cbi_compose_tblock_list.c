//-----------------------------------------------------------------------+
// File         :  compose_tblock_list.c                                 |
//                                                                       |
// Description  :  Creates a list of the timing block values for each    |
//                 successive bunch pattern bit enabled in the rotated   |
//                 bunch pattern received from the instrument.           |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int compose_tblock_list( int rot_bunch_pat[], int tblock_list[] ) {

  int i, bitcount = 0;

  for (i = 0; i < CBI_MAX_BUNCHES; i++) {
    tblock_list[i] = -1;
  }
  
  for (i = 0; i < CBI_MAX_BUNCHES; i++) {
    if (rot_bunch_pat[i] == 1) {
      if (i % 2 == 0) {
	tblock_list[bitcount] = 0;
      } else {
	tblock_list[bitcount] = 1;
      }
      bitcount++;
    }
  }

  return F_SUCCESS;

}
