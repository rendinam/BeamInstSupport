#ifndef CBI_PROTOS_H
#define CBI_PROTOS_H
//-----------------------------------------------------------------------+
// File         :  cbi_protos.h                                          |
//                                                                       |
// Description  :  Function prototypes for the bulk of the routines      |
//                 provided in BeamInstSupport for core instrumentation  |
//                 functionality needed on the control system.           |
//-----------------------------------------------------------------------+

#include "faccess.h"
#include "cbi_gp_plotting.h"
#include "cbi_cesr_interface.h"
#include "cbpmfio.h"

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif

  extern int  cleanup_sockets(void);
  extern void std_sigint_handler( int signum );
  extern int  cbi_mem_setup( void );

  extern int  cbi_prompt_pattern_mode( void );
  extern int  cbi_dump_interface(void);
  extern int  cbi_exit_app( void );
  extern int  cbi_get_debug_prep( void );
  extern int  cbi_get_debug_post(int iidx);
  extern int  cbi_idle_post(int iidx);
  extern int  cbi_list_online_instruments( void );
  extern int  cbi_toggle_active_instruments( void );
  extern int  cbi_power_cycle_prep(void);
  extern int  cbi_power_cycle_post(int iidx);
  extern int  cbi_power_cycle_final(void);
  extern int  cbi_program_flash_prep(void);
  extern int  cbi_program_flash_post(int iidx);
  extern int  cbi_active( int iidx );
  extern int  cbi_num_active();
  extern int  cbi_toggle_activity( int iidx );
  extern int  cbi_close_mpmnet_connection(void);
  extern int  cbi_print_cmd_struct(int iidx, FILE *fp);
  extern int  cbi_print_debug_struct(int iidx, FILE *fp);
  extern int  cbi_print_stat_struct(int iidx, FILE *fp);
  extern int  cbi_xbus_peek( int iidx, int addr, int* value );
  extern int  cbi_xbus_poke( int iidx, int addr, int value );

  // file_io
  extern int   cbi_copy_file(char *dest, char *from);
  extern int   cbi_copy_file_by_ptr(FILE *fpd, FILE *fps);
  extern int   cbi_datafile_name( int filetype, char *filename, int iidx, int file_idx);
  extern int   cbi_datafile_name_single( int filetype, char *filename, int file_idx);

  extern int   cbi_new_file_idx(int dirspec, int bump);
  extern int   cbi_read_allocation( char *filename, char *alloc_scheme, char *alloc_name );
  extern int   cbi_manager_read_allocations(int num_backends);
  extern int   cbi_read_config_file( char *filename );

  extern int   cbi_prep_datafile_header(RDV3_FILE_HEADER *hdata, int filetype, int iidx, int file_idx );
  extern int   cbi_prep_cesr_header(RDV3_CESR_HEADER *cdata);


  extern int   cbi_write_param_file_header(FILE *fp, int file_idx, int session_based);
  extern int   cbi_add_param( PARAMETER_INFO *pars, char *name, int rows, int cols, int type, void *ptr );

  extern int   cbi_select_saved_pattern( char *category, int pattern_array[] );
  extern int   cbi_select_saved_pattern_cat( int pattern_array[] );

  extern int   cbi_new_pattern_number( char *BP_category );
  extern int   cbi_save_bunch_pattern( int pattern_array[], char *BP_category, char *BP_name, int BP_entry_num );
  extern FILE* cbi_managed_file_open( char *filename, char *full_fname, int iidx, int dirspec, int filetype, int multiplicity, int supplemental );
  extern FILE* cbi_managed_file_open_fixedindex( char *filename, char *full_fname, int iidx, int dirspec, int filetype);

  extern int    fa_datdump( FILE *fp, int Yrange[], int Xrange[], int type, char *format, void *data );
  extern int    fa_extract_section_order( FILE *fp, char sections[][FA_MAX_STRING_LENGTH] );
  extern int    file_subdir( int file_idx, char *subdir_string );

  extern void   cbi_noop(int dummy, ...);
  extern int    cbi_scan_line_input( FILE *fp, char *format, ... );
  extern int    cbi_scan_line_output( FILE *fp, char *format, ... );
  extern int    cbi_bunch_patt_input( FILE *fp, char *format, ... );
  extern int    cbi_bunch_patt_output( FILE *fp, char *format, ... );


  // user_io
  extern int   cbi_compose_bunch_pattern( int pattern_array[], int timing_mode );
  extern int   cbi_prompter( int num_prompts, CBI_INPUT_PROMPT_DESCRIPTOR *pd_list );
  extern int   cbi_query_active(void);
  extern int   cbi_query_action( CBI_COMMAND commands[], int num_cmds );
  extern int   cbi_int_range_validator( int val, int low, int high );
  
  // utils
  extern int   cbi_bitval( int word, int bit );
  extern int   cbi_parity( int number );
  extern int   compose_tblock_list( int rot_bunch_pat[], int tblock_list[] );
  extern char  cbi_get_char(void);
  extern char  cbi_get_char_local(void);
  extern int   cbi_get_int(void);
  extern int   cbi_get_int_local(void);
  extern float get_cern_current(void);
  extern float get_species_current(int species);
  extern int   cbi_manager_input_handler( CBI_COMMAND commands[], int num_cmds );
  extern int   cbi_manager_selection_handler( int num_backends, char sel_specs[][CBI_SELEC_SPEC_LENGTH] );
  extern int   cbi_check_heartbeat( int iidx );
  extern int   cbi_check_timing_integrity( int iidx );

  extern int   cbi_set_bunch( int pattern_array[], int bunch_bit_idx );
  extern int   cbi_unset_bunch( int pattern_array[], int bunch_bit_idx );
  extern int   cbi_clear_bp( int pattern_array[] );
  extern int   cbi_get_errors(int* error_array, int *error_list, int num_error_codes);

  extern int   cbi_print_online_list( FILE *fp );

  extern int   replace( char *string, char old, char new);
  extern char  *replace_str(char *st, char *orig, char *repl);
  extern int   spaces( FILE *fp, int padding );
  extern int   strip( char *string );
  
  extern int   ctl_cbpm_interface(CBI_COMMAND cmd_list[], int num_commands, int num_comm_structs);

  extern int   cbi_print_mpm_iface_contents( CTL_INTERFACE *p_interface );


  // comms
  extern int   cbi_init_mpm_comms(void);
  extern int   cbi_open_sockets_active( void );
  extern int   cbi_close_sockets( void );
  extern int cbi_MPMnet_keepalive( int *keepalive );

  extern int   gp_plot( char *full_fname, int col_start, int col_end, PLOT_PARAMS pparams, char *plot_title, char titles[][CBI_MAX_STRING_LENGTH] );

  extern int cbi_command_idx_from_name( CBI_COMMAND *cmd_list, char *command_name );

  extern int cbi_sleep(int);
  extern int cbi_get_bid(int,int,int);
  extern int cbi_bid2species(int);
  extern int cbi_bid2train(int);
  extern int cbi_bid2bunch(int);
  
  extern int cbi_command_status_summary( FILE *fp, char *name );
  extern int cbi_gate_control(int gate_state);
  extern int cbi_init_command( int BPM_IDX, int cmd );
  extern int cbi_init_prompts( CBI_INPUT_PROMPT_DESCRIPTOR *prompts, int num_prompts );
  extern int cbi_execute_command( int BPM_IDX );
  extern int cbi_wait_for_command(int BPM_IDX);

  //extern int cbi_inst_loop( int beg_idx, int end_idx, void (*p_func), int num_args, ... );
  extern int cbi_inst_loop( int beg_idx, int end_idx, void (*p_func), int parallel, int num_args, ... );
  extern int cbi_inst_status_handler( CBI_COMMAND *cmd_list, int *command_index );
  extern int cbi_load_command( CBI_COMMAND *cmd_list, int command_list_len, char *command_name, int provide_prep_call );
  extern int num_online( void );

  extern int cbi_compute_comm_struct_size( const COMM_KEY_CONFIG *p_cfg );
  extern int cbi_copy_touched_structs( int BPM_IDX, int num_plat_comm_structs );
  extern int cbi_compare_xbus_and_ethernet(FILE *fp, int iidx, int struct_tag, int verboseTypeout, int* same, int* xbusAllZeros, int* ethernetAllZeros, int(*fptr)(FILE*,int), int* XBusEnabled );
  extern int cbi_success( void );
  extern int cbi_set_handshake_tries( int tries );
  extern int cbi_wipe_comm_structs(CBI_CTL_MODULE *temp_struct_tree, int num_plat_comm_structs);
  extern int cbi_dispatch_sequencer( CBI_COMMAND cmd_list[], int command_index, int num_comm_structs, int num_sequence_steps );


//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif

#endif  // CBI_PROTOS_H






