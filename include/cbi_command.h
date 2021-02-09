#ifndef CBI_COMMAND_H
#define CBI_COMMAND_H

#include "cbi_constants.h"

//-----------------------------------------------------------------------+
// File         :  cbi_command.h                                         |
//                                                                       |
// Description  :  Define the standardized data acquisition system       |
//                 command properties structure and supporting constants.|
//                                                                       |
// Authors      :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


#define NOT_AVAILABLE_VIA_INTERFACE       -99


//-----------------------------------------------------------------------+
// CBI_COMMAND_TYPE - enumeration of command types for beam              |
//                    instrumentation server programs                    |
//                                                                       |
//  SINGLE_TYPE   - A standard, non-sequence command                     |
//                                                                       |
//  SEQUENCE_TYPE - A meta-command that causes the invocation of a list  |
//                  of other, pre-defined commands.                      |
//                                                                       |
//  HIDDEN_TYPE   - For MPM interface command dispatching and other      |
//                  special cases.  Also allows definition of a commands |
//                  that will not show up in the menu of commands.       |
//                    Note: To be superceded by group masking            |
//                          functionality not yet implemented.           |
//-----------------------------------------------------------------------+
   enum CBI_COMMAND_TYPE {
      SINGLE_TYPE = 0,
      SEQUENCE_TYPE,
      HIDDEN_TYPE
   };


//-----------------------------------------------------------------------+
// CBI_COMMAND - Data structure typedef for storing information about    |
//               commands standardized for use with the CBI data         |
//               acquisition system.                                     |
//                                                                       |
// name      : The name of the command                                   |
//                                                                       |
// helptext  : Help text for the command                                 |
//                                                                       |
// command   : The unique ID of a DSP or SYSTEM level command.           |
//             If required, this is what gets passed to the instrument   |
//             to select the on-board action to take.                    |
//                                                                       |
// flag      : Flag value for the command as needed for request via a    |
//             server's MPM interface.                                   |
//                                                                       |
// type      : The type of the command (see CBI_COMMAND_TYPE)            |
//                                                                       |
//        -- See 'Command Flow Diagram' for detailed usage   --          |
//        -- information regarding function pointers defined --          |
//        -- here.                                           --          |
//                                                                       |
// prep_func : Function pointer to a command preparation function.  For  |
//             simple commands that do not require the participation of  |
//             an instrument set, this can comprise the entire command   |
//             definition.                                               |
//             a control module data structure by the server program.    |
//                                                                       |
// post_func : Function pointer to a function that will be run once for  |
//             each instrument that takes part in a given command.       |
//                                                                       |
// final_func: Follow-up function.  If specified, gets run a single      |
//             time.  Useful for final data concatenation, analysis,     |
//             or archiving.                                             |
//                                                                       |
// parallel_post : Boolean flag that instructs the dispatch sequencer to |
//                 use a multi-threaded approach to run many command     |
//                 _post() functions simltaneously to more efficiently   |
//                 take advantage of available network bandwidth.        |
//         NOTE: If a post function is to take advantage of this, it     |
//               must be thread-safe and should primarily deal with data |
//               readback via 'cbi_gp_struct' calls.  File I/O and CESR  |
//               database accesses (via MPMnet) should be deferred to a  |
//               _final() function.                                      |
//                                                                       |
// menu_mask : Bit-wise menu mask indicating what selection of menus     |
//             each command is associated with                           |
//                                                                       |
// menuID    : Menu ID (to be filled by the application) for each        |
//             command.  The menuID will be a fixed value for each       |
//             server instance.                                          |
//                                                                       |
// group_mask: Bit-wise group mask so that GUIs/menus can be organized   |
//             according to broad families of commands                   |
//-----------------------------------------------------------------------+
   typedef struct {
         char *name;
         char *helptext;
         int  command;
         int  flag;
         int  type;
         int (*prep_func) (void);
         int (*post_func) (int); 
         int (*final_func)(void);
         int  parallel_post;
         int  menu_mask;
         int  menuID;
         int  group_mask;
   } CBI_COMMAND;

#if defined (__cplusplus)
}
#endif



#endif // CBI_COMMAND_H


 
