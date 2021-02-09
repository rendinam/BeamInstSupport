/*!
*-----------------------------------------------------------------------+
* File         :  dispatch_sequencer.c                                  |
*                                                                       |
* Description  :  Handles the control system dispatching and sequencing |
*                 of all instrumentation system commands.               |
*                                                                       |
* Arguments    :  -Pointer to fully-populated array of command          |
*                  definition structures for the given platform.        |
*                                                                       |
*                 -Array index of the structure defining the command to |
*                  dispatch.                                            |
*                                                                       |
*                 -The number of communications data structures         |
*                  supported by the given platform.                     |
*                                                                       |
*                 -The number of steps in the command sequence that is  |
*                  to be dispatched.                                    |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


// max number of bytes to get at once via socket communication
#define MAXDATASIZE 300


int cbi_dispatch_sequencer( CBI_COMMAND cmd_list[], int command_index, int num_comm_structs, int num_sequence_steps ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  if (debug_level > CBI_DEBUG_LEVEL_4) {
    printf("[debug %d] DISPATCH SEQUENCER CALLED WITH: \n", debug_level);
    printf(" num_comm_structs   = %d\n", num_comm_structs );
    printf(" num_sequence_steps = %d\n", num_sequence_steps );
  }

  int status, commstatus;
  // Function pointer definitions for use below.
  int (*fptr1)()   = NULL;
  int (*fptr2)()   = NULL;
  int seq_step;
  int command;
  int num_args;
  int iidx;
  int retval;


  int parallel;
  char xfer[MAXDATASIZE] = {""};


  //----------------------------------------------------------
  // Loop over all sequence steps.
  // num_sequence_commands = 1   - For normal commands
  // num_sequence_commands = N   - For SEQUENCE_TYPE commands
  //----------------------------------------------------------
  for (seq_step = 0; seq_step < num_sequence_steps; seq_step++) {
    
    // Clear all instrument status (function return) codes to success value.
    for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
      CTL_System.p_Module[iidx]->status = CBI_F_SUCCESS;
    }
    
    
    // FIXME: generalize by platform
    cbi_wipe_comm_structs( CTL_System.p_Module_temp, num_comm_structs );
    fptr1 = fptr2 = NULL;
    if (debug_level > CBI_DEBUG_LEVEL_4) {
      printf("[debug %d] Wiped communication structures\n", debug_level);
    }

    // Default is to not skip connection process to active instruments
    // as part of each command.
    CTL_System.skip_comm_connect = FALSE;

    //--------------------------------------------------------
    // If this is a step in a sequence, not a stand-alone 
    // command, run the next step of the sequencing function.
    //--------------------------------------------------------
    if (num_sequence_steps > 1) {
      fptr1 = cmd_list[command_index].prep_func;
      status = (*fptr1)();
      // If the sequence step returned a break request, 
      // break out of sequence processing loop entirely.
      if (status == CBI_COMMAND_SEQUENCE_BREAK_REQ) {
	printf("Breaking out of command sequence...\n");
	break;
      }
    }
    if (debug_level > CBI_DEBUG_LEVEL_4) {
      printf("3\n");
    }
    
    if (CTL_System.skip_comm_connect == FALSE) {
      cbi_open_sockets_active();
    } else {
      printf("\nSkipping communication connect!\n\n");
    }


    //----------------------------------------------
    // Reset data file index indicator.
    //----------------------------------------------
    CTL_System.output_file_index = CBI_FILE_ID_NOT_SET;


    //-------------------------------------------------
    // If a prep function is available, execute it.
    //-------------------------------------------------

    // Reset the number of handshake check tries to default.
    CTL_System.handshake_timeout_tries = 0;

    status = CBI_F_SUCCESS;
    if ( CTL_System.loaded_command.prep_func != NULL ) {
      if (debug_level > CBI_DEBUG_LEVEL_1) {
	printf("Command name == [%s]\n", CTL_System.loaded_command.name );
      }
      fptr1 = CTL_System.loaded_command.prep_func;
      status = (*fptr1)();
      printf("Prep() function status = %d\n", status);
    }

      
    //------------------------------------------------------
    // Save any '_post()' function pointer that is provided
    //------------------------------------------------------
    if ( CTL_System.loaded_command.post_func != NULL ) {
	fptr2 = CTL_System.loaded_command.post_func;
    }  
    
    
    //----------------------------------------------
    // Grab a copy of the command specifier
    // 0 = CBI_INVALID_CMD_0 (NULL) means no request
    // gets dispatched to the instruments.
    //----------------------------------------------
    command = CTL_System.loaded_command.command;
    

    //----------------------------------------------
    // Note any preparation errors
    //----------------------------------------------
    if (num_sequence_steps == 1) {
      if (status == CBI_F_FAILURE) {
	printf("\'Prep\' returned failure.  Back to menu.\n");
	if (CTL_System.skip_comm_connect == FALSE) {
	  cbi_close_sockets();
	}
	continue;
      }
    }
    

    //----------------------------------------------
    // Back to command polling loop if this was a
    // a SYSTEM (non-instrument) command. 
    //----------------------------------------------
    if (command == CBI_INVALID_CMD_0 && fptr2 == NULL) {
      if (CTL_System.skip_comm_connect == FALSE) {
	cbi_close_sockets();
      }
      continue;
    }
    
    
    //----------------------------------------------
    // Copy touched structs to each active 
    // instrument's structure tree and transfer to
    // the instrument as needed.
    //----------------------------------------------
    num_args = 2;
    fptr1 = &(cbi_copy_touched_structs);
    printf("--Copying and transferring structure set...\n");
    cbi_inst_loop( 0, CTL_System.n_Modules, fptr1, FALSE, num_args, num_comm_structs );
        

    //----------------------------------------------
    // Initialize command and execute
    //----------------------------------------------
    if (command != CBI_INVALID_CMD_0) {
      
      num_args = 2;
      fptr1 = &(cbi_init_command);
      printf("--Initializing command...\n");
      cbi_inst_loop( 0, CTL_System.n_Modules, fptr1, FALSE, num_args, command );
      
      // Execute the command on each device
      num_args = 0;
      fptr1 = &(cbi_execute_command);
      printf("--Executing command...\n");
      cbi_inst_loop( 0, CTL_System.n_Modules, fptr1, FALSE, num_args );
      if (debug_level > CBI_DEBUG_LEVEL_4) {
	printf("Debug: \'Execute command\' loop\n");
      }
      
      
      //----------------------------------------------
      // TODO: Appropriate delay / 
      //                   multipart handshake here
      //----------------------------------------------
      // Must allow for instruments to get to
      // final acquisition trigger enable point
      // after performing an optional initial 
      // timing adjustment / other setup / etc.
      
      
      //--------------------------------------------------------------
      // If running in trigger-enable mode, set the trigger(data)
      // enable word here.  Reset below once acquisition is complete.
      //--------------------------------------------------------------
      if (CTL_System.self_trigger_enable == CBI_TRUE) {
      	// NOTE: Wait here should account for specific measurement
        //       setup time.  The alternative is an intelligent
	//       multi-part handshake to guarantee fast enabling.
        cbi_sleep(500);  // For Triplet time shift test.
      	printf("Asserting trigger enable (CBPM COMMAND ele 1) = 2...\n");
      	cbi_gate_control(CBI_GATE_ENABLE);
      }

      
      //----------------------------------------------
      // Wait for command completion handshake
      //----------------------------------------------
      num_args = 0;
      fptr1 = &(cbi_wait_for_command);
      printf("--Waiting for handshake completion...\n");
      // Reset global handshake timeout indicator
      CTL_System.handshake_timeout_flag = FALSE;
      cbi_inst_loop( 0, CTL_System.n_Modules, fptr1, FALSE, num_args );
      

      //--------------------------------------
      // If in trigger-enable mode, de-assert 
      // the CESR trigger(data)-enable word.
      //--------------------------------------
      if (CTL_System.self_trigger_enable == CBI_TRUE) {
      	printf("De-asserting trigger enable (CBPM COMMAND ele 1) = 0...\n");
      	cbi_gate_control(CBI_GATE_DISABLE);
      }

      
    } //endIF command != CBI_INVALID_CMD_0
    

    //--------------------------------------------------
    // Run '_post()' function for data retrieval etc...
    //--------------------------------------------------
    if (fptr2 != NULL) {
      num_args = 1;
      printf("--Post function loop...\n");
      // Use serial or parallel execution of this function based on flag
      // present in the definition for the command in question.
      parallel = cmd_list[command_index].parallel_post;
      cbi_inst_loop( 0, CTL_System.n_Modules, fptr2, parallel, num_args );
    }
    
    
    //----------------------------------------------
    // Examine final status of each instrument
    // and take appropriate action.
    //----------------------------------------------
    

    //------------------------------
    // Command status summary call
    //------------------------------
    cbi_command_status_summary( stdout, CTL_System.loaded_command.name );
    CTL_System.cmd_dispatch_method = CBI_LOCAL_CMD_DISPATCH;
    if (debug_level > CBI_DEBUG_LEVEL_4) {
      printf("7\n");
    }
    

    //--------------------------------------------------------------
    // Run a follow-up function (with no automatic instrument loop) 
    // if provided.
    // Allows for data concatenation, processing, analysis, 
    // archiving, etc.
    //--------------------------------------------------------------
    if ( CTL_System.loaded_command.final_func != NULL ) {
      printf("Executing 'final' function...\n");
      fptr2 = CTL_System.loaded_command.final_func;
      status = (*fptr2)();
      if (status != CBI_F_SUCCESS) {
	sprintf(message, " _final() function failed.");
	log_message(S_ERROR, func_name, message);
      }
    }
    

    if (CTL_System.skip_comm_connect == FALSE) {
      cbi_close_sockets();
    }


  } // endFOR seq_step
  


  //-------------------------------------------
  // Command or sequence dispatch finished.
  // Reset instrument error recovery flag for 
  // all active instruments.
  //-------------------------------------------
  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    if (CTL_System.p_Module[iidx]->active == CBI_TRUE) {
      CTL_System.p_Module[iidx]->in_recovery = CBI_FALSE;
    }
  }
  

  return retval;

}








