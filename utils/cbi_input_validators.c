//-----------------------------------------------------------------------+
// File         : cbi_input_validators.c                                 |
//                                                                       |
// Description  : Several functions to assist in validating user input:  |
//                 int_range_validator                                   |
//                    args: int val                                      |
//                          int low                                      |
//                          int high                                     |
//                Returns CBI_F_SUCCESS if value is between low and high |
//                        CBI_F_FAILURE otherwise.                       |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_int_range_validator( int val, int low, int high ) {
  
  if (val < low ||
      val > high) {
    return CBI_F_FAILURE;
  } else {
    return CBI_F_SUCCESS;
  }

}

