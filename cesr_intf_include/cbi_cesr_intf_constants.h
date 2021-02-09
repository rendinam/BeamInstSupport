#ifndef CBI_CESR_INTF_CONSTANTS_H
#define CBI_CESR_INTF_CONSTANTS_H
//-----------------------------------------------------------------------+
// File         :  cbi_cesr_intf_constants.h                             |
//                                                                       |
// Description  :  Defines constants and parameters which specify the    |
//                 allowed actions available through the instrumentation |
//                 software MPM interface to the CESR control system,    |
//                 status codes, command names, .                        |
//                                                                       |
// Author       :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+
#include "cbi_cesr_interface.h"


//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------|
#if defined (__cplusplus)
extern "C" {
#endif


//-----------------------------------------------------------------------+
// Special return values for the cbpm_request function                   |
//                                                                       |
// CBPM_REQUEST_SUCCESS    - SUCCESSFUL command completion               |
// CBPM_REQUEST_PROC_ERROR - One or more CBPM processes returns an error | 
// CBPM_LOCK_ERROR         - Error obtaining database semaphore lock or  |
//                           subsequent software lock                    |
// CBPM_SLOCK_ERROR        - Error obtaining software lock               |
// CBPM_BAD_PROC_ERROR     - Bad CBPM Process list                       |
// CBPM_INPUT_ERROR        - Error with inputs                           |
// CBPM_TIMEOUT_ERROR      - Timeout while waiting for CBPM response     |
// CBPM_MISC_ERROR         - Miscellaneous error                         |
//-----------------------------------------------------------------------+
   enum CTL_CBPM_RETURN { CBPM_REQUEST_SUCCESS,
                          CBPM_REQUEST_PROC_ERROR,
                          CBPM_LOCK_ERROR,
                          CBPM_SLOCK_ERROR,
                          CBPM_BAD_PROC_ERROR,
                          CBPM_INPUT_ERROR,
                          CBPM_TIMEOUT_ERROR,
                          CBPM_MISC_ERROR = 99
   };

//-----------------------------------------------------------------------+
// Command Definitions                                                   |
//                                                                       |
// Data acquisition commands:                                            |
// CBPM_EXIT_FLAG        - CBPM processes under CESR control system      |
//                         control should return to manual control       |
// CBPM_NOCMD_FLAG       - No command requested                          |
// CBPM_IDLE_BPM_FLAG    - IDLE all active modules                       |
// CBPM_ORBIT_FLAG       - Take single sample orbit data for the         |
//                         specified species/train/bunch                 |
// CBPM_PHASE_FLAG       - Take betatron phase data for the specified    |
//                         species/train/bunch                           |
// CBPM_HEP_DATA_FLAG    - Take continuous HEP data (uses the predefined |
//                         species/train/bunch information for each BPM) |
// CBPM_CONT_DATA_FLAG   - Take continuous data using the list of        |
//                         species/train/bunch values provided.          |
// CBPM_MULTI_BUNCH_FLAG - Take multi-sample orbit data for multiple     |
//                         bunches                                       |
// CBPM_TURNS_ORBIT_FLAG - Take single turn orbit data                   |
// CBPM_INJ_TRAJ_FLAG    - Take injected bunch trajectory data           |
// CBPM_PEDESTALS_FLAG   - Take pedestal data for all gain settings      |
//                         (should only be used in no-beam conditions)   |
// CBPM_SHAKER_DATA_FLAG - Take turn-by-turn data with beam shakers on   |
//                                                                       |
// Utility commands:                                                     |
// CBPM_WRITE_BTNS_FLAG  - Write CBPM Buttons file                       |
//                                                                       |
// Special commands to control the MPM mailbox interface                 |
// CBPM_COMMAND_DISABLE  - Disable command interface                     |
// CBPM_COMMAND_ENABLE   - Enable command interface                      |
//-----------------------------------------------------------------------+
#define CBPM_EXIT_FLAG          -1
#define CBPM_EXIT_NAME        "CBPM_EXIT"
#define CBPM_NOCMD_FLAG          0
#define CBPM_NOCMD_NAME       "CBPM_NO_COMMAND"
#define CBPM_IDLE_BPM_FLAG       1
#define CBPM_IDLE_BPM_NAME    "CBPM_IDLE"
#define CBPM_ORBIT_FLAG          2
#define CBPM_ORBIT_NAME       "CBPM_ORBIT"
#define CBPM_PHASE_FLAG          3
#define CBPM_PHASE_NAME       "CBPM_PHASE"
#define CBPM_HEP_DATA_FLAG       4
#define CBPM_HEP_DATA_NAME    "CBPM_HEP_DATA"
#define CBPM_CONT_DATA_FLAG      5
#define CBPM_CONT_DATA_NAME   "CBPM_CONT_DATA"
#define CBPM_MULTI_BUNCH_FLAG    6
#define CBPM_MULTI_BUNCH_NAME "CBPM_MULTI_BUNCH"
#define CBPM_TURNS_ORBIT_FLAG    7
#define CBPM_TURNS_ORBIT_NAME "CBPM_TURNS_ORBIT"
#define CBPM_INJ_TRAJ_FLAG       8
#define CBPM_INJ_TRAJ_NAME    "CBPM_INJ_TRAJ"
#define CBPM_PEDESTALS_FLAG      9
#define CBPM_PEDESTALS_NAME   "CBPM_PEDESTALS"
#define CBPM_SHAKER_DATA_FLAG   10
#define CBPM_SHAKER_DATA_NAME "CBPM_SHAKER_DATA"


#define CBPM_RAW_DATA_FLAG     20
#define CBPM_RAW_DATA_NAME     "CBPM_RAW_DATA"
#define CBPM_AUTO_TIME_FLAG    21
#define CBPM_AUTO_TIME_NAME    "CBPM_AUTO_TIME"


#define CBPM_WRITE_BTNS_FLAG   100
#define CBPM_WRITE_BTNS_NAME  "CBPM_WRITE_BUTTONS_FILE"

#define CBPM_RESET_INIT_FLAG   200
#define CBPM_RESET_INIT_NAME  "CBPM_RESET_AND_INIT"

#define CBPM_COMMAND_DISABLE       1000
#define CBPM_COMMAND_DISABLE_NAME "CBPM_DISABLE"

#define CBPM_COMMAND_ENABLE        1001
#define CBPM_COMMAND_ENABLE_NAME  "CBPM_ENABLE"

//-----------------------------------------------------------------------+
// Mode definitions -                                                    |
//                                                                       |
// CBPM_PROCESS_WAIT   - wait for command to return                      |
// CBPM_PROCESS_NOWAIT - send command and return immediately             |
//-----------------------------------------------------------------------+
   enum CBPM_WAIT_STATE { CBPM_PROCESS_WAIT,
                          CBPM_PROCESS_NOWAIT
   };

//-----------------------------------------------------------------------+
// Possible values in the command_done array entries                     |
//                                                                       |
// CBPM_CMD_START    - value to start server process commands            |
// CBPM_CMD_CLEAR    - the "cleared" value of a command_done word.  The  |
//                     cbpm_request routine sets this value for all cbpm |
//                     monitoring processes when an action is requested. |
// CBPM_CMD_WORKING  - monitoring process is working                     |
// CBPM_CMD_DONE     - action completed successfully                     |
// CBPM_CMD_ERROR    - action was UNSUCCESSFUL                           |
//-----------------------------------------------------------------------+
#define CBPM_CMD_START    -1
#define CBPM_CMD_CLEAR     0
#define CBPM_CMD_WORKING   1
#define CBPM_CMD_DONE      2
#define CBPM_CMD_ERROR     3
#define CBPM_CMD_DISABLED 99
#define CBPM_UNUSED_ARG   -1

//-----------------------------------------------------------------------+
// CSRBPM DSTAT and CSRBPM PSTAT return values - Indicates the status of |
// CSRBPM DDATA (also EDATA) information on a detector-by-detector basis |
// for orbit data.  Also used to indicate the status of CSRBPM PDATA     |
// information for phase data.                                           |
//                                                                       |
// CBPM_NO_DATA   - Data uninitialized                                   |
// CBPM_BAD_DATA  - Error in acquiring data for this detector            |
// CBPM_GOOD_DATA - Data is OK                                           |
// CBPM_OLD_DATA  - Data is stale                                        |
//-----------------------------------------------------------------------+
#define CBPM_NO_DATA   -2
#define CBPM_BAD_DATA   0
#define CBPM_GOOD_DATA  1
#define CBPM_OLD_DATA   3

//-----------------------------------------------------------------------+
// Definitions for input parameters                                      |
//-----------------------------------------------------------------------+
#define CESR_ELECTRON -1
#define CESR_POSITRON  1

//-----------------------------------------------------------------------+
// Specifications for the "CBPM CONTROL" CESR database node              |
//-----------------------------------------------------------------------+
#define CBPM_CONTROL_NODE    "CBPM CONTROL"
#define CBPM_CONTROL_NUM_ELE 100

//-----------------------------------------------------------------------+
// Software lock values -                                                |
//                                                                       |
// CESR_CMD_LOCK   -  interface is in use                                |
// CESR_CMD_NOLOCK -  interface is available for use                     |
//-----------------------------------------------------------------------+
   enum CBI_SOFT_LOCK_STATUS { CESR_CMD_NOLOCK, CESR_CMD_LOCK };
   enum CBI_LOCK_REQUEST { CBI_SET_LOCK, CBI_REFRESH_LOCK, CBI_RELEASE_LOCK };

//-----------------------------------------------------------------------+
// Interface dump flags -                                                |
//                                                                       |
// CTL_INTF_FULL_DUMP - Dump entire interface mailbox                    |
// CTL_INTF_CMD_DUMP  - Dump command information                         |
//-----------------------------------------------------------------------+
   enum CBI_INTF_DUMP_MODE { CBI_CTL_INTF_FULL_DUMP, CBI_CTL_INTF_CMD_DUMP };




//-----------------------------------------------------------------------+
// Structure to monitor the status of the interface [GLOBAL extent]      |
//                                                                       |
//-----------------------------------------------------------------------+
#if defined (CBI_CO_NO_EXTERN)
  //CTL_INTERFACE_MONITOR CTL_CBPM_Intf_Monitor;
  //CTL_INTERFACE_PARAMS  CTL_CBPM_Intf_Params;
#else
   extern CTL_INTERFACE_MONITOR CTL_CBPM_Intf_Monitor;
   extern CTL_INTERFACE_PARAMS  CTL_CBPM_Intf_Params;
#endif



//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif

#endif  // CTL_CBPM_INTF_CONSTANTS_H
