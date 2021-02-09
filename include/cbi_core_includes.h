#ifndef CBI_CORE_INCLUDES_H
#define CBI_CORE_INCLUDES_H
//-----------------------------------------------------------------------+
// File         :  cbi_core_includes.h                                   |
//                                                                       |
// Description  :  Aggregates all core beam instrumentation header file  |
//                 references into one include file for easy use.        |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


// System Includes
//-------------------------------------
#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <math.h>     
#include <ctype.h>    
#include <signal.h>   
#include <sys/stat.h> 
#include <time.h>     
#include <pthread.h>
#include <unistd.h>
#include <errno.h>


// ACC-Release Includes
//-------------------------------------
#include "CESR_platform.h"  // Platform macro definitions
#include "utilities.h"      //
#include "messages.h"       // Status and logging message capabilities
#include "Mnet_client_c.h"  // MPMnet functionality


// Communication Support
//-------------------------------------
#include "cbi_net_common.h"
#include "cbi_net_client.h"


// File I/O Support
//-------------------------------------
#include "cbpmfio.h"
#include "faccess.h"


// Beam Instrumentation Core
//-------------------------------------
#include "cbi_core_inst_includes.h"

#include "cbi_comm_struct.h" 
#include "cbi_cesr_intf_constants.h"
#include "cbi_cesr_intf_protos.h"
#include "cbi_cesr_interface.h"
#include "cbi_control_types.h"   // Structure definitions used by control applications
#include "cbi_communication.h"   // Instrument communication code

#include "cbi_config_a.h"        // Communications structure configuration definitions
#include "cbi_protos_a.h"

#include "cbi_conv_protos.h"
#include "cbi_control_system.h"
#include "cbi_command.h"
#include "cbi_utils.h"
#include "cbi_names.h"
#include "cbi_protos.h"


#include "cbi_gp_plotting.h"



#if defined (__cplusplus)
}
#endif

#endif //CBI_CORE_INCLUDES_H
