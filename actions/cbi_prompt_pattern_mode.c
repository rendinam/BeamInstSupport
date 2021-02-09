/*!
*-----------------------------------------------------------------------+
* File         :  cbi_prompt_pattern_mode.c                             |
*                                                                       |
* Description  :  Wrapper for the bunch pattern composition function to |
*                 allow dispatching in a standard way by the            |
*                 cbi_dispatch_sequencer function.                      |
*                                                                       |
* Arguments    :                                                        |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_prompt_pattern_mode( void ) {

  int retval, input, timing_mode = m14ns;
  
  //-------------------------------------------------
  // Define the list of prompts and their parameters
  //-------------------------------------------------
  CBI_INIT_PROMPTS(1);


  int valid_inputs[3] = {2, 4,    14};
  prompts[0].supp_text_ptr    = NULL;
  prompts[0].prompt_text      = "Select bunch spacing [4/14]ns ";
  prompts[0].data_type        = CBI_INT;
  prompts[0].valid_inputs     = valid_inputs;
  prompts[0].default_value    = 14;
  prompts[0].destination      = &(input);
  prompts[0].last_val_default = TRUE;

  retval = CBI_PROMPTER();

  switch (input) {
  case 4:
    timing_mode = m4ns;
    break;
  case 14:
    timing_mode = m14ns;
    break;
  }
  

  cbi_clear_bp( CTL_System.meas_info.p_bunch_pattern );


  if ( CTL_System.RunningMode == CBI_SINGLE_MODE ||
       (CTL_System.RunningMode == CBI_BACKEND_MODE && CTL_System.instance_ID == 1) ) {

    cbi_compose_bunch_pattern( CTL_System.meas_info.p_bunch_pattern, timing_mode );

  }


  return CBI_F_SUCCESS;

}
