/*!-----------------------------------------------------------------------+
* File         :  cbi_query_active.c                                      |
*                                                                         |
* Description  :  Prompt to toggle the activity state of any currently    |
*                 online instrument.                                      |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define MAXDATASIZE    300


int cbi_query_active(void) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int inst, bpm, retval = 0;
  char idx1, idx2, input;


  char buf[MAXDATASIZE];

  char SELECTION_NOTIFY[CBI_MAX_STRING_LENGTH] = {"SELECTION\0"};

  if (CTL_System.RunningMode == CBI_BACKEND_MODE) {

    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("Sending instrument selection notification to manager...%s\n", SELECTION_NOTIFY);
      fflush(stdout);
    }
    if(send(CTL_System.mgmt_socket_IDs[0], SELECTION_NOTIFY, 10, 0) == -1) {
      perror("Server notify send error");
      fflush(stdout);
      return CBI_F_FAILURE;
    }

    // Wait for selection information to come from manager
    strcpy( buf, "" );
    recv(CTL_System.mgmt_socket_IDs[0], buf, MAXDATASIZE, 0);
    if (debug_level > CBI_DEBUG_LEVEL_2) {
      printf("Received selection info string: \"%s\"\n", buf);
    }
    input = (int)buf[0];


  } else {

    //-------------------------------------------------
    // Define the list of prompts and their parameters
    //-------------------------------------------------
    CBI_INIT_PROMPTS(1);
    
    
    int valid_inputs[7] = {6, (int)'a', (int)'n', (int)'r', (int)'i', (int)'s', (int)'q'};
    int map_out[6]      = { 1, 2, 3, 4, 5, 6 };
    prompts[0].supp_text_ptr    = NULL;
    prompts[0].prompt_text      = "\nInstrument activation:  (a)ll, (n)one, (r)ange toggle, (i)nvert all, (s)ingle toggle, (q)uit : ";
    prompts[0].data_type        = CBI_CHAR;
    prompts[0].valid_inputs     = valid_inputs;
    prompts[0].map_out          = NULL;
    prompts[0].default_value    = (int)'q';
    prompts[0].destination      = &(input);
    prompts[0].last_val_default = FALSE;
    
    
    retval = CBI_PROMPTER();
    fflush(stdout);

  }

  printf("\n");
  fflush(stdout);

  switch (input) {

  case (int)'a':    case (int)'A':
    printf("Activating ALL online instruments\n");
    for (inst = 0; inst < num_online(); inst++) {
      CTL_System.p_Module[inst]->active = CBI_ACTIVE;
    }
    break;

  case (int)'n':    case (int)'N':
    printf("Deactivating ALL online instruments\n");
    for (inst = 0; inst < num_online(); inst++) {
      CTL_System.p_Module[inst]->active = CBI_INACTIVE;
    }
    break;

  
  case (int)'r':    case (int)'R':
    if (CTL_System.RunningMode != CBI_BACKEND_MODE) {
      printf("Specify beginning index : ");
    }
    idx1 = cbi_get_int();
    if (idx1 < 0 || idx1 > num_online() ) {
      printf("\nValid input is in the range 0-%d\n\n", num_online() );
      cbi_sleep(1.25);
      break;
    }
    if (CTL_System.RunningMode != CBI_BACKEND_MODE) {
      printf("Specify ending index    : ");
    }
    idx2 = cbi_get_int();
    if (idx2 < idx1) {
      printf("\nSecond index needs to be greater than the first.\n\n");
      cbi_sleep(1.25);
      break;
    }
    if (idx2 < 0 || idx2 > num_online() ) {
      printf("\nValid input is in the range 0-%d\n\n", num_online() );
      cbi_sleep(1.25);
      break;
    }
    for (inst = (idx1-1); inst <= (idx2-1); inst++) {
      if (CTL_System.p_Module[inst]->active == CBI_INACTIVE) {
	CTL_System.p_Module[inst]->active = CBI_ACTIVE;
	continue;
      }
      if (CTL_System.p_Module[inst]->active == CBI_ACTIVE) {
	CTL_System.p_Module[inst]->active = CBI_INACTIVE;
	continue;
      }
    }
    break;

  case (int)'i':    case (int)'I':
    printf("Toggling activity state of all online instruments\n");
    for (inst = 0; inst < num_online(); inst++) {
      if (CTL_System.p_Module[inst]->active == CBI_INACTIVE) {
	CTL_System.p_Module[inst]->active = CBI_ACTIVE;
	continue;
      }
      if (CTL_System.p_Module[inst]->active == CBI_ACTIVE) {
	CTL_System.p_Module[inst]->active = CBI_INACTIVE;
	continue;
      }
    }
    break;

  case (int)'s':    case (int)'S':
    if (CTL_System.RunningMode != CBI_BACKEND_MODE) {
      printf("Enter index of instrument to toggle: ");
    }
    idx1 = cbi_get_int();
    if (debug_level > CBI_DEBUG_LEVEL_1) {
      printf("index = %d\n", idx1);
    }
    cbi_toggle_activity( idx1-1 );
    break;


  case (int)'q':    case (int)'Q':
    retval = -1;
    break;

  default:
    break;
  }


  return retval;
  
}
