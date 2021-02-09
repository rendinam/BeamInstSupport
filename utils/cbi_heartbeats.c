//-----------------------------------------------------------------------+
// File         : heartbeats.c                                           |
//                                                                       |
// Description  : check_heartbeat - Checks the instrument's heartbeat    |
//                counter to determine if it is ready to accept and      |
//                process commands.                                      |
//                                                                       |
//                check_timing_integrity - Checks the instrument's       |
//                timing integrity indicator to see if a reliable turns  |
//                marker signal is present for driving data collection.  |
//                                                                       |
//                NOTE: The timing integrity check only provides         |
//                      up-to-date information if the instrument is      |
//                      responsive.  I.e. the check_heartbeat call must  |
//                      return TRUE before the value of                  |
//                      check_timing_integrity can be trusted.           |
//                                                                       |

// Arguments    : BPM_IDX                                                |
//                                                                       |
// Return value : TRUE  - heartbeat is present / timing integrity good   |
//                FALSE - heartbeat not present / timing integrity poor  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_check_heartbeat( int iidx ) {
  
  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int stat, heartbeat1, heartbeat2;
  int retval = FALSE;
  int failure = FALSE;

  // Read back the heartbeat structure twice in rapid succession.
  stat = cbi_gp_struct(READ, CTL_System.comm_method, CBI_HEARTBEAT_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
  if (stat != CBI_F_SUCCESS) {
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("ERROR in getting 1st heartbeat value!\n");
    }
    failure = TRUE;
  }
  heartbeat1 = CTL_System.p_Module[iidx]->core->cbi_heartbeat.heartbeat;

  stat = cbi_gp_struct(READ, CTL_System.comm_method, CBI_HEARTBEAT_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
  if (stat != CBI_F_SUCCESS) {
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("ERROR in getting 2nd heartbeat value!\n");
    }
    failure = TRUE;
  }
  heartbeat2 = CTL_System.p_Module[iidx]->core->cbi_heartbeat.heartbeat;

  // If the hearbeat counter values are different, the instrument
  // has updated them and thus is awake and ready to process commands.
  if (failure == FALSE) {
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("Heartbeat value 1: %d    Heartbeat value 2: %d\n", heartbeat1, heartbeat2);
    }
    if (heartbeat1 != heartbeat2) {
      retval = TRUE;
    } 
  } else {
    retval = -1;
  }

  return retval;
  
}



int cbi_check_timing_integrity( int iidx ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int stat, integrity;

  // Read back the timing integrity indicator
  stat = cbi_gp_struct(READ, CTL_System.comm_method, CBI_HEARTBEAT_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
  if (stat == CBI_F_SUCCESS) {
    integrity = CTL_System.p_Module[iidx]->core->cbi_heartbeat.timing_integrity;
  } else {
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("ERROR in getting timing integrity indicator!\n");
    }
    integrity = -1;
  }


  return integrity;

}
