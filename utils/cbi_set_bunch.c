//-----------------------------------------------------------------------+
// File         :  cbi_set_bunch.c                                       |
//                                                                       |
// Description  :  Sets the specified bit in the specified instrument's  |
//                 local bunch bucket pattern high.                      |
//                                                                       |
// Arguments    :  Pointer to a master control struct                    |
//                 The index (starting at 1) of the bunch bucket         |
//                                                                       |
//                   (Bunch bucket 1 is mapped internally to C-array     |
//                   index 0.)                                           |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_set_bunch( int pattern_array[], int bunch_bit_idx ) {

  int reg, bit, i;
  int rcnt, bcnt;
  int compose = 1;

  int debug_level = CTL_System.debug_verbosity;

  reg = (bunch_bit_idx-1) / 16;
  bit = (bunch_bit_idx-1) % 16;

  if (debug_level > CBI_DEBUG_LEVEL_2) {
    printf("Setting - reg: %d,  bit %d\n", reg, bit);
  }

  compose = compose << bit;

  for (rcnt = 0; rcnt < 40; rcnt++) {
    if (rcnt == reg) {
	pattern_array[rcnt] = (pattern_array[rcnt] | compose);
    }
  }

  return CBI_F_SUCCESS;

}




int cbi_unset_bunch( int pattern_array[], int bunch_bit_idx ) {

  int reg, bit, i;
  int rcnt, bcnt;
  int compose = 1;

  int debug_level = CTL_System.debug_verbosity;

  reg = (bunch_bit_idx-1) / 16;
  bit = (bunch_bit_idx-1) % 16;
  
  if (debug_level > CBI_DEBUG_LEVEL_2) {
    printf("Unsetting - reg: %d,  bit %d\n", reg, bit);
  }
    
  compose = compose << bit;

  for (rcnt = 0; rcnt < 40; rcnt++) {
    if (rcnt == reg) {
	pattern_array[rcnt] = ( pattern_array[rcnt] & (~compose) );
    }
  }

  return CBI_F_SUCCESS;

}

