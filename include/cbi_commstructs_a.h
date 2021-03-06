
#ifndef CBI_INST_DECS_A_H
#define CBI_INST_DECS_A_H
//------------------------------------------------------------------------
//  D O     N O T     E D I T     T H I S     F I L E
//------------------------------------------------------------------------
// File         :  cbi_INST_decs_a.h
// Date Created :  Fri Sep 14 14:07:50 2012
//
// Description  : This file was automatically generated by the BIParser.
//                It contains communications data structure declarations
//                internal to the instrument
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif



// Declare all structures used for communicating with control system.
// Storage space will be allocated by the routine that defines 
// INST_NO_EXTERN.  All other routines will refer to that space.
// Anything that the control system can change must be declared "volatile"
// so that the compiler does not try to optimize it out.

#if defined (INST_NO_EXTERN)

//------------------------------------------------------------------------
// "#pragma align _LONG" instances below are for work-around of
// digital board FPGA bug (c. 9-Oct-2009) affecting CBInet structure
// transfers when structures happen to fall near a particular type of
// address rollover boundary.
//------------------------------------------------------------------------
   #pragma align _LONG
volatile CBI_CMD                         INST_cmd;
   #pragma align _LONG
         CBI_STAT                        INST_stat;
   #pragma align _LONG
volatile CBI_DEBUG                       INST_debug;
   #pragma align _LONG
         CBI_IDENT                       INST_ident;
   #pragma align _LONG
         CBI_HEARTBEAT                   INST_heartbeat;
   #pragma align _LONG
         CBI_MODULE_CONFIG               INST_module_config;


#else
   volatile extern CBI_CMD                         INST_cmd;
            extern CBI_STAT                        INST_stat;
   volatile extern CBI_DEBUG                       INST_debug;
            extern CBI_IDENT                       INST_ident;
            extern CBI_HEARTBEAT                   INST_heartbeat;
            extern CBI_MODULE_CONFIG               INST_module_config;


#endif //(..._NO_EXTERN)

#if defined (__cplusplus)
}
#endif



#endif // INST_..._H
