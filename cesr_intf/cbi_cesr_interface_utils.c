//-----------------------------------------------------------------------+
// File         :  cbi_cesr_interface_utils.c                            |
//                                                                       |
// Description  :  Utility functions for interacting with the control    |
//                 system CBPM mailbox interface                         |
//                                                                       |
// Author       :  M. Palmer (4/2/2009)                                  |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


//-----------------------------------------------------------------------+
// Definitions -                                                         |
//                                                                       |
//                                                                       |
//-----------------------------------------------------------------------+
// Possible return values from VMGLOCK
#define CBI_LOCK_SUCCESS  1
#define CBI_LOCK_FAILURE -1
#define CBI_LOCK_EXISTS  -2



//-----------------------------------------------------------------------+
// External variables -                                                  |
//                                                                       |
//                                                                       |
//-----------------------------------------------------------------------+
// Initialize variables specifying the "CBPM CONTROL" node
char *node = CBPM_CONTROL_NODE;
int   ele[2];

//-----------------------------------------------------------------------+
// Function     :  cbi_check_cesr_intf                                   |
//                                                                       |
// Description  :  Check the contents of the control system CBPM mailbox |
//                 interface                                             |
//                                                                       |
// Prototypes   :  int cbi_check_cesr_intf(CTL_CBPM_INTERFACE *)         |
//                                                                       |
// Arguments    :  CTL_CBPM_INTERFACE structure pointer                  |
//                                                                       |
// Return Value :  requested action - requested command flag as          |
//                                    specified in                       |
//                                    cesr_interface.h and               |
//                                    cesr_interface.inc                 |
//                                                                       |
// Author       :  M. Palmer (4/2/2009)                                  |
//-----------------------------------------------------------------------+
int cbi_check_cesr_intf(CTL_INTERFACE *interface) {

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = (char *)__FUNCTION__;

   int retval;
   int failure = FALSE;
//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+

// pointers to data locations
   int *p_lock          = &(interface->lock);
   int *p_command       = &(interface->command);
   int *p_num_bunches   = &(interface->num_bunches);
   int *p_bunch_id      = &(interface->bunch_id[0]);
   int *p_num_samples   = &(interface->num_samples);
   int *p_num_cbpm_proc = &(interface->num_processes);
   int *p_cbpm_proc_id  = &(interface->process_id[0]);
   int *p_command_done  = &(interface->command_done[0]);
   int *p_heartbeat     = &(interface->heartbeat[0]);
   int *p_file_type     = &(interface->file_type[0]);
   int *p_file_id       = &(interface->file_id[0]);
   int *p_num_turns     = &(interface->num_turns);
   int *p_num_shots     = &(interface->num_shots);
   int *p_turn_select   = &(interface->turn_select);

//-----------------------------------------------------------------------+
// Retrieve the entire control structure                                 |
//-----------------------------------------------------------------------+
   ele[0] = ELE_LOCK;
   ele[1] = ELE_LOCK;
   retval = vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_lock);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_COMMAND;
   ele[1] = ELE_COMMAND;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_command);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_NUM_BUNCHES;
   ele[1] = ELE_NUM_BUNCHES;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_bunches);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_BID;
   ele[1] = ELE_BID + CBI_MAX_CTL_BUNCH_LIST - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_bunch_id);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_NUM_SAMPLES;
   ele[1] = ELE_NUM_SAMPLES;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_samples);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_NUM_PROCESSES;
   ele[1] = ELE_NUM_PROCESSES;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_cbpm_proc);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_PROCESS_ID;
   ele[1] = ELE_PROCESS_ID + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_cbpm_proc_id);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_COMMAND_DONE;
   ele[1] = ELE_COMMAND_DONE + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_command_done);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_HEARTBEAT;
   ele[1] = ELE_HEARTBEAT + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_heartbeat);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_FILE_TYPE;
   ele[1] = ELE_FILE_TYPE + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_file_type);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_FILE_ID;
   ele[1] = ELE_FILE_ID + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_file_id);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_NUM_TURNS;
   ele[1] = ELE_NUM_TURNS + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_turns);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_NUM_SHOTS;
   ele[1] = ELE_NUM_SHOTS + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_shots);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   ele[0] = ELE_TURN_SELECT;
   ele[1] = ELE_TURN_SELECT + CBI_MAX_CTL_PROCESSES - 1;
   retval =vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_turn_select);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
     failure = TRUE;
   }

   fflush(stdout);
	    
// Return status
   if (failure) {
     return F_FAILURE;
   } else {
     return F_SUCCESS;
   }

} 
//-----------------------------------------------------------------------+
// End function cbi_check_cesr_intf                                      |
//-----------------------------------------------------------------------+


//-----------------------------------------------------------------------+
// Function     :  cbi_dump_cesr_intf                                    |
//                                                                       |
// Description  :  Dumps the command information from a CBPM CONTROL     |
//                 mailbox, for diagnostic purposes, upon a CBPM command |
//                 request by the control system.                        | 
//                                                                       |
// Prototype    :  int cbi_dump_cesr_intf(int, CTL_CBPM_INTERFACE *)     |
//                                                                       |
// Arguments    :  mode      - CTL_INTF_FULL_DUMP or CTL_INTF_CMD_DUMP   |
//                 interface - Pointer to CTL_INTERFACE structure        |
//                                                                       |
// Return Value :  Success or failure                                    |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

int cbi_dump_cesr_intf(int mode, CTL_INTERFACE *interface) {

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = (char *)__FUNCTION__;

   char *blanks  = "";
   char *blanks2 = "     ";

   char *c_species  = NULL;
   char *c_electron = "electron";
   char *c_positron = "positron";

   int species, train, bunch;

   int i;
   char name[30];
   
//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+

// pointers to data locations
   int *p_lock          = &(interface->lock);
   int *p_command       = &(interface->command);
   int *p_num_bunches   = &(interface->num_bunches);
   int *p_bunch_id      = &(interface->bunch_id[0]);
   int *p_num_samples   = &(interface->num_samples);
   int *p_num_cbpm_proc = &(interface->num_processes);
   int *p_cbpm_proc_id  = &(interface->process_id[0]);
   int *p_command_done  = &(interface->command_done[0]);
   int *p_heartbeat     = &(interface->heartbeat[0]);
   int *p_file_type     = &(interface->file_type[0]);
   int *p_file_id       = &(interface->file_id[0]);
   int *p_num_turns     = &(interface->num_turns);
   int *p_num_shots     = &(interface->num_shots);
   int *p_turn_select   = &(interface->turn_select);

//-----------------------------------------------------------------------+
// Print the entire control structure (assumed to be already retrived)   | 
//-----------------------------------------------------------------------+

   // Print the dump header
   sprintf(message, " ");
   log_message(S_BLANK,func_name, message);
   sprintf(message, "CBPM CONTROL Interface Dump:  ");
   log_message(S_DINFO,func_name, message);
   sprintf(message, " ");
   log_message(S_BLANK,func_name, message);

   // Lock Status
   if (mode == CBI_CTL_INTF_FULL_DUMP) {
      sprintf(message, "%s LOCK = %d", blanks, *p_lock);
      log_message(S_BLANK, func_name, message);
   }

   // Command information
   // First get command name
   switch (*p_command) {
      
      case CBPM_EXIT_FLAG:
         sprintf(name, CBPM_EXIT_NAME);
         break;
      case CBPM_NOCMD_FLAG:
         sprintf(name, CBPM_NOCMD_NAME);
         break;
      case CBPM_IDLE_BPM_FLAG:
         sprintf(name, CBPM_IDLE_BPM_NAME);
         break;
      case CBPM_ORBIT_FLAG:
         sprintf(name, CBPM_ORBIT_NAME);
         break;
      case CBPM_PHASE_FLAG:
         sprintf(name, CBPM_PHASE_NAME);
         break;
      case CBPM_HEP_DATA_FLAG:
         sprintf(name, CBPM_HEP_DATA_NAME);
         break;
      case CBPM_CONT_DATA_FLAG:
         sprintf(name, CBPM_CONT_DATA_NAME);
         break;
      case CBPM_MULTI_BUNCH_FLAG:
         sprintf(name, CBPM_MULTI_BUNCH_NAME);
         break;
      case CBPM_TURNS_ORBIT_FLAG:
         sprintf(name, CBPM_TURNS_ORBIT_NAME);
         break;
      case CBPM_INJ_TRAJ_FLAG:
         sprintf(name, CBPM_INJ_TRAJ_NAME);
         break;
      case CBPM_PEDESTALS_FLAG:
         sprintf(name, CBPM_PEDESTALS_NAME);
         break;
      case CBPM_SHAKER_DATA_FLAG:
         sprintf(name, CBPM_SHAKER_DATA_NAME);
         break;
      case CBPM_WRITE_BTNS_FLAG:
         sprintf(name, CBPM_WRITE_BTNS_NAME);
         break;
      case CBPM_COMMAND_DISABLE:
         sprintf(name, CBPM_COMMAND_DISABLE_NAME);
         break;
      case CBPM_COMMAND_ENABLE:
         sprintf(name, CBPM_COMMAND_ENABLE_NAME);
         break;
      default:
         sprintf(name, "UNKNOWN COMMAND");
         break;
   }

   sprintf(message, "%s Command ID = %d, Command Name = %s", blanks, *p_command, name);
   log_message(S_BLANK, func_name, message);

   // Number of bunches requested
   sprintf(message, "%s Num Bunches = %d", blanks, *p_num_bunches);
   log_message(S_BLANK, func_name, message);

   // Bunch list
   for (i = 0; i < *p_num_bunches; i++) {
      species = cbi_bid2species(*(p_bunch_id + i));
      train   = cbi_bid2train(*(p_bunch_id + i));
      bunch   = cbi_bid2bunch(*(p_bunch_id + i));

      if (species == ELECTRONS) {
         c_species = c_electron;
      }   
      else {
         c_species = c_positron;
      }

      sprintf(message, "%s %d:  Bunch ID = %d, Species = %d(%s), Train = %d, Bunch = %d", 
              blanks2, i, *(p_bunch_id+i),species, c_species, train, bunch);
      log_message(S_BLANK, func_name, message);
   }

   // Number of samples requested
   sprintf(message, "%s Num Samples = %d", blanks, *p_num_samples);
   log_message(S_BLANK, func_name, message);

   if (mode == CBI_CTL_INTF_FULL_DUMP) {
      sprintf(message, "%s Num Active CBPM Processes = %d", blanks, *p_num_cbpm_proc);
      log_message(S_BLANK, func_name, message);

      // Process ID      
      for (i = 0; i < *p_num_cbpm_proc; i++) {
         sprintf(message, 
                 "%s Process ID = %5d  Status = %5d  Heartbeat = %3d  Last File Type = %3d  Last File ID = %d", 
                 blanks2, *(p_cbpm_proc_id + i), *(p_command_done + i), *(p_heartbeat + i), *(p_file_type + i), 
                 *(p_file_id + i));
         log_message(S_BLANK, func_name, message);
      }
   }

   // Number of turns requested
   sprintf(message, "%s Num Turns = %d", blanks, *p_num_turns);
   log_message(S_BLANK, func_name, message);

   // Number of shots requested
   sprintf(message, "%s Num Shots = %d", blanks, *p_num_shots);
   log_message(S_BLANK, func_name, message);

   // Turn after injection trigger
   sprintf(message, "%s Turn after Trigger for Data Start= %d", blanks, *p_turn_select);
   log_message(S_BLANK, func_name, message);

   sprintf(message, " ");
   log_message(S_BLANK,func_name, message);

   fflush(stdout);    

   // Return status
   return F_SUCCESS;
} 
//-----------------------------------------------------------------------+
// End function cbi_dump_cesr_intf                                       |
//-----------------------------------------------------------------------+



//-----------------------------------------------------------------------+
// Function     :  cbi_lock_cesr_intf                                    |
//                                                                       |
// Description  :  Sets hardware and software locks on the CBPM CONTROL  |
//                 node in the CESR DB.                                  |
//                                                                       |
// Prototype    :  int cbi_lock_cesr_intf(int *)                         |
//                                                                       |
// Arguments    :  ele[] - pointer to array holding first and last       |
//                         elements to lock with a mailbox semaphore     |
//                                                                       |
// Return Value :                                                        |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

int cbi_lock_cesr_intf(int mode, int *hardware_ele){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = (char *)__FUNCTION__;

   int get_lock;                       // hardware lock flag
   int lock_stat;                      // lock status flag
   int ele[2] = {ELE_LOCK, ELE_LOCK};  // specifies software lock element

   int istat;
   int lock;
   int *p_lock = &lock;


//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// Set up hardware and software locks in order to take over control of   |
// any CBPM processes that are presently active and under control system |
// control.                                                              |
//-----------------------------------------------------------------------+

// Check for a hardware lock on the 1st control system entries of the 
// CBPM CONTROL node
   if (mode == CBI_SET_LOCK) {
      get_lock = vmglock_c(CBPM_CONTROL_NODE, hardware_ele[0], hardware_ele[1], False);

      // Diagnostic dump
      if (get_lock == CBI_LOCK_SUCCESS) {
         sprintf(message, "VMGLOCK_C return value:  %d - SUCCESS", get_lock);
      } else if (get_lock == CBI_LOCK_FAILURE) {
         sprintf(message, "VMGLOCK_C return value:  %d - FAILURE", get_lock);
      } else if (get_lock == CBI_LOCK_EXISTS) {
         sprintf(message, "VMGLOCK_C return value:  %d - LOCK ALREADY EXISTS", get_lock);
      } else {
         sprintf(message, "VMGLOCK_C return value:  %d - UNKNOWN", get_lock);
      }
      if (get_lock != CBI_LOCK_SUCCESS) {
         log_message(S_ERROR, func_name, message);
      }

      // Force lock return value -  THIS NEEDS TO BE FIXED!!!!
      get_lock = CBI_LOCK_SUCCESS;

      if (get_lock == CBI_LOCK_FAILURE) {
         sprintf(message, "Could not get semaphore lock for %s", 
                 CBPM_CONTROL_NODE);
         log_message(S_ERROR, func_name, message);
         sprintf(message, "Node %s apparently in use by another process", 
                 CBPM_CONTROL_NODE);
         log_message(S_WARNING, func_name, message);
         
	 // Return error
	 fflush(stdout);
         return CBPM_LOCK_ERROR;
      }
      
      // Now have a hardware lock:  
      // Next check for a pre-existing software lock.  If it doesn't exist 
      // set it here, otherwise punt.
      istat = vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_lock);
      if (*p_lock == CESR_CMD_LOCK) {
         sprintf(message, "Could not get software lock for %s",
                 CBPM_CONTROL_NODE);
         log_message(S_ERROR, func_name, message);
         
         // Release hardware lock and return with errors
         lock_stat = vmpunlock_c(CBPM_CONTROL_NODE, hardware_ele[0], hardware_ele[1]);
         
         // Return error
	 fflush(stdout);
         return CBPM_SLOCK_ERROR;
         
      } else {
         *p_lock = CESR_CMD_LOCK;
         vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_lock);
      }      
      
      fflush(stdout);
      return CBPM_REQUEST_SUCCESS;

   } else if (mode == CBI_REFRESH_LOCK) {

      lock_stat = vmglock_c(CBPM_CONTROL_NODE, hardware_ele[0], hardware_ele[1], False);

      fflush(stdout);
      return CBPM_REQUEST_SUCCESS;

   } else if (mode == CBI_RELEASE_LOCK) {

     // Remove the software lock
      *p_lock = CESR_CMD_NOLOCK;
      vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_lock);
      
      // Remove the hardware lock
      lock_stat = vmpunlock_c(CBPM_CONTROL_NODE, hardware_ele[0], hardware_ele[1]);
      fflush(stdout);
      return CBPM_REQUEST_SUCCESS;

   } else {
      sprintf(message, "Invalid mode specified:  %d", mode);
      log_message(S_ERROR, func_name, message);
      fflush(stdout);
      return CBPM_MISC_ERROR;
   }
}
//-----------------------------------------------------------------------+
// End function cbi_lock_cesr_ctl                                        |
//-----------------------------------------------------------------------+



//-----------------------------------------------------------------------+
// Function     :  cbi_register_cesr_intf                                |
//                                                                       |
// Description  :  Registers/unregisters the current CBPM server in the  |
//                 CESR "CBPM Control" process list.                     |  
//                                                                       |
// Prototype    :  int cbi_register_cesr_intf(int mode)                  |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  M. Palmer                                             |
//                                                                       |
//-----------------------------------------------------------------------+

int cbi_register_cesr_intf(int mode) {

  //-----------------------------------------------------------------------+
  // Local Variables -                                                     |
  //                                                                       |
  // func_name  -  Pointer to name of the local function                   |
  //                                                                       |
  //-----------------------------------------------------------------------+

   char *func_name = (char *)__FUNCTION__;

   // Pointers to data locations
   int  num_cbpm_proc;
   int *p_num_cbpm_proc = &num_cbpm_proc;
   int  proc_list[CBI_MAX_CTL_PROCESSES];
   int *p_cbpm_proc_id;

   int proc, slot;

   int clear    = CBI_CTL_IFACE_CLEAR_VALUE;
   int *p_clear = &clear;

   // Initialize variables specifying the "CBPM CONTROL" node
   char *node = CBPM_CONTROL_NODE;
   int   ele[2];

   // Set the hardware lock to extend from the software lock element to 
   // through the PROCESS_ID list
   int hdw_lock_ele[2] = {ELE_LOCK, ELE_COMMAND_DONE - 1};

   int lock_status;

   int status = F_SUCCESS;

   //-----------------------------------------------------------------------+
   //                     EXECUTABLE CODE STARTS HERE                       |
   //-----------------------------------------------------------------------+
   

   //-----------------------------------------------------------------------+
   // Check and set locks on the CBPM CONTROL node                          |
   //-----------------------------------------------------------------------+
   lock_status = cbi_lock_cesr_intf(CBI_SET_LOCK, hdw_lock_ele);
   if (lock_status == CBI_LOCK_FAILURE) {

     // Log error message
     sprintf(message, "Unable to lock CBPM CONTROL node");
     log_message(S_ERROR, func_name, message);
     fflush(stdout);

     return F_FAILURE;
   }

   //-----------------------------------------------------------------------+
   // Retrieve the number of active BPM server processes                    |
   //-----------------------------------------------------------------------+
   ele[0] = ELE_NUM_PROCESSES;
   ele[1] = ELE_NUM_PROCESSES;
   vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_cbpm_proc);

   

   if (mode == CBI_INTF_CONNECT) {  // Connect procedure

     //--------------------------------------------------------
     // If adding a server, see if all slots are already full.
     //--------------------------------------------------------
     if (num_cbpm_proc >= CBI_MAX_CTL_PROCESSES) {
       sprintf(message, "All CBPM server slots full!");
       log_message(S_ERROR, func_name, message);
       status = F_FAILURE;
     }

     //-----------------------------------------------------------------------+
     // Retrieve the CBPM active server list and verify that no duplicate     |
     // entry exists for this server.                                         |
     //-----------------------------------------------------------------------+
     ele[0] = ELE_PROCESS_ID;
     ele[1] = ELE_PROCESS_ID + CBI_MAX_CTL_PROCESSES - 1;
     p_cbpm_proc_id = proc_list;
     vxgetn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_cbpm_proc_id);
     for (proc = 0; proc < CBI_MAX_CTL_PROCESSES; proc++) {
       if (proc_list[proc] == CTL_CBPM_Intf_Monitor.ProcessID) {
	 sprintf(message, "CBPM Server ID  %3d  already in use!",
		 CTL_CBPM_Intf_Monitor.ProcessID);
	 log_message(S_ERROR, func_name, message);
	 status = F_FAILURE;
       }
     }
     
     //-----------------------------------------------------------------------+
     // All slots are not full and there is not already an instance of this   |
     // server ID.  Now locate the first empty slot.                          |
     //-----------------------------------------------------------------------+
     for (proc = 0; proc < CBI_MAX_CTL_PROCESSES; proc++) {
       if (proc_list[proc] == clear) {
	 slot = proc;
	 break;
       }
     }

     //-----------------------------------------------------------------------+
     // Now it is safe to update the list of active BPM processes             |
     //-----------------------------------------------------------------------+
     if (status == F_SUCCESS) {
       CTL_CBPM_Intf_Monitor.MailboxID = slot;
       
       ele[0] = ELE_NUM_PROCESSES;
       ele[1] = ELE_NUM_PROCESSES;
       *p_num_cbpm_proc += 1;
       vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_cbpm_proc);
       
       ele[0]         = ELE_PROCESS_ID + CTL_CBPM_Intf_Monitor.MailboxID;
       ele[1]         = ele[0];
       p_cbpm_proc_id = &(CTL_CBPM_Intf_Monitor.ProcessID);
       vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_cbpm_proc_id);
     }

     //-----------------------------------------------------------------------+
     // Set interface enabled flag in central management structure.           |
     //-----------------------------------------------------------------------+
     CTL_System.InterfaceState = CBI_CTL_INTF_ENABLED;


   } else {  // Disconnect procedure

     
     // Clear the process ID for the current process
     ele[0] = ELE_PROCESS_ID + CTL_CBPM_Intf_Monitor.MailboxID;
     ele[1] = ele[0];
     vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_clear);
     
     // Decrement active server count by one since this is only
     // de-registering a single server.
     printf("Previous num servers = %d\n", *p_num_cbpm_proc);
     (*p_num_cbpm_proc)--;
     ele[0] = ELE_NUM_PROCESSES;
     ele[1] = ELE_NUM_PROCESSES;
     printf("Num servers= %d\n", *p_num_cbpm_proc);
     vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], p_num_cbpm_proc);

     // Clear the command_done slot for this server
     ele[0] = ele[1] = ELE_COMMAND_DONE + CTL_CBPM_Intf_Monitor.MailboxID;
     printf("Clearing 'command done' field\n");
     vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], &clear);

     //-----------------------------------------------------------------------+
     // Set interface off flag in central management structure.               |
     //-----------------------------------------------------------------------+
     CTL_System.InterfaceState = CBI_CTL_INTF_OFF;


   }      


   // Release all locks
   lock_status = cbi_lock_cesr_intf(CBI_RELEASE_LOCK, hdw_lock_ele);
   if (lock_status != CBPM_REQUEST_SUCCESS) {
      sprintf(message, 
              "Failure to release CBPM CONTROL lock!  Manual UNLOCK required!");
      log_message(S_ERROR, func_name, message);
      status = F_FAILURE;
   }

   fflush(stdout);
   return status;
} 
//-----------------------------------------------------------------------+
// End function cbi_register_cesr_ctl                                    |
//-----------------------------------------------------------------------+




//-----------------------------------------------------------------------+
// Function     :  cbi_reply_cesr_intf.c                                 |
//                                                                       |
// Description  :  Returns status info from a CESR Control System        |
//                 command to the the requesting routine by means of the |
//                 CBPM CONTROL interface arrays.                        |
//                                                                       |
// Prototype    :  int cbi_reply_cesr_intf(int status)                   |
//                                                                       |
// Arguments    :  int status                                            |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

int cbi_reply_cesr_intf(int status) {

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = (char *)__FUNCTION__;

   // Elements of the "CBPM CONTROL" node
   int   ele[2];
   int offset, retval;

//-----------------------------------------------------------------------+
//                     EXECUTABLE CODE STARTS HERE                       |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// Get offset for current process                                        |
//-----------------------------------------------------------------------+
   offset = CTL_CBPM_Intf_Monitor.MailboxID;


//-----------------------------------------------------------------------+
// If a file has been written, put its information into the interface    |
//  TODO: Move to up above the COMMAND DONE step.
//-----------------------------------------------------------------------+

   if (CTL_CBPM_Intf_Monitor.FileStatus == CBI_CTL_NEW_FILE) {
      
     // Save file type
     ele[0] = ELE_FILE_TYPE + offset;
     ele[1] = ele[0];
     retval = vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], &(CTL_CBPM_Intf_Monitor.FileType));
     if (retval != MPM_SUCCESS) {
       sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
       log_message(S_ERROR, func_name, message);
     }

     // Save file ID
     ele[0] = ELE_FILE_ID + offset;
     ele[1] = ele[0];
     retval = vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], &(CTL_CBPM_Intf_Monitor.FileID));
     if (retval != MPM_SUCCESS) {
       sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
       log_message(S_ERROR, func_name, message);
     }

     // Set status to OLD
     CTL_CBPM_Intf_Monitor.FileStatus == CBI_CTL_OLD_FILE;
   }


//-----------------------------------------------------------------------+
// Write the reply to the CBPM CONTROL node                              |
//-----------------------------------------------------------------------+
   ele[0] = ELE_COMMAND_DONE + offset;
   ele[1] = ele[0];
   
   retval = vxputn_c(CBPM_CONTROL_NODE, ele[0], ele[1], &status);
   if (retval != MPM_SUCCESS) {
     sprintf(message,"ERROR reading from %s elements %d-%d." ,CBPM_CONTROL_NODE, ele[0], ele[1] );
     log_message(S_ERROR, func_name, message);
   }


   // Return status
   return F_SUCCESS;
} 
//-----------------------------------------------------------------------+
// End function cbi_reply_cesr_intf                                      |
//-----------------------------------------------------------------------+








        
