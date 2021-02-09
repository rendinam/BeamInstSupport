//-----------------------------------------------------------------------+
// File         :  cbi_wipe_comm_structs.c                               |
//                                                                       |
// Description  : Wipes a given communication structure with a           |
//                pre-defined bit pattern so that changes to the         |
//                made by the user may be detected.                      |
//                                                                       |
// Arguments    :  - Pointer to a single instrument's master structure   |
//                   tree                                                |
//                 - The total number of declared communications structs | 
//                   for a given platform.                               |
//                                                                       |
// Author       :  M. Rendina                                            |
//                              TODO: Allow discrimination for bit       |
//                                    patterns wthin structures that may |
//                                    coincidentcally acquire the wiping |
//                                    pattern.  (I.e. bunch patterns)    |
//-----------------------------------------------------------------------+

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"

#include "cbi_core_includes.h"


int cbi_wipe_comm_structs(CBI_CTL_MODULE *temp_struct_tree, int num_plat_comm_structs) {

  int  struct_tag;
  int debug_level = CTL_System.debug_verbosity;

  const COMM_KEY        *p_key;
  const COMM_KEY_CONFIG *p_cfg;

  int struct_size;
  
  // Visit each struct by tag number starting at 
  // FIRST_LOCAL_TAG through (FIRST_LOCAL_TAG + num_plat_comm_structs)
  for (struct_tag = CBI_FIRST_LOCAL_TAG; struct_tag < num_plat_comm_structs+CBI_FIRST_LOCAL_TAG; struct_tag++) {

    p_key = &(temp_struct_tree->key[ temp_struct_tree->tag2key_map[struct_tag]]);
    p_cfg = p_key->p_config;

    if (p_cfg->protection == CBI_CTL_READ_WRITE) {
      struct_size = cbi_compute_comm_struct_size( p_cfg );
      if (debug_level > CBI_DEBUG_LEVEL_4) {
	printf("%s  is R/W   and has size = %d \n", p_cfg->name, struct_size);
      }
      // Sets consecutive BYTES.  Therefore, multiply size by 4.
      memset( p_key->data.ptr[0], CBI_COMM_WIPE_BYTE_VALUE, struct_size * 4 );
    } 

  } //endFOR struct_tag


  return F_SUCCESS;

}



// Support functions for this file ========================


// Calculate number of words in a given communications struct
int cbi_compute_comm_struct_size( const COMM_KEY_CONFIG *p_cfg ) {

  int i, dcount, ele_size, total_size = 0;

  for (i = 0; i < p_cfg->num_vars; i++) {
    dcount   = p_cfg->data_count[i];
    ele_size = p_cfg->ele_size[i];
    total_size += dcount * ele_size;
  }

  return total_size;

}
