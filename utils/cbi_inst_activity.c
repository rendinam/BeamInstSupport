/*!-----------------------------------------------------------------------+
* File         :  cbi_activity.c                                          |
*                                                                         |
* Description  :  Three simple, related functions.                        |
*                 1) active - returns the activity state of a given device|
*                 2) num_active - Returns number of currently active      |
*                    devices.                                             |
*                 3) toggle_activity - Switch an instrument from          |
*                    CBI_ACTIVE to CI_INACTIVE and vice versa, depending  |
*                    on its current activity state.                       |
*                                                                         |
* Arguments    :  Index of device to query                                |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_active( int iidx ) {
  
  return CTL_System.p_Module[iidx]->active;

}


//----------------------------------------------------

int cbi_num_active() {

  int inst, count = 0;

  for (inst = 0; inst < CBI_MAX_INSTRUMENTS; inst++) {
    if (CTL_System.p_Module[inst]->active == CBI_ACTIVE) {
      count++;
    }
  }

  return count;

}


//----------------------------------------------------

int cbi_toggle_activity( int iidx ) {

  int state;

  if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE ) {
    CTL_System.p_Module[iidx]->active = CBI_INACTIVE;
    state = (int)'i';
  } else {
    CTL_System.p_Module[iidx]->active = CBI_ACTIVE;
    state = (int)'a';
  }
  return state;
}

