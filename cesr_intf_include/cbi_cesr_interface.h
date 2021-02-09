#ifndef CBI_CESR_INTERFACE_H
#define CBI_CESR_INTERFACE_H

//-----------------------------------------------------------------------+
// File         :  cbi_cesr_interface.h                                  |
//                                                                       |
// Description  :  Defines parameters and structures for a beam          |
//                 instrumentation system MPM mailbox interface          |
//                                                                       |
// Author       :  M. Palmer - 4/2/2009                                  |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif

//-----------------------------------------------------------------------+
// Control Interface Parameters -                                        |
//-----------------------------------------------------------------------+
#define CBI_MAX_CTL_BUNCH_LIST       45
#define CBI_MAX_CTL_PROCESSES         5
#define CBI_CTL_IFACE_CLEAR_VALUE     0

//-----------------------------------------------------------------------+
// CTL_INTERFACE - structure which maps the entries in a standard beam   |
//                 instrumentation control mailbox in the CESR MPM       |
//                                                                       |
// lock               - software lock to be set by the program           |
//                      requesting an action                             |
// command            - command flag of action to be taken               |
// num_bunches        - number of bunches specified                      |
// bunch_id           - list of bunch IDs                                |
// num_processes      - number of server processes running under control |
//                      system mailbox control                           |
// process_id         - Process IDs (internally specified, this is       |
//                      NOT the operating system process ID)             |
// command_done       - array of command completion flags                |
// heartbeat          - array of heartbeat flags for each CBPM process.  |
//                      Values are set by each CBPM_monitor process and  |
//                      cleared by the CBPM_serv process.                |
// num_turns          - number of turns of data requested                |
// num_shots          - number of injected shots to average              |
//                                                                       |
// file_type          - file type of last file written                   |
// file_id            - file index of last file written                  |
//                                                                       |
// DEFINES -            Point to the first data index for each element   |
//                                                                       |
// CTL_INTF_ELE_USED Total number of elements used in MPM node           |
//-----------------------------------------------------------------------+
   typedef struct {
         int lock;
         int command;
         int num_bunches;
         int bunch_id [CBI_MAX_CTL_BUNCH_LIST];
         int num_samples;
         int num_processes;
         int process_id[CBI_MAX_CTL_PROCESSES];
         int command_done[CBI_MAX_CTL_PROCESSES];
         int heartbeat[CBI_MAX_CTL_PROCESSES];
         int file_type[CBI_MAX_CTL_PROCESSES];
         int file_id[CBI_MAX_CTL_PROCESSES];
         int num_turns;
         int num_shots;
         int turn_select;
   } CTL_INTERFACE;


#define ELE_LOCK          1
#define ELE_COMMAND       2
#define ELE_NUM_BUNCHES   3
#define ELE_BID           (ELE_NUM_BUNCHES   + 1)
#define ELE_NUM_SAMPLES   (ELE_BID           + CBI_MAX_CTL_BUNCH_LIST)
#define ELE_NUM_PROCESSES (ELE_NUM_SAMPLES   + 1)
#define ELE_PROCESS_ID    (ELE_NUM_PROCESSES + 1)
#define ELE_COMMAND_DONE  (ELE_PROCESS_ID    + CBI_MAX_CTL_PROCESSES)
#define ELE_HEARTBEAT     (ELE_COMMAND_DONE  + CBI_MAX_CTL_PROCESSES)
#define ELE_FILE_TYPE     (ELE_HEARTBEAT     + CBI_MAX_CTL_PROCESSES)
#define ELE_FILE_ID       (ELE_FILE_TYPE     + CBI_MAX_CTL_PROCESSES)
#define ELE_NUM_TURNS     (ELE_FILE_ID       + CBI_MAX_CTL_PROCESSES)
#define ELE_NUM_SHOTS     (ELE_NUM_TURNS     + 1)
#define ELE_TURN_SELECT   (ELE_NUM_SHOTS     + 1)

#define CTL_INTF_ELE_USED     ELE_TURN_SELECT



//-----------------------------------------------------------------------+
// Struct and enumeration to monitor the state and status of the control |
// system interface                                                      |
//-----------------------------------------------------------------------+
   enum CTL_INTERFACE_STATE {
      CBI_CTL_INTF_OFF,
      CBI_CTL_INTF_ENABLED,
      CBI_CTL_INTF_DISABLED
   };

   enum CTL_INTERFACE_ACTION {
      CBI_INTF_DISCONNECT,
      CBI_INTF_CONNECT
   };

   enum CTL_INTERFACE_STATUS {
      CBI_INTF_STATUS_UNKNOWN,
      CBI_INTF_STATUS_OK
   };

   enum CTL_FILE_STATUS {
      CBI_CTL_FILE_UNKNOWN,
      CBI_CTL_NEW_FILE,
      CBI_CTL_OLD_FILE
   };

   enum CTL_FILE_TYPES {
      EMPTY_FILE,
      CBPM_TBT_FILE,    // Turn-by-turn data
      CBPM_BUTTON_FILE, // Averaged data 
      CBPM_PHASE_FILE,  // Betatron phase tables
      CBPM_CONFIG_FILE
   };

   typedef struct {
         enum CTL_INTERFACE_STATE state;
         int status;
         int MailboxID;
         int ProcessID;
         int FileStatus;
         int FileType;
         int FileID;
   } CTL_INTERFACE_MONITOR;


//-----------------------------------------------------------------------+
// Struct to hold the input parameters from the control system interface |
//-----------------------------------------------------------------------+
   typedef struct {
         int num_bunches;
         int bunch_id [CBI_MAX_CTL_BUNCH_LIST];
         int num_samples;
         int num_turns;
         int num_shots;
         int trig_cnt;  // equivalent to turn_select in interface structure
         int species;
         int train;
         int bunch;
   } CTL_INTERFACE_PARAMS;     



//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif

#endif  // CBI_CESR_INTERFACE_H






