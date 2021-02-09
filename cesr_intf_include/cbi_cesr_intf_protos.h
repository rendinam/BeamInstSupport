#ifndef CBI_CESR_INTF_PROTOS_H
#define CBI_CESR_INTF_PROTOS_H
//-----------------------------------------------------------------------+
// File         :  cbi_cesr_intf_protos.h                                |
//                                                                       |
// Description  :  Defines parameters and structures for a beam          |
//                 instrumentation system MPM mailbox interface          |
//                                                                       |
// Author       :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+

#include "cbi_command.h"
#include "cbi_control_types.h"
#include "cbi_comm_struct.h"

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


//-----------------------------------------------------------------------+
// CBPM Interface Prototypes                                             |
//-----------------------------------------------------------------------+
  extern int cbi_cesr_interface(CBI_COMMAND cmd_list[], int num_commands, int num_comm_structs);

  extern int cbi_cesr_intf_set_defaults(void);
  extern int cbi_check_cesr_intf(CTL_INTERFACE *);
  extern int cbi_dump_cesr_intf(int, CTL_INTERFACE *);
  extern int cbi_lock_cesr_intf(int, int *);
  extern int cbi_register_cesr_intf(int);
  extern int cbi_reply_cesr_intf(int);
  extern int cbi_disable_cesr_intf(void);
  extern int cbi_enable_cesr_intf(void);


//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif


#endif  // CBI_CESR_INTF_PROTOS_H






