//-----------------------------------------------------------------------+
// File         :  cbi_mem_setup.c                                       |
//                                                                       |
// Description  :  Handles attachment of core communication data         |
//                 structures to the central management data structure   |
//                 as well as other initializations for each instrument. |
//                                                                       |
// Arguments    :                                                        |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_mem_setup( void ) {

  char *func_name = (char *)__FUNCTION__;

  int iidx;
  int status;

  // Set the temporary master struct pointer to reference the temporary struct tree.
  //CTL_System.p_Module_temp = &(tsharc_ctl_mod_temp);
  CTL_System.p_Module_temp = &(CTL_Module_temp);



  // Initialize the temporary core structure tree for 'command imaging'
  // and attach to temp master struct.
  CTL_System.p_Module_temp->core = &(core_comm_structs_temp);
  if ((status = cbi_init_a(CTL_System.p_Module_temp)) != CBI_F_SUCCESS) {
    sprintf(message, "Core temporary structure initialization failed -- Exiting!");
    log_message(S_ERROR,func_name,message);
    return CBI_F_FAILURE;
  }


  for (iidx = 0; iidx < CBI_MAX_INSTRUMENTS; iidx++) {
    // Set pointer to the structure corresponding to the BPM to initialize.
    //CTL_System.p_Module[iidx] = &(tsharc_ctl_mod[iidx]);
    CTL_System.p_Module[iidx] = &(CTL_Module[iidx]);
    
    // Initialize pointer key for core structures  
    CTL_System.p_Module[iidx]->core = &(core_comm_structs[iidx]);
    if ((status = cbi_init_a(CTL_System.p_Module[iidx])) != CBI_F_SUCCESS) {
      sprintf(message, "Core structure  initializations failed -- Exiting!");
      log_message(S_ERROR,func_name,message);
      return CBI_F_FAILURE;
    }
    // Initialize the activity state of each allocated instrument slot.
    CTL_System.p_Module[iidx]->active = CBI_EMPTY_SLOT;
  }
  //printf("Got through CORE structures initialization.\n");



  // Initialize communications identification fields.
  strcpy( (char *)CTL_System.p_Module_temp->comm.ethernet.hostname, "" );
  strcpy( (char *)CTL_System.p_Module_temp->comm.ethernet.IP_address, "" );


  return CBI_F_SUCCESS;


}
