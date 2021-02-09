#ifndef CBI_CTL_TYPES_H
#define CBI_CTL_TYPES_H
//-----------------------------------------------------------------------+
// File         :  cbi_control_types.h                                   |
//                                                                       |
// Description  :  This header file defines control system data          |
//                 structures for the Cornell Beam Instrumentation       |
//                 system. Control system typedefs for CBI modules.      |
//                                                                       |
//                 These are the nested data structures that are used on |
//                 the control side to hold, or point to those structs   |
//                 that hold, in a compartmentalized fashion, all        |
//                 descriptive, control, and identification infomation   |
//                 and collected data for a single BPM unit.             |
//                                                                       |
//                 Structures declared here hold information that the    |
//                 instrument cannot provide on its own and that the     |
//                 control system must supply and associate with a given |
//                 instrument.  void pointers are allocated to allow     |
//                 structs of arbitrary size/depth to be made part of    |
//                 this hierarchy to accomodate platform-specific sets   |
//                 of instrument data structures.                        |
//                                                                       |
// Author       :  M. Palmer / J. Kern / M. Rendina                      |
//-----------------------------------------------------------------------+
#include "utilities.h"
#include "cbi_command.h"
#include "cbi_constants.h"
#include "cbi_types_a.h"
#include "cbi_comm_struct.h"
#include "cbi_cesr_interface.h"



//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


#define NUM_MENUS           3
#define MX_MENU_CHARS      50  //max length of menu listing
#define MX_EXE_NAME_LEN    25


//-----------------------------------------------------------------------+
//                                                                       |
// XBUS_ADDRESS:    contains the description of a CESR DSP readout       |
//                  module.                                              |
//                                                                       |
//                  Variables -                                          |
//                                                                       |
//                  pkt_node      - Name of packet node for this module  |
//                                  (for DSP data structure access from  |
//                                  control system)                      |
//                  pkt_ele       - Packet node element for this module  |
//                  adr_node      - Name of address node for this module |
//                                  (for direct memory access from       |
//                                  control system)                      |
//                  adr_ele       - Address node element for this module |
//                  dat_node      - Name of data node for this module    |
//                                  (for direct memory access from       |
//                                  control system)                      |
//                  dat_ele       - Data node element for this module    |
//                  config        - Pointer to configuration data        |
//                                  structure (void to allow arbitrary   |
//                                  configuration data)                  |
//         Are the element values always identical?                      |
//-----------------------------------------------------------------------+
typedef struct {
  char  pkt_node[CBI_MAX_NODE_CHARS];
  char  adr_node[CBI_MAX_NODE_CHARS];
  char  dat_node[CBI_MAX_NODE_CHARS];
  int   element;
} XBUS_INFO;


//-----------------------------------------------------------------------+
// ETHERNET_INFO:   Ethernet- and TCP/IP-specific communications details.|
//                                                                       |
//                                                                       |
//-----------------------------------------------------------------------+
typedef struct {
  char hostname[CBI_MAX_NAME_CHARS];  // TCP/IP hostname of the instrument?
  int  IP_address[16];                // IP address of the instrument      
  //int  MAC_address;                   // Hardware MAC address (placeholder)
  int  socket_ID;                     // Socket file-descriptor number
} ETHERNET_INFO;
  

//-----------------------------------------------------------------------+
//                                                                       |
// MODULE_COMM:     Cluster of communication channel addressing structs. |
//                                                                       |
//                  Variables -                                          |
//                                                                       |
//                  xbus          - struct containing pointer and node   |
//                                  information for xbus communication   |
//                  ethernet      - struct containing network address    |
//                                  for ethernet communication           |
//                                                                       |
//-----------------------------------------------------------------------+
typedef struct {
  XBUS_INFO        xbus;
  ETHERNET_INFO    ethernet;  // Change this to a void *<name> pointer
} MODULE_COMM;                // and have control initialization set up
                              // a reference to ethernet information for
                              // instruments that support it.  Otherwise,
                              // this will point to NULL for non-equipped
                              // units.


//-----------------------------------------------------------------------+
//  CBI_DETECTOR:   Contains the description of the detector to which a  |
//                  BPM readout unit is attached.                        |
//                                                                       |
//                  Variables -                                          |
//                                                                       |
//                  name          - Detector name                        |
//                  location      - Shorthand location label 43W, 6E etc |
//                  type          - Detector type                        |
//                  position      - (x,y,s) coordinates of detector      |
//                  config        - Pointer to detector configuration    |
//                                  information (void to allow arbitrary |
//                                  configuration data)                  |
//                                                                       | 
//-----------------------------------------------------------------------+
// Note:  the function to specify the config pointer can be declared in  
//        the input file and the initialization call automatically placed
//        in the general initialization function.  
//-----------------------------------------------------------------------+
typedef struct {
  char    name[CBI_MAX_NAME_CHARS];
  char    location[CBI_MAX_NAME_CHARS];
  int     CESR_index;
  char    type[CBI_MAX_NAME_CHARS];
  float   position[3];
  void   *config;
} CBI_DETECTOR;
  



//-----------------------------------------------------------------------+
//  CBI_CTL_MODULE   :  HIGHEST LEVEL aggregate data structure for       |
//                      interacting with a beam measurement device. This |
//                      holds pointers to all necessary substructures as |
//                      defined in the platform-specific instrumentation |
//                      libraries and headers.                           |
//-----------------------------------------------------------------------+
typedef struct {
  void                    *control;    // pointer to instrument-specific control side structs
  CBI_DATA                *core;       // Cluster of communication data structs COMMON to all instruments. (DSP-mediated)
  void                    *dsp_data;   // Cluster of platform-specific communications data structs. (DSP-mediated)
  void                    *analysis;   // Cluster of platform-specific analysis structs.
  enum CTL_ACTIVE_MODULE   active;     // 'Instrument active' flag
  enum ACTION              db_update;  // Necessary?
  enum CBI_INST_STATUS     status;
  int                      in_recovery; // T/F flag.
  MODULE_COMM              comm;
  CBI_DETECTOR             det;
  int                      handshake_timeout;
  char                     inst_exe_name[MX_EXE_NAME_LEN]; // relocate?
  // --Members necessary for structure imaging--
  int                      n_keys;     
  COMM_KEY                 key[CBI_MAX_CBI_STRUCT_TAGS];
  int                      tag2key_map[CBI_MAX_CBI_STRUCT_TAGS];
} CBI_CTL_MODULE;




//===========================================================================


//-----------------------------------------------------------------------+
// CBI_INPUT_PROMPT_DESCRIPTOR : Input prompt descriptor configuration   |
//                               structure.  Holds all options necessary |
//                               for the automated user prompting engine |
//                               to display prompts in a customizable    |
//                               manner.                                 |
//                                                                       |
//        User configurable elements                                     |
//   active             If TRUE, present this prompt to the user         |
//                      If FALSE, skip it-- useful for conditional       |
//                      presentation of prompts.                         |
//                                                                       |
//  *prompt_text        Simple prompt text to present                    |
//                                                                       |
//  *supp_text_ptr      Pointer to single string containing              |
//                      supplementary explanation, instruction, or menu  |
//                      text for the given prompt.                       |
//                                                                       |
//   data_type          Data type expected                               |
//                                                                       |
//   default_value      Initial default value to present to user         |
//                                                                       |
//   last_val_default   Flag to enable default overriding with last      | 
//                      input value                                      |
//                                                                       |
//  *destination        Pointer to where input shall be placed           |
//                                                                       |
//  *harvester          Routine to collect more complex input than       |
//                      a single prompt can provide.                     |
//                                                                       |
//  *validator          Routine to check input for validity              |
//                                                                       |
//-------------------------                                              |
//        Internal management elements, not to be modified manually.     |
//                                                                       |
//   last_value         Previous value entered by user                   |
//-----------------------------------------------------------------------+
typedef struct {
  int     active;
  char   *prompt_text;
  char   *supp_text_ptr;
  int     data_type;
  int    *valid_inputs;
  int    *map_out;
  int     default_value;
  int     last_val_default;
  void   *destination;
  void   *harvester;
  void   *validator;
  //-------------------------
  int    *last_value;
} CBI_INPUT_PROMPT_DESCRIPTOR;


//===========================================================================

//----------------------------------------------------
// Structure for holding application version info.
//----------------------------------------------------
typedef struct {
  int    major;
  int    minor;
  int    patchlevel;
  char   modifier[10];
} CBI_APP_VERSION;



//---------------------------------------------------------------------------
// For holding instrument-independent information that shall be written to 
// or read in from a data file header.
// Writing a file will use this as a data source
// Reading a file will use this as a data sink
//---------------------------------------------------------------------------
typedef struct {
  // Pointers correspond to direct parameter mappings in instrument comm. structs.
  int   *p_bunch_pattern;
  int   *p_turn_spacing;
  int   *p_trigger;
  int   *p_num_turns;
  // Others have no direct command parameter correspondence or are derived 
  // from combinations of the above or the specific measurement type or command 
  // specified by the user and stored in data file.
  int   cesr_condx;
  float CERN_current;
  int   file_version;
  char  command_ID[CBI_MAX_STRING_LENGTH];
  char  timestamp[CBI_MAX_STRING_LENGTH];
  int   tsetup;
  char  trigger[30];
  int   num_instruments;
  int   inst_count;  // For iterator behavior in cbpm_rawfile_inst_names...
  int   num_bunches;
  char  file_type[100];
  int   species;
  char  bunch_patt_name[100];
} CBI_MEAS_INFO;



//--------------------------------------------------------------------
// CBI_ITYPE_SPECIFIC:
// Structure for holding pointers to instrument-type-specific
// arrays, values, and functions that are to be made available
// to this lower-level instrumentation library upon instrument-type
// setup.
//--------------------------------------------------------------------
typedef struct {
  char          instrument_type[20];
  int           num_platform_commands; // [CBI_MAX_PLATFORMS]
  char         *instrument_routine_names;
  char         *instrument_error_names;
  int          num_instrument_error_names;  
  char         *tsetup_names;
  void         *tsetup_func_ptr;
  void         *species_func_ptr;
  void         *supp_info_func_ptr;
} CBI_ITYPE_SPECIFIC;



//-----------------------------------------------------------------------+
//  CBI_MANAGE:    Contains CBPM system management information           |
//                                                                       |
//    cfg_file     - configuration file                                  |
// output_file_index - Index of most recent data file written to disk    |
//    n_Modules    - number of instruments brought online                |
//    p_module     - array of pointers to defined instrument             |
// p_Module_temp   - Pointer to temporary general instrument struct      |
//    s_OrderList  - list of module indices in order of position around  |
//                   the ring                                            |
//    n_Active     - number of ACTIVE instruments                        |
//    UpdateList   - list of module indices to updateD                   |
//    AccessMode   - Mode type (single module or active instruments)D    |
// debug_verbosity - debug level of typeout/log messages                 |
//     ServerID    - ID value assigned to application instance's server  |
//                   mode operation.                                     |
//     RunningMode - ISOLATED, BACK-END, or MANAGER                      |
//-----------------------------------------------------------------------+
typedef struct {
  char                 cfg_file[CBI_MAX_FILENAME_LEN];
  int                  output_file_index;
  int                  n_Modules;
  CBI_CTL_MODULE      *p_Module[CBI_MAX_INSTRUMENTS];
  CBI_CTL_MODULE      *p_Module_temp;
  int                  n_Active;
  int                  UpdateList[CBI_MAX_INSTRUMENTS];
  int                  num_sequence_commands;
  int                  skip_comm_connect;
  CBI_COMMAND          loaded_command;
  CBI_COMMAND         *command_list;
  CBI_ITYPE_SPECIFIC   itype;
  int                  self_trigger_enable;
  CBI_MEAS_INFO        meas_info;
  int                  handshake_timeout_tries;
  int                  handshake_timeout_flag;
  int                  debug_verbosity;
  int                  maintenance_mode;
  int                  option_mode;
  int                  option_mode2;
  int                  option_mode3;
  int                  ServerID;
  int                  comm_method;
  int                  RunningMode;
  int                  num_mgmt_connections;
  int                  instance_ID;
  int                  inst_quant_offset;
  int                  mgmt_socket_IDs[CBI_MAX_BACKENDS];
  int                  phase_meas_bunch_index;
  CBI_APP_VERSION      version;
  enum  CBI_CMD_DISPATCH_METHOD cmd_dispatch_method;
  enum  CTL_INTERFACE_STATE     InterfaceState;
} CBI_MANAGE;



//-----------------------------------------------------------------------+
// APP_CONFIG:  Space set aside for local configuration information for  |
//              applications built upon this library                     |
//                                                                       |
// app_ID   : String used to associate an application instance with a    |
//            particular group of instruments.  Mapping is provided in   |
//            the instrument list file.                                  |
//                                                                       |
// bunpat_def_file : Full path and filename of a bunch pattern           |
//                   definition file to use.                             |
//                                                                       |
// data_dir : Path of directory to hold generated data files             |
//                                                                       |
// sA_LDR_dir : Series-alpha (SHARC) BPM pre-built LDR file directory.   |
//                                                                       |
// s1_LDR_dir : Series-1 (TSHARC) BPM pre-built LDR file directory.      |
//                                                                       |
// s2_LDR_dir : Series-2 (TSHARC) BPM pre-built LDR file directory.      |
//                                                                       |
// param_file : Path of file that holds operational parameters for       |
//              all desired instruments.                                 |
//                                                                       |
// instr_list_file : Path of file that holds the list of instruments to  |
//                   bring online when the control application starts.   |
//-----------------------------------------------------------------------+
#define MAX_DIRPATH_LENGTH   100

typedef struct {
  char  app_ID[50];
  int   CBPM_server_ID;   // FIXME: Temporary, until enter_server_mode can extract numerical portion of the above field.
  char  cbi_PT_EXE_file[MAX_DIRPATH_LENGTH];
  char  cbi_ST_EXE_file[MAX_DIRPATH_LENGTH];
  char  directories[NUM_INDEXED_DIRS][MAX_DIRPATH_LENGTH];
  char  param_file[MAX_DIRPATH_LENGTH];
  char  det_param_file[MAX_DIRPATH_LENGTH];
  char  inst_param_file[MAX_DIRPATH_LENGTH];
  char  bunpat_def_file[MAX_DIRPATH_LENGTH];
  char  system_timing_setup_label[CBI_MAX_STRING_LENGTH];
  int   system_timing_setup;
  int   default_gain_setting;
  int   comm_method;
  char  inst_alloc_file[100];
  char  currmon_control_node[CBI_MAX_NODE_CHARS];
  char  signed_bunch_current_node[CBI_MAX_NODE_CHARS];
  char  bunch_current_nodes[CBI_NUM_SPECIES][CBI_MAX_NODE_CHARS];
  char  current_sum_node[CBI_MAX_NODE_CHARS];
} CBI_APP_CONFIG;
  

typedef struct {
  int  backend_ID;
  int  local_iidx;
  char inst_name[10];
} SELECTION_INFO;





//----------------------------------------------------
// Managed config:
//   Used for holding manager-specific configuration
//   info.  Only used in two-tier mode.
//----------------------------------------------------
typedef struct {
  char            alloc_schemes[CBI_MAX_BACKENDS][20];
  char            alloc_names[CBI_MAX_BACKENDS][20];
  int             num_instruments[CBI_MAX_BACKENDS];
  SELECTION_INFO  assignments[CBI_MAX_INSTRUMENTS];
} CBI_MANAGER_CONFIG;





//----------------------------------------------------
// Struct to hold config-file parameter properties.
// Used when programatically reading parameters from 
// a file, such as in a loop.  
// See file_io/read_instrument_params.c for
// examples of usage.
//----------------------------------------------------
typedef struct {
  char   name[40];
  int    rows;
  int    cols;
  int    type;
  void   *pointer;
} PARAMETER_INFO;





#if defined (__cplusplus)
}
#endif


#endif // CBI_CTL_TYPES_H










