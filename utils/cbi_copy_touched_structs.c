/*!
*-------------------------------------------------------------------------+
* File         :  cbi_copy_touched_structs.c                              |
*                                                                         |
* Description  :  Copies all READ/WRITE communications structure contents |
*                 that have been modified from the wiped state brought    |
*                 about by the wipe_comm_structs() function.              |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"
#include "messages.h"

#include "cbi_core_includes.h"


int cbi_copy_touched_structs( int iidx, int num_plat_comm_structs ) {

  char *func_name = (char *)__FUNCTION__;

  long int struct_size = 0;
  int struct_tag, word, word2;
  int stat, retval = CBI_F_SUCCESS;

  int debug_level = CTL_System.debug_verbosity;

  if (debug_level > CBI_DEBUG_LEVEL_3) { 
    printf("iidx               = %d\n", iidx);
    printf("num_plat_comm_structs = %d\n", num_plat_comm_structs);
  }


  CBI_CTL_MODULE *p_ctl_mod_temp;
  p_ctl_mod_temp = CTL_System.p_Module_temp;

  const COMM_KEY         *p_key_src, *p_key_dest;
  const COMM_KEY_CONFIG  *p_cfg_src, *p_cfg_dest;


  int *ptr_src, *ptr_dest;


  for (struct_tag = CBI_FIRST_LOCAL_TAG; struct_tag < num_plat_comm_structs+CBI_FIRST_LOCAL_TAG; struct_tag++) {

    p_key_src = &(p_ctl_mod_temp->key[ p_ctl_mod_temp->tag2key_map[struct_tag]]);
    p_cfg_src = p_key_src->p_config;

    p_key_dest = &((CTL_System.p_Module[iidx])->key[ CTL_System.p_Module[iidx]->tag2key_map[struct_tag]]);
    p_cfg_dest = p_key_dest->p_config;


    if (p_cfg_src->protection == CBI_CTL_READ_WRITE) {

      struct_size   = cbi_compute_comm_struct_size( p_cfg_src );
      
      //--------------------------------------------------------
      // Check to determine if the temp comms structure's 
      // contents have changed since they were wiped.
      //
      // If the contents have changed, this structure received
      // parameters during the 'prep' phase and must be
      // copied to the real instrument structure tree before 
      // being transmitted to the instruments in preparation 
      // for the pending command execution.
      //--------------------------------------------------------
      if (debug_level > CBI_DEBUG_LEVEL_3) {
	printf("Struct #%d (%s) size = %d \n", struct_tag, p_cfg_src->name, struct_size);
      }

      ptr_src = p_key_src->data.ptr[0];

      for ( word = 0; word < struct_size; word++) {

	if (debug_level > CBI_DEBUG_LEVEL_3) {
	  printf("%4d) 0x%x\n", word, *ptr_src );
	}

	if ( *ptr_src != CBI_COMM_WIPE_WORD_VALUE) {
	  
	  ptr_src  = p_key_src->data.ptr[0];
	  ptr_dest = p_key_dest->data.ptr[0];


	  //-----------------------------------------------
	  // Restore wipevalues to 0 here.  
	  // FIXME: Temporary until strict param checks
	  //        / composition is in place.
	  //-----------------------------------------------
	  for ( word2 = 0; word2 < struct_size; word2++) {
	    
	    // Replace all WIPE_WORD sentry values with 0.
	    if ( *ptr_src == CBI_COMM_WIPE_WORD_VALUE) {
	      *ptr_src = 0;
	    }
	
	    ptr_src++;
	    
	  }


	  ptr_src  = p_key_src->data.ptr[0];
	  ptr_dest = p_key_dest->data.ptr[0];
	  
	  //---------------------------------------------------
          // Copy the structure's data from the temporary tree
	  // to the real instrument tree.
	  //---------------------------------------------------
	  memmove( ptr_dest, ptr_src, struct_size * 4 );

	  // Send this structure to the instrument
	  stat = cbi_gp_struct(WRITE, CTL_System.comm_method, struct_tag, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
	  if (stat != CBI_F_SUCCESS) {
	    sprintf(message, "Failed to send structure to instrument %s @ %s", 
		    CTL_System.p_Module[iidx]->comm.ethernet.hostname,
		    CTL_System.p_Module[iidx]->det.location);
	    log_message(S_ERROR, func_name, message);
	    // Flag communications error for this instrument and deactivate so it
	    // does not take part in the remainder of stages for this command.
	    // Communications error flag will trigger an instrument recovery process
	    // at the end of command processing.
	    CTL_System.p_Module[iidx]->status = CBI_F_COMM_ERROR;
	    CTL_System.p_Module[iidx]->active = CBI_INACTIVE;
	    retval = CBI_F_FAILURE;
	  }

	  if (debug_level > CBI_DEBUG_LEVEL_4) {
	    printf("  --SENT--  %32s (tag:%d) [code: %d] \n", p_cfg_dest->name, struct_tag, stat);
	  }

	  break;
	}

	ptr_src++;

      } //endFOR word


    } //endIF protection

  } // endFOR struct_tag

  return retval;

}
