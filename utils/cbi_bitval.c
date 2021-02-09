//-----------------------------------------------------------------------+
// File         :  bitval.c                                              |
//                                                                       |
// Description  :  Returns the value of the bit at the given position in |
//                 the provided word.                                    |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M.Rendina                                             |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_bitval( int word, int bit ) {

  int bitmask = 1;
  int value = 0;

  bitmask = ( bitmask << bit );
  value = (word & bitmask);

  if (value > 0) {
    return 1;
  } else {
    return 0;
  }

}


// Returns 0 if input integer parity is EVEN
//         1 if input integer parity is ODD
int cbi_parity( int number ) {
  return (number % 2);
}

