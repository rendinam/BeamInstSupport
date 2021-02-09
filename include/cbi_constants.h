#ifndef CBI_CONSTANTS_H
#define CBI_CONSTANTS_H
//-----------------------------------------------------------------------+
// File         :  cbi_constants.h                                       |
//                                                                       |
// Description  :  This header file defines a set of named system        |
//                 constants, enumerations, and macro code that is       |
//                 common or at least made available to ALL              |
//                 beam instrumentation hardware module and control      |
//                 system code.                                          |
//                                                                       |
// Author       :  M. Palmer / C. Strohman / M. Rendina                  |
//-----------------------------------------------------------------------+

#include "cbi_io_constants.h"

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


//------------------------------------------------------------------------+
// Mathematical Constants:                                                |
//------------------------------------------------------------------------+
#define PI_                  3.14159265358979


//----------------------------
// Useful boolean constants
// (Common, so guard against 
// prior definition)
//----------------------------
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Bunch species is used as an index into arrays
enum CBI_SPECIES_ID {
  POSITRONS,
  ELECTRONS,
  DUAL
};


enum COMM_METHODS {
  XBUS,
  ETHERNET,
  NUM_COMM_METHODS
};



enum DATAFILE_DIRS {
  PHASE_DIR,
  PROC_DIR,
  RAW_DIR,
  TSCAN_DIR,
  PARAMS_DIR,
  DET_CONSTS_DIR,
  INST_CONSTS_DIR,
  NUM_INDEXED_DIRS
};


  //#define CBI_MAX_STRING_LENGTH          100
#define CBI_MAX_ROUTINE_NAME_LENGTH     25
#define CBI_MAX_ROUTINE_ERROR_LENGTH     50
#define CBI_MAX_TSETUP_NAME_LENGTH      15


//------------------------------------------------------------------------+
// Digital Board Address Definitions                                      |
//------------------------------------------------------------------------+

#define CBI_MAX_PACKET_ADDRESSES      512


//------------------------------------------------------------------------+
// Data Transfer Parameters                                               |
//---------------------------                                             |
//  MX_READBACK_BUF   Currently defined to be the maximum size of one ADC |
//                    buffer area in a TigerSHARC-based 2nd-generation    |
//                    digital BPM instrument (CBPM-II).  This is the most |
//                    supported by any instruments now in use.            |
//------------------------------------------------------------------------+
#define CBI_MAX_READBACK_BUF    524287


#define CBI_SOCKET_CLOSED       -1


// Names of the timing system encoded word gate bit levels
enum CBI_GATE_STATE {
  CBI_GATE_DISABLE,
  CBI_GATE_ENABLE
};

#define CBI_GATE_DISABLE_CODE   0x0
#define CBI_GATE_ENABLE_CODE    0x2




// For local parameter structure imaging
#define  CBI_COMM_WIPE_BYTE_VALUE     0xFE
#define  CBI_COMM_WIPE_WORD_VALUE    (CBI_COMM_WIPE_BYTE_VALUE << 24) + (CBI_COMM_WIPE_BYTE_VALUE << 16) + (CBI_COMM_WIPE_BYTE_VALUE << 8) + (CBI_COMM_WIPE_BYTE_VALUE)




enum CBI_MANAGED_FILE_OPEN_MODES {
  CBI_SINGLE_FILE,
  CBI_MULTI_FILE
};


//------------------------------------------------------------------------+
// CBI_UNITIALIZED_PARAM          Primarily for timing parameters, this   |
//                                  value signifies that a timing value   |
//                                  has not yet been initialized or       |
//                                  determined experimentally.            |
//                                                                        |
// CBI_EMPTY_INPUT              Used to signify when the user does not    |
//                              provide a new value at an input parameter |
//                              prompt.                                   |
//------------------------------------------------------------------------+
#define CBI_UNINITIALIZED_PARAM      -1
#define CBI_EMPTY_INPUT              -999999


//------------------------------------------------------------------------+
// Command dispatching parameters:                                        |
//                                                                        |
// CBI_MAX_PLATFORMS                 The maximum number of independent    |
//                                   instrumentation platforms that can be|
//                                   supported.                           |
//------------------------------------------------------------------------+
#define CBI_MAX_PLATFORMS               4


//------------------------------------------------------------------------+
// Two-tier operation parameters                                          |
//  CBI_MAX_BACK_ENDS    Maximum number of concurrent managed back-end    |
//                       server instances supported.                      |
//                                                                        |
//  CBI_MAX_DATASIZE     Maximum size of a network transfer buffer used   |
//                       for communications between manager and back-end  |
//                       instances.                                       |
//                                                                        |
//  CBI_SELEC_SPEC_LENGTH   Maximum length of an instrument selection     |
//                          request string to be passed from manager to   |
//                          back-ends.                                    |
//------------------------------------------------------------------------+
#define CBI_MAX_BACK_ENDS               4
#define CBI_MAX_DATASIZE                300
#define CBI_SELEC_SPEC_LENGTH           20


//------------------------------------------------------------------------+
// Timing mode indices                                                    |
// Timing modes are instrument timing board clock frequency               |
// modes and determine minimum bunch spacing digigitization               |
// capability.                                                            |
//------------------------------------------------------------------------+
enum TIMING_MODES {
  m4ns,
  m14ns,
  CBI_MAX_TIMING_MODES
};



//------------------------------------------------------------------------+
// Hardware Trigger Masks                                                 |
//                                                                        |
//   A and B bitfields are provided in the phase/data portion of the      |
//   turns-marker burst on every turn of the central timing signal.       |
//------------------------------------------------------------------------+
#define TRIG_MASK_NONE                0x00      // No acquisition trigger
#define TRIG_MASK_A                   0x01      // 60Hz line-synchronous
#define TRIG_MASK_B                   0x02      // Injection-synchronous






//------------------------------------------------------------------------+
// Accelerator Parameters:                                                |
//--------------------------                                              |
// CBI_RF_FREQ          RF Frequency is nominally 500 MHz UNITS are GHz   |
//                      Times are thus in nanoseconds.                    |
//                                                                        |
// CBI_NUM_BUCKETS      Number of RF buckets per turn                     |
//                                                                        |
// CBI_MAX_BUNCHES      Maximum number of charge bunches addressable by   |
//                      instrumentation systems.                          |
//                                                                        |
// CBI_PED_SHIFT        Move this much from a bunch for ped delay         |
//                                                                        |
// BUNCHES_PER_TRAIN  Number of bunches per train in CESR                 |
//                                                                        |
// CBI_BUNCHES_PER_SPECIES  Number of bunches per species in CESR         |
//                                                                        |
// CBI_NUM_TRAINS           Number of trains in CESR                      |
//                                                                        |
// CBI_NUM_SPECIES          Number of particle species to consider        |
//                                                                        |
//------------------------------------------------------------------------+
#define CBI_FREQ_RF                 0.499765000 
#define CBI_BUCKET_LENGTH           (1.0/RF_FREQ)
#define CBI_NUM_BUCKETS             1281
#define CBI_MAX_BUNCHES             640
#define CBI_MAX_16BIT_PATTERN_REGS  40
#define CBI_PED_SHIFT               14.0 
#define CBI_BUNCHES_PER_TRAIN       5    
#define CBI_BUNCHES_PER_SPECIES     (NUM_TRAINS * BUNCHES_PER_TRAIN)
#define CBI_NUM_TRAINS              9    
#define CBI_NUM_SPECIES             2   


//------------------------------------------------------------------------+
// Data acquisition parameters
//
// MAX_DATAFILE_TYPES   The number of unique types of data files that may
//                      be written during the execution of a given action
//
//------------------------------------------------------------------------+
#define CBI_MAX_DATAFILE_TYPES   10

#define CBI_FILE_ID_NOT_SET      -99


//------------------------------------------------------------------------+
// Digital board FPGA v5.6-5.8 constants
//
// MPS_OFFSET   Offset to utilize 'multiprocessor space' to access things
//              inside the DSP memory space from outside.
//              i.e. The packet address table needs to have this offset
//              added to every address so that external accesses
//              (XBUS / CBInet) reference the correct memory location in
//              DSP internal memory.
//------------------------------------------------------------------------+
#define CBI_MPS_OFFSET           0x02000000 // 
                                        // to access things inside the DSP memory  
                                        // space from outside.
                                        // I.e. the packet address table needs to
                                        // add this offset to every address so that
                                        


//------------------------------------------------------------------------+
// XBUS Parameters:                                                       |
//-------------------                                                     |
// MX_PKT_DATA        Max data in a packet                                | 
//                                                                        |
// MX_NAME_CHAR       Max number of characters in element name (including |
//                    the terminating '\0')                               |
//                                                                        |
// MX_NODE_CHAR       Max number of characters in a node name (including  |
//                    the terminating '\0')                               |
//                                                                        |
// VMPM_FAILURE       Return status if a vuxxxx command fails             |
//------------------------------------------------------------------------+
#define CBI_MAX_PKT_DATA          256
#define CBI_MAX_NODE_CHARS        13
#define CBI_VMPM_FAILURE         -1
#define CBI_PKT_XFER_FAILURE     -1


//------------------------------------------------------------------------+
// Single (isolated), Manager, and Back-end mode flag constants.          |
//------------------------------------------------------------------------+
#define CBI_SINGLE_MODE           0
#define CBI_BACKEND_MODE          1
#define CBI_MANAGER_MODE          2
#define CBI_MGR_TESTING_MODE      3
#define CBI_MAX_BACKENDS         10


//------------------------------------------------------------------------+
// Constants governing socket communication:                              |
//--------------------                                                    |
// CBI_MAX_SOCKDAT_SIZE        Centralized definition for a convenient    |
//                             number of words available in a socket      |
//                             system send/receive buffer.                |
//------------------------------------------------------------------------+
#define CBI_MAX_SOCKDAT_SIZE    300


//------------------------------------------------------------------------+
// Control and status constants for injection software interfacing.       |
//                                      
//------------------------------------------------------------------------+
#define CBI_WINJ_HANDSHAKE             0x0A
#define CBI_CURRMON_HANDSHAKE          0x15

#define CBI_INJ_CONTROL_NODE           "CSR 2NS CCON"
#define CBI_CURRMON_SERVER_ID_ELE      1
#define CBI_CURRMON_HANDSHAKE_ELE      2
#define CBI_CURRMON_TIMING_MODE_ELE    3
#define CBI_CURRMON_SPECIES_ELE        4


//------------------------------------------------------------------------+
//  MPM Database Node Names and Element Numbers                           |
//
//------------------------------------------------------------------------+
#define CBI_POS_CURRMON_NODE           "POS 2NS MAMP"
#define CBI_ELE_CURRMON_NODE           "ELE 2NS MAMP"
#define CBI_SIGNED_CURR_NODE           "CSR 4NS MAMP"
#define CBI_CURRENT_SUM_NODE           "CSR CURRENTS"
#define CBI_POLARITY_NODE              "CSR POLARITY"
#define CBI_POLARITY_ELE               2
#define CBI_POWER_CYCLE_NODE           "CBPM PWR ALL"

//------------------------------------------------------------------------+
// Debugging and Testing Support:                                         |
//---------------------------------                                       |
// MX_TRACE_LEVELS     Max function call depth able to be stored for      |
//                     DSP exe runtime traceback functionality.           |
//                                                                        |
// MX_DEBUG_WORDS       was 127                                           |
//                                                                        |
// MX_DEBUG_BUFS                                                          |
//                                                                        |
// MX_USER_PARAMS      Number of user-defined integer parameters that can |
//                     get passed from the control system to the DSPs for |
//                     testing and special functions.                     |
//------------------------------------------------------------------------+
#define CBI_MAX_TRACE_LEVELS      15 
#define CBI_MAX_DEBUG_WORDS       660
#define CBI_MAX_DEBUG_BUFS        24
#define CBI_MAX_USER_PARAMS       5

#define CBI_MAX_ERROR_WORDS       4



#define CBI_MAX_FILENAME_LEN      250


//------------------------------------------------------------------------+
// Struct TAG Management:                                                 |
//-------------------------                                               |
// CBI_MX_CBI_STRUCT_TAGS                                                 |
//                                                                        |
// CBI_STD_DATA_TAG_OFFSET                                                |
//------------------------------------------------------------------------+
#define CBI_MAX_CBI_STRUCT_TAGS   1400
#define CBI_STD_DATA_TAG_OFFSET   0



//------------------------------------------------------------------------+
// Communications Parameters                                              |
//----------------------------                                            |
// CBI_FULL_STRUCT          Flag for signifying that a communications     |
//                      structure transfer routine is to transfer the     |
//                      entire contents of the structure in question.     |
//------------------------------------------------------------------------+
#define CBI_FULL_STRUCT          -1


//------------------------------------------------------------------------+
// DAQ Parameters                                                         |
//-----------------                                                       |
// CBI_MAX_INSTRUMENTS   Maximum number of modules with which it is       |
//                       possible to interact in a given session.         |
//                                                                        |
// CBI_PHASE_WORD_MASK   The bitmask to use when extracting the shaker    |
//                       phase value from the encoded word that           |
//                       accompanies each turns marker.                   |
//------------------------------------------------------------------------+
#define CBI_MAX_INSTRUMENTS       92
#define CBI_PHASE_WORD_MASK       0x000001FF



//------------------------------------------------------------------------+
// Number of files that instrumentation software will write to a given    |
// numbered filetype subdirectory before spilling over into a new         |
// directory.                                                             |
//------------------------------------------------------------------------+
#define CBI_FILES_PER_DIR      1000




//------------------------------------------------------------------------+
// Data File Types:                                                       |
// FIXME: Make second-stage move to platform-dependent library            |
//------------------------------------------------------------------------+
enum CBPM_DATA_FILE_TYPES {
  INVALID_DATAFILE_TYPE,
  TIME_SCAN,
  RAW,
  RAW_EXTENDED,
  CHAN_CAL,
  FIND_BUNCH,
  CHAN_CAL_ERROR,
  PROC,
  CALIBRATION_LOG,
  PHASE_TABLES,
  PARAMETERS
};




//------------------------------------------------------------------------+
// Macros to enable user-friendly command sequencing in control           |
// applications.  See documentation on proper use of these.               |
//   TODO: Document fully and provide examples
//   TODO: Add simple looping capability
//------------------------------------------------------------------------+
#define CBI_COMMAND_SEQUENCE_BREAK_REQ    -100

#define CBI_COMMAND_SEQUENCE_START(x)     static int CBI_COMMAND_SEQUENCE_STEP_COUNT = 0;                  \
                                          int cbi_command_sequence_retval;                                 \
                                          int CBI_COMMAND_SEQUENCE_NUM_STEPS = x;                          \
                                          switch (CBI_COMMAND_SEQUENCE_STEP_COUNT) {                       \
                                          case (0):                                                        \
                                            cbi_command_sequence_retval = CBI_COMMAND_SEQUENCE_NUM_STEPS;  \


#define CBI_COMMAND_SEQUENCE_STEP(x)      break;                                                 \
                                          case (x):                                              \
                                          cbi_load_command( NULL, (int)NULL, NULL, CBI_FALSE );  \


// Allow for skipping a step in a pre-defined command sequence
#define CBI_NEXT_SEQUENCE_STEP            CTL_System.loaded_command.prep_func = NULL;  \
                                          CTL_System.loaded_command.post_func = NULL;  \
                                          break;                                       \


// Allow for breaking out of a sequence entirely.
#define CBI_COMMAND_SEQUENCE_BREAK        CBI_COMMAND_SEQUENCE_STEP_COUNT = 0;         \
                                          return CBI_COMMAND_SEQUENCE_BREAK_REQ;       \


// Boilerplate code to signify the end of a sequence composition
#define CBI_COMMAND_SEQUENCE_END          break;                                                                    \
					  default:                                                                  \
                                            break;                                                                  \
				          }                                                                         \
                                          if (CBI_COMMAND_SEQUENCE_STEP_COUNT == CBI_COMMAND_SEQUENCE_NUM_STEPS) {  \
                                            CBI_COMMAND_SEQUENCE_STEP_COUNT = 0;                                    \
                                          } else {                                                                  \
                                            CBI_COMMAND_SEQUENCE_STEP_COUNT++;                                      \
                                          }                                                                         \
                                          return cbi_command_sequence_retval;                                       \


//------------------------------------------------------------------------+
// Macros to enable user-friendly input prompt generation                 |
//        TODO:  Usage notes here.                                        |
//                                                                        |
//------------------------------------------------------------------------+
#define CBI_INIT_PROMPTS(NUM_PROMPTS)   CBI_INPUT_PROMPT_DESCRIPTOR  prompts[NUM_PROMPTS];                            \
                                        static int last_vals[NUM_PROMPTS];                                            \
                                        static int first_time = CBI_TRUE;                                             \
                                        int cbi_number_of_prompts = NUM_PROMPTS;                                      \
                                        int cbi_init_count;                                                           \
                                        for (cbi_init_count = 0; cbi_init_count < NUM_PROMPTS; cbi_init_count++) {    \
                                          prompts[cbi_init_count].last_value       = &(last_vals[cbi_init_count]);    \
                                          prompts[cbi_init_count].prompt_text      = NULL;                            \
                                          prompts[cbi_init_count].supp_text_ptr    = NULL;                            \
                                          prompts[cbi_init_count].data_type        = (int)NULL;                       \
                                          prompts[cbi_init_count].valid_inputs     = NULL;                            \
                                          prompts[cbi_init_count].map_out          = NULL;                            \
                                          prompts[cbi_init_count].default_value    = (int)NULL;                       \
                                          prompts[cbi_init_count].last_val_default = (int)NULL;                       \
                                          prompts[cbi_init_count].destination      = NULL;                            \
                                          prompts[cbi_init_count].harvester        = NULL;                            \
                                          prompts[cbi_init_count].validator        = NULL;                            \
                                        }                                                                             \
                                        cbi_init_prompts(prompts, NUM_PROMPTS);                                       \
                                        if (first_time == CBI_TRUE) {                                                 \
                                          for (cbi_init_count = 0; cbi_init_count < NUM_PROMPTS; cbi_init_count++) {  \
                                            last_vals[cbi_init_count]     = CBI_EMPTY_INPUT;                          \
                                          }                                                                           \
                                          first_time = CBI_FALSE;                                                     \
                                        }                                                                             \


#define CBI_PROMPTER()                  cbi_prompter( cbi_number_of_prompts, prompts );



//------------------------------------------------------------------------+
// Universal commands                                                     |
//   Commands defined for all instrument types.                           |
//------------------------------------------------------------------------+
enum CBI_UNIVERSAL_COMMANDS {
  CBI_INVALID_CMD_0,
  CBI_IDLE_CMD,
  CBI_FIRST_PLATFORM_COMMAND
};



//------------------------------------------------------------------------+
// DSP_BOOLEAN      Specifies global True and False values                |
//------------------------------------------------------------------------+
enum DSP_BOOLEAN { 
  CBI_FALSE, 
  CBI_TRUE 
};


//------------------------------------------------------------------------+
// DATA_TYPE        Specifies data types for translation by the COMM_KEY  |
//                  infrastructure.                                       |
//------------------------------------------------------------------------+
enum DATA_TYPE { 
  CBI_INT, 
  CBI_UINT, 
  CBI_FLOAT,
  CBI_CHAR,
  CBI_STRUCT
};


//------------------------------------------------------------------------+
// The control system will set the handshake to CMD_NEW when it issues a  |
// new command. The dispatch routine will set it to CMD_WORKING when it   |
// sees that a new command has been issued. It will set it to CMD_DONE    |
// when the command is finished.                                          |
//------------------------------------------------------------------------+
enum XBUS_HANDSHAKE {
  CBI_ILLEGAL_HANDSHAKE_0,
  CBI_CMD_NEW_HANDSHAKE,
  CBI_CMD_WORKING_HANDSHAKE,
  CBI_CMD_DONE_HANDSHAKE,
  CBI_CMD_ERROR_HANDSHAKE,
  CBI_CMD_TIMEOUT_HANDSHAKE
};


//------------------------------------------------------------------------+
// Debug verbosity level specifications.  Higher values mean more verbose |
// output.                                                                |
//------------------------------------------------------------------------+
enum CBI_DEBUG_VERBOSITY {
  CBI_DEBUG_LEVEL_0,
  CBI_DEBUG_LEVEL_1,
  CBI_DEBUG_LEVEL_2,
  CBI_DEBUG_LEVEL_3,
  CBI_DEBUG_LEVEL_4,
  CBI_DEBUG_LEVEL_5,
  CBI_NUM_DEBUG_LEVELS
};


//---------------------------------------------------------------------------
// Enumeration of instrument list output formatting modes
//---------------------------------------------------------------------------
enum CBI_INST_LIST_FORMAT {
  CBI_LABELS,
  CBI_DIVIDER,
  CBI_VALUES
};


//---------------------------------------------------------------------------
// enumeration of instrumentation function success and failure codes
//---------------------------------------------------------------------------
// F_SUCCESS         - Function completed successfully
// F_SUCCESS_W_GATE  - Used by prep functions to signify that gate-enable 
//                     is required.
// F_SUCCESS_NO_GATE - Used by prep functions to signify that gate-enable  
//                     should not be used.
// F_WARNING         - Provisional success
// F_HANDSHAKE_ERROR - Command handshaking with instrument failed.
// F_FAILURE         - Function did not complete successfully
// F_COMM_ERROR      - Function encountered a communications error during 
//                     execution.
//----------------------------------------------------------------------------
enum CBI_FUNCTION_STATUS {
  CBI_F_SUCCESS,
  CBI_F_SUCCESS_W_GATE,
  CBI_F_SUCCESS_NO_GATE,
  CBI_F_WARNING,
  CBI_F_HANDSHAKE_TIMEOUT,
  CBI_F_HANDSHAKE_ERROR,
  CBI_F_FAILURE,
  CBI_F_COMM_ERROR,
  CBI_F_CHANGED_GAIN,
  CBI_NUM_FUNCTION_STATUS_VALS
};


// enumeration for R/W permission of control system to DSP structs
// Needed by COMM_KEY
enum CTL_PROTECTION {
  CBI_CTL_READ_ONLY,
  CBI_CTL_READ_WRITE,
  CBI_CTL_WRITE_ONLY
};


//enumerations for ped_cal_in.update_mode 
//and peak_find_in.update_mode
enum CBI_UPDATE_FLAG_ENUM {
  CBI_DONT_UPDATE,
  CBI_UPDATE
};


// Methods available to dispatch commands
enum CBI_CMD_DISPATCH_METHOD {
  CBI_LOCAL_CMD_DISPATCH,
  CBI_REMOTE_CMD_DISPATCH,
  CBI_SELF_CMD_DISPATCH,
  CBI_NUM_CMD_DISPATCH_METHODS
};


// All possible activity states in which an instrument can exist
enum CTL_ACTIVE_MODULE {
  CBI_EMPTY_SLOT = -1,
  CBI_INACTIVE,
  CBI_ACTIVE,
  CBI_MAINT_1,
  CBI_MAINT_2,
  CBI_MAINT_3,
  CBI_MAINT_4,
  CBI_UNAVAILABLE
};


// Instrument status flags:
// Either OK for no errors or problems during a command
// execution or data read-back, 
// or
// A particular problem was encountered of the type
// specified.
enum CBI_INST_STATUS {
  CBI_OK_STATUS,
  CBI_STRUCT_COPY_FAILURE,
  CBI_HANDSHAKE_FAILURE,
  CBI_POST_STAGE_FAILURE,
  CBI_COMM_FAILURE,
  CBI_MAX_INST_STATUS_VALS
};


#if defined (__cplusplus)
}
#endif

#endif // CBI_CONSTANTS
