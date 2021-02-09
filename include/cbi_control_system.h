#ifndef CBI_CONTROL_SYSTEM_H
#define CBI_CONTROL_SYSTEM_H
//-----------------------------------------------------------------------+
// File         :  cbpm_control_system.h                                 |
//                                                                       |
// Description  :  This header file defines generic instrumentation      |
//                 system configuration data structures and declarations |
//                 (with 'extern' option) for global data structures     |
//                 needed for data acquisition system operation,         |
//                 the master array of customizable nested structures    |
//                 assigned to each instrument to be controlled in a     |
//                 given data acquisition session.                       |
//                                                                       |
// Author       :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+


//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


//-----------------------------------------------------------------------+
// The following header files are those required by this header file.    |
//-----------------------------------------------------------------------+
#include "cbi_core_includes.h"


#if defined (__cplusplus)
}
#endif


#endif // CBI_CONTROL_SYSTEM_H





//-----------------------------------------------------------------------+
// Declare the globals necessary for running the data acquisition system.|
// One routine is intended to set the flag CBPM_NO_EXTERN to set aside   |
// memory for this data.  All other routines will access them by default |
// as externs.                                                           |
//-----------------------------------------------------------------------+
#if defined (CBI_CO_NO_EXTERN)

/* CBI_CTL_MODULE      CTL_Module[CBI_MAX_INSTRUMENTS]; */
/* CBI_CTL_MODULE      CTL_Module_temp; */
/* CBI_MANAGE          CTL_System; */
/* CBI_APP_CONFIG      appconfig; */
/* CBI_MANAGER_CONFIG  mgrconfig; */

/* // This array holds the communication structure collections common to all */
/* // instrument types. */
//CBI_DATA          core_comm_structs[CBI_MAX_INSTRUMENTS];
//CBI_DATA          core_comm_structs_temp; 

#else

extern CBI_CTL_MODULE      CTL_Module[];
extern CBI_CTL_MODULE      CTL_Module_temp;
extern CBI_MANAGE          CTL_System;
extern CBI_APP_CONFIG      appconfig;
extern CBI_MANAGER_CONFIG  mgrconfig;

extern CBI_DATA  core_comm_structs[];     
extern CBI_DATA  core_comm_structs_temp;

#endif





