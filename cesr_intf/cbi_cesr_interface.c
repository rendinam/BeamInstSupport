//-----------------------------------------------------------------------+
// File          : cbi_cesr_interface.c                                  |
//                                                                       |
// Description  :  Control system function to handle command requests    |
//                 via CESR CBPM mailbox interface (node = CBPM CONTROL) |
//                 Based on cbpm_cesr_ctl_interface.c (B. Rock)          |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Authors      :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


//-----------------------------------------------------------------------+
// Local defines -                                                       |
//-----------------------------------------------------------------------+
#define SLEEP_TIME 400


int cbi_cesr_interface(CBI_COMMAND cmd_list[], int num_commands, int num_comm_structs) {

   char *func_name = (char *)__FUNCTION__;

   int debug_level = CTL_System.debug_verbosity;

   const int list_size        = sizeof(cmd_list);
   const int list_size2       = sizeof(*cmd_list);
   const int bis_command_size = sizeof(CBI_COMMAND);
   
   const int array_siz = num_commands;
   
   int i;
   int good_cmd;
   int nmod;
   int stat, success_tally;
   
   
   // The interface structure may need to go into a shared header
   CTL_INTERFACE interface;
   CTL_INTERFACE *p_interface = &interface;
   
   // Log entry into control interface mode
   sprintf(message,"---CBPM CONTROL INTERFACE ACTIVE---");
   log_message(S_INFO, func_name, message);
   sprintf(message, " <CTRL><C> exits server mode");
   log_message(S_DINFO,func_name,message);
   fflush(stdout);   
   
   //  Load default interface parameters
   /// ctl_cbpm_intf_set_defaults(); // FIXME: re-enable

   
   // Register this server process in the control system list
   stat = cbi_register_cesr_intf(CBI_INTF_CONNECT);
   
   // Log error and exit if unable to register properly
   if (stat != F_SUCCESS) {
      sprintf(message, "Unable to attach to CTL CBPM INTERFACE!");
      log_message(S_ERROR,func_name,message);
      fflush(stdout);
      return CBI_F_FAILURE;
   }
   
   
   // Set the system interface state and status flag
   CTL_CBPM_Intf_Monitor.state  = CBI_CTL_INTF_ENABLED;
   CTL_CBPM_Intf_Monitor.status = CBI_INTF_STATUS_OK;
   
   
   // Function pointer definitions for use below.
   int (*fptr1)()   = NULL;
   int (*fptr2)()   = NULL;


   int iidx;
   int command_index = -1;

   int seq_step;
   int command;
   int status;
   int num_sequence_steps;

   //------------------------------------------------------
   // Command dispatch loop
   // Can be escaped by changing the state of
   // 
   //------------------------------------------------------
   while ( CTL_CBPM_Intf_Monitor.state != CBI_CTL_INTF_OFF ) {
      
      int numINST = CTL_System.n_Active;

      //-------------------------------------------
      // Wipe temporary communications structures
      //-------------------------------------------
      cbi_wipe_comm_structs( CTL_System.p_Module_temp, num_comm_structs );
      
      
      //---------------------------------------------------------------
      // If setup for a local command dispatch, poll MPM array for
      // command specification.  Otherwise, use the appropriate method 
      // to obtain the command to execute.
      //---------------------------------------------------------------
      if ( CTL_System.cmd_dispatch_method == CBI_LOCAL_CMD_DISPATCH ) {


	//--------------- Obtain and validate MPM command --------------------------------
	// Examine the MPM interface array
	if ( cbi_check_cesr_intf(p_interface) == F_FAILURE ) {
	  sprintf(message,"Error check CBPM CONTROL node");
	  log_message(S_ERROR,func_name,message);
	  fflush(stdout);
	  cbi_sleep(SLEEP_TIME);
	  continue;
	  
	  // If interface is DISABLED, this MUST wait for the ENABLE command
	} else if ( CTL_CBPM_Intf_Monitor.state == CBI_CTL_INTF_DISABLED ) {
	  
	  if ( interface.command == CBPM_COMMAND_ENABLE) {
            if ( (stat = cbi_enable_cesr_intf() ) == F_SUCCESS) {
	      cbi_reply_cesr_intf(CBPM_CMD_DONE);
	      sprintf(message,"CESR CTL INTERFACE entering ENABLED state...");
	      log_message(S_DINFO,func_name,message);
	      fflush(stdout);
            } else {
	      cbi_reply_cesr_intf(CBPM_CMD_ERROR);
	      sprintf(message,"CESR CTL INTERFACE ENABLE ERROR -- EXITING!");
	      log_message(S_ERROR,func_name,message);
	      fflush(stdout);
	      exit(stat);
            }
	  } else {
            cbi_sleep(SLEEP_TIME);
            continue;
	  }
	}
	
	// SLEEP and continue if NO COMMAND
	else if ( interface.command == CBPM_NOCMD_FLAG ) {
	  cbi_sleep(SLEEP_TIME);
	  continue;
	  
	  // Enter the DISABLED state on receipt of DISABLE command
	} else if ( interface.command == CBPM_COMMAND_DISABLE) {
	  stat = cbi_disable_cesr_intf();
	  if (stat == F_SUCCESS) {
            cbi_reply_cesr_intf(CBPM_CMD_DONE);
            sprintf(message,"CESR CTL INTERFACE entering DISABLED state...");
            log_message(S_DINFO,func_name,message);
	    fflush(stdout);
            continue;
	  } else {
            cbi_reply_cesr_intf(CBPM_CMD_ERROR);
            sprintf(message,"CESR CTL INTERFACE DISABLE ERROR -- EXITING!");
            log_message(S_ERROR,func_name,message);
	    fflush(stdout);
            exit(stat); // FIXME: Change to something more graceful.
	  }      
	  
	  // Break from infinite loop if receive EXIT command
	} else if ( interface.command == CBPM_EXIT_FLAG ) {
	  // Set the system interface status flag
	  CTL_CBPM_Intf_Monitor.state = CBI_CTL_INTF_OFF;
	  log_message(S_INFO,func_name,"Exiting!"); 
	  fflush(stdout);
	  break;   
	} 
	
	// Only parse a command if the start flag is set
	if ( interface.command_done[CTL_CBPM_Intf_Monitor.MailboxID] == 
	     CBPM_CMD_START) {
	  if ( cbi_check_cesr_intf(p_interface) == F_FAILURE ) {
            sprintf(message,"Error check CBPM CONTROL node");
            log_message(S_ERROR,func_name,message);
	    fflush(stdout);
            cbi_sleep(SLEEP_TIME);
            continue;
	  }
	} else {
	  continue;
	}
	
	// Dump command information
	cbi_dump_cesr_intf(CBI_CTL_INTF_CMD_DUMP, p_interface);
	fflush(stdout);


	good_cmd = FALSE;
	// Check for valid command and extract relevant fields
	// in preparation for command execution.
	for ( i = 0; i < array_siz; i++ ) {

	  if (debug_level > CBI_DEBUG_LEVEL_3) {
	    printf("DEBUG: Command list entry [%d]  --  Command value = %d\n", i, cmd_list[i].flag );
	  }
	  if (cmd_list[i].flag == interface.command) { 
	    command_index = i;
	    printf("Found a valid 3rd-party command [%d] idx=%d\n", cmd_list[i].flag, command_index);
	    good_cmd = TRUE;
	    break;
	  }
	  fflush(stdout);
	
	}



	if ( !good_cmd ) {
	  
	  sprintf(message,"Illegal Command %d",interface.command);
	  log_message(S_ERROR,func_name,message);
	  fflush(stdout);   
	  cbi_reply_cesr_intf(CBPM_CMD_ERROR);
	  cbi_sleep(SLEEP_TIME);
	  continue;
	  
	} else {
	  
	  // Give feedback of status
	  cbi_reply_cesr_intf(CBPM_CMD_WORKING);
	}



      } // endIF LOCAL_CMD_DISPATCH else, a 'SELF' command dispatch for recovery purposes.

      


      //------------------------------------------------------------
      // If command is a sequence, load the sequence and loop over
      // steps, executing separate preps and posts, if requested, 
      // across all instruments for each step in the sequence.
      //------------------------------------------------------------
      CTL_System.num_sequence_commands = 0;
      
      if (cmd_list[command_index].type == SEQUENCE_TYPE) {
	// Reset number of sequence commands in the queue
	fptr1 = cmd_list[command_index].prep_func;
	num_sequence_steps = (*fptr1)();
	printf("Sequence type encountered with %d steps.\n", num_sequence_steps);
      } else {
	if (debug_level > CBI_DEBUG_LEVEL_3) {
	  printf("Normal command type encountered with 1 step.\n");
	  printf("Command index = %d\n", command_index);
	  printf("Command name  = %s\n", cmd_list[command_index].name);
	}
	cbi_load_command( cmd_list, num_commands, cmd_list[command_index].name, CBI_TRUE ); // FIXME
	num_sequence_steps = 1;
      }
      fflush(stdout);



      //---------------------------------------------------------------
      // Populate Interface parameters holding structure with values
      // harvested from the actual MPM interface array.
      //---------------------------------------------------------------
      // Force the index 0 params to be set - no matter what command is called
      CTL_CBPM_Intf_Params.species     = cbi_bid2species( interface.bunch_id[0] );
      
      // Now loop and load the full list of bunch IDs
      for ( i=0; i < interface.num_bunches; i++ ) {
         CTL_CBPM_Intf_Params.bunch_id[i] = interface.bunch_id[i];
      }
      
      // Set the number of bunches and samples parameter
      CTL_CBPM_Intf_Params.num_bunches = interface.num_bunches;
      CTL_CBPM_Intf_Params.num_samples = interface.num_samples;
      CTL_CBPM_Intf_Params.num_turns   = interface.num_turns;
      CTL_CBPM_Intf_Params.trig_cnt    = interface.turn_select;
      
      // Load the injection control struct with number of shots requested
      CTL_CBPM_Intf_Params.num_shots = interface.num_shots;
      
      int num_args = 0;
      

      if (debug_level > CBI_DEBUG_LEVEL_4) {
	printf("1\n");
      }


      cbi_dispatch_sequencer( cmd_list, command_index, num_comm_structs, num_sequence_steps );



      //-----------------------------
      // Instrument success summary
      //-----------------------------
      if (stat == CBI_F_SUCCESS) {
	cbi_reply_cesr_intf(CBPM_CMD_DONE);
	sprintf(message,"Finished Command %d.",interface.command);
	log_message(S_DINFO, func_name, message);
      } else {
	cbi_reply_cesr_intf(CBPM_CMD_ERROR);
	sprintf(message,"Interface errors encountered for Command %d.",interface.command);
	log_message(S_DINFO, func_name, message);
      }
      

      //---------------------------------
      // Instrument error recovery stage
      //---------------------------------
      cbi_inst_status_handler( cmd_list, &command_index );
      if (debug_level > CBI_DEBUG_LEVEL_4) {
	printf("Upper COMMAND INDEX = %d\n", command_index);
      }

      sprintf(message, " <CTRL><C> exits server mode");
      log_message(S_INFO, func_name, message);
      fflush(stdout);


   } //endWHILE CTL_CBPM_Intf_Monitor.state != CBI_CTL_INTF_OFF
      
      


   // UN-Register this process from the control system list
   log_message(S_INFO,func_name,"Exiting server mode");
   stat = cbi_register_cesr_intf(CBI_INTF_DISCONNECT);

   // Reset command that was loaded to enter server (interface) mode
   // for proper continuation and cleanup.
   CTL_System.loaded_command.command   = CBI_INVALID_CMD_0;
   CTL_System.loaded_command.post_func = NULL;
   // Reinitialize all instrument status fields to successful status.
   for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
     CTL_System.p_Module[iidx]->status      = CBI_F_SUCCESS;
   }
   // Re-enable local command dispatching to pass flow control back
   // to the calling function.
   CTL_System.cmd_dispatch_method = CBI_LOCAL_CMD_DISPATCH;



   // turn on update flag
   // MAP2009_0402   CTL_Update.update_flag = DO_UPDATE;
   
   return CBI_F_SUCCESS;
}



//-----------------------------------------------------------------------+
// Routine to temporarily disable the CTL CBPM Interface                 |
//-----------------------------------------------------------------------+
int cbi_disable_cesr_intf(void) {
   
   char *func_name = "dsable_ctl_cbpm_intf";
   
   // Set the Interface Disabled Flag
   CTL_CBPM_Intf_Monitor.state = CBI_CTL_INTF_DISABLED;

   // Set the unknown status flag since an external entity has disabled 
   // the CBPM Server Process
   CTL_CBPM_Intf_Monitor.status = CBI_INTF_STATUS_UNKNOWN;
   
   return CBI_F_SUCCESS;
}



//-----------------------------------------------------------------------+
// Routine to re-enable the CTL CBPM Interface                           |
//-----------------------------------------------------------------------+
int cbi_enable_cesr_intf(void) {
   
   char *func_name = "ctl_enable_cbpm_interface";
   int status;
   
   // Load the STANDARD timing delays upon re-enable
   // MAP2009_0402   status = cbpm_set_delay_table(STD_DELAYS);
   
   // Now set the Interface Enabled Flag
   CTL_CBPM_Intf_Monitor.state = CBI_CTL_INTF_ENABLED;
   
   return CBI_F_SUCCESS;
}
