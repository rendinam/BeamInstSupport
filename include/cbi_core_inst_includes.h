#ifndef CBI_CORE_INST_INCLUDES_H
#define CBI_CORE_INST_INCLUDES_H
//-----------------------------------------------------------------------+
// File         :  cbi_core_includes.h                                   |
//                                                                       |
// Description  :  Aggregates all common Beam Instrumentation Core       |
//                 header file references into one include file for easy |
//                 use.                                                  |
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


#include "cbi_constants.h"
#include "cbi_command_interface.h"

#include "cbi_types_a.h"
#include "cbi_commstructs_a.h"



#if defined (__cplusplus)
}
#endif

#endif //CBI_CORE_INST_INCLUDES_H
