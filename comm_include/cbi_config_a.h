#ifndef CBI_CONFIG_A_H
#define CBI_CONFIG_A_H

//------------------------------------------------------------------------
//  D O     N O T     E D I T     T H I S     F I L E
//------------------------------------------------------------------------
// File         :  cbi_config_a.h
// Date Created :  Fri Sep 14 14:07:50 2012
//
// Description  :  This file was automatically generated by the BIParser.
//                 It defines the contents of the COMM_KEY_CONFIG data
//                 struct defs for each flagged structure from the
//                 BIParser input (.def) file.
//------------------------------------------------------------------------

#include <stdio.h>
//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


#include "cbi_conv_protos.h"
#include "cbi_core_inst_includes.h"
#include "cbi_types_a.h"
#include "cbi_tags_a.h"


//================================================================
#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_cmd_cfg = {
         "cbi_cmd",                                  // name
         CBI_CMD_TAG,                                // struct_tag
         1,                                          // exe_type_allowed
         7,                                          // pkt_size
         1,                                          // num_pkts
         4,                                          // num_vars
         CBI_CTL_READ_WRITE,                         // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_INT, CBI_INT, CBI_INT, CBI_INT},       // data types
         {1, 1, CBI_MAX_ERROR_WORDS, 1},             // data counts
         {1, 1, 1, 1},                               // element sizes
         {cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_cmd_cfg;
#endif


#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_stat_cfg = {
         "cbi_stat",                                 // name
         CBI_STAT_TAG,                               // struct_tag
         1,                                          // exe_type_allowed
         18,                                         // pkt_size
         1,                                          // num_pkts
         4,                                          // num_vars
         CBI_CTL_READ_ONLY,                          // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_INT, CBI_INT, CBI_INT, CBI_INT},       // data types
         {1, 1, 1, CBI_MAX_TRACE_LEVELS},            // data counts
         {1, 1, 1, 1},                               // element sizes
         {cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_stat_cfg;
#endif


#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_debug_cfg = {
         "cbi_debug",                                // name
         CBI_DEBUG_TAG,                              // struct_tag
         1,                                          // exe_type_allowed
         2,                                          // pkt_size
         661,                                        // num_pkts
         4,                                          // num_vars
         CBI_CTL_READ_WRITE,                         // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_INT, CBI_INT, CBI_INT, CBI_INT},       // data types
         {1, CBI_MAX_DEBUG_WORDS, CBI_MAX_DEBUG_WORDS, 1}, // data counts
         {1, 1, 1, 1},                               // element sizes
         {cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_debug_cfg;
#endif


#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_ident_cfg = {
         "cbi_ident",                                // name
         CBI_IDENT_TAG,                              // struct_tag
         1,                                          // exe_type_allowed
         18.0,                                       // pkt_size
         1.0,                                        // num_pkts
         6,                                          // num_vars
         CBI_CTL_READ_ONLY,                          // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_CHAR, CBI_CHAR, CBI_INT, CBI_INT, CBI_INT, CBI_INT}, // data types
         {16, 28, 1, 1, 1, 4},                       // data counts
         {0.25, 0.25, 1, 1, 1, 1},                   // element sizes
         {cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_ident_cfg;
#endif


#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_heartbeat_cfg = {
         "cbi_heartbeat",                            // name
         CBI_HEARTBEAT_TAG,                          // struct_tag
         1,                                          // exe_type_allowed
         3,                                          // pkt_size
         1,                                          // num_pkts
         3,                                          // num_vars
         CBI_CTL_READ_ONLY,                          // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_INT, CBI_INT, CBI_INT},                // data types
         {1, 1, 1},                                  // data counts
         {1, 1, 1},                                  // element sizes
         {cbi_int_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_heartbeat_cfg;
#endif


#if defined(CBI_NO_EXTERN)
      static const COMM_KEY_CONFIG cbi_module_config_cfg = {
         "cbi_module_config",                        // name
         CBI_MODULE_CONFIG_TAG,                      // struct_tag
         1,                                          // exe_type_allowed
         20.0,                                       // pkt_size
         1.0,                                        // num_pkts
         10,                                         // num_vars
         CBI_CTL_READ_ONLY,                          // protection
         FIXED_REC_LENGTH,                           // rec length flag
         {CBI_INT, CBI_FLOAT, CBI_CHAR, CBI_INT, CBI_INT, CBI_INT, CBI_INT, CBI_FLOAT, CBI_INT, CBI_INT}, // data types
         {1, 1, 44, 1, 1, 1, 1, 1, 1, 1},            // data counts
         {1, 1, 0.25, 1, 1, 1, 1, 1, 1, 1},          // element sizes
         {cbi_int_convert, cbi_float_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_int_convert, cbi_float_convert, cbi_int_convert, cbi_int_convert}, // conv func ptrs
         NULL,                                       // custom copy/check function
         cbi_struct_io                               // IO function
      };
#else
      extern const COMM_KEY_CONFIG cbi_module_config_cfg;
#endif


#if defined (__cplusplus)
}
#endif


#endif //..config_H