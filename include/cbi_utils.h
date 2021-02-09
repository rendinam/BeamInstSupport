#ifndef CBI_UTILS_H
#define CBI_UTILS_H
//-----------------------------------------------------------------------+
// File         :  cbi_utils.h                                           |
//                                                                       |
// Description  :  Defines parameters and function prototypes for        |
//                 various utility functions in this library.            |
//                 Todo: Merge into main prototypes file?                |
//                                                                       |
// Author       :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif

//-----------------------------------------------------------------------+
// Utils Function Prototypes -                                           |
//-----------------------------------------------------------------------+
/*   extern int cbi_command_idx_from_name( CBI_COMMAND *cmd_list, char *command_name ); */

/*   extern int cbi_sleep(int); */
/*   extern int cbi_get_bid(int,int,int); */
/*   extern int cbi_bid2species(int); */
/*   extern int cbi_bid2train(int); */
/*   extern int cbi_bid2bunch(int); */
  
/*   extern int cbi_command_status_summary( FILE *fp, char *name ); */
/*   extern int cbi_gate_control(int gate_state); */
/*   extern int cbi_init_command( int BPM_IDX, int cmd ); */
/*   extern int cbi_init_prompts( CBI_INPUT_PROMPT_DESCRIPTOR *prompts, int num_prompts ); */
/*   extern int cbi_execute_command( int BPM_IDX ); */
/*   extern int cbi_wait_for_command(int BPM_IDX); */

/*   //extern int cbi_inst_loop( int beg_idx, int end_idx, void (*p_func), int num_args, ... ); */
/*   extern int cbi_inst_loop( int beg_idx, int end_idx, void (*p_func), int parallel, int num_args, ... ); */
/*   extern int cbi_inst_status_handler( CBI_COMMAND *cmd_list, int *command_index ); */
/*   extern int cbi_load_command( CBI_COMMAND *cmd_list, int command_list_len, char *command_name, int provide_prep_call ); */
/*   extern int num_online( void ); */

/*   extern int cbi_compute_comm_struct_size( const COMM_KEY_CONFIG *p_cfg ); */
/*   extern int cbi_copy_touched_structs( int BPM_IDX, int num_plat_comm_structs ); */
/*   extern int cbi_success( void ); */
/*   extern int cbi_set_handshake_tries( int tries ); */
/*   extern int cbi_wipe_comm_structs(CBI_CTL_MODULE *temp_struct_tree, int num_plat_comm_structs); */
/*   extern int cbi_dispatch_sequencer( CBI_COMMAND cmd_list[], int command_index, int num_comm_structs, int num_sequence_steps ); */

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif

#endif  // CBI_CO_UTILS_H






