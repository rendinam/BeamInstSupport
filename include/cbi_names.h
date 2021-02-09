#ifndef CBI_NAMES_H
#define CBI_NAMES_H
//-----------------------------------------------------------------------+
// File         :  cbi_names.h                                           |
//                                                                       |
// Description  :  Strings associated with various instrumentation       |
//                 entities.                                             |
//                                                                       |
// Author       :                                                        |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif

//-----------------------------------------------------------------------+
// Required include files                                                |
//-----------------------------------------------------------------------+
#include "cbi_core_includes.h"


//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif

#endif  // CBI_NAMES_H


//------------------------------------------------------------------------


#define INSTRUMENT_TEST_NAME         "BENCH_TEST"


#if defined (CBI_CO_NO_EXTERN)


/* char comm_method_names[NUM_COMM_METHODS][10] = { */
/*      "XBUS", */
/*      "ETHERNET" }; */


/* char cbi_species_names[CBI_NUM_SPECIES+1][10] = { */
/*      "POSITRONS", */
/*      "ELECTRONS", */
/*      "DUAL"       }; */


/* char cbi_return_code_names[CBI_NUM_FUNCTION_STATUS_VALS][30] = { */
/*      "SUCCESS", */
/*      "SUCCESS w/ GATE ENABLE", */
/*      "SUCCESS NO GATE ENABLE", */
/*      "WARNING", */
/*      "HANDSHAKE TIMEOUT", */
/*      "FAILURE", */
/*      "COMMUNICATIONS ERROR", */
/*      "CHANGED GAIN" }; */


/*  char func_status[20][CBI_MAX_STRING_LENGTH] = { */
/*      "DSP_F_SUCCESS", */
/*      "DSP_F_WARNING", */
/*      "DSP_F_FAILURE", */
/*      "DSP_F_CHANGED_GAIN", */
/*      "none" }; */


/*  char mod_types[7][9] = {"N/A     ",  */
/* 			 "BSM     ",  */
/* 			 "BPM     ",  */
/* 			 "FLM     ",  */
/* 			 "FLMA    ",  */
/* 			 "ERL_BPM ", */
/* 			 "BPM W/FE"}; */


/*  char data_modes[12][CBI_MAX_STRING_LENGTH] = { */
/*      "NONE",                      // 0 */
/*      "TIME SCAN",                 // 1 */
/*      "RAW DATA",                  // 2 */
/*      "RAW DATA - EXTENDED",       // 3 */
/*      "CHANNEL CALIBRATION",       // 4 */
/*      "BUNCH FIND",                // 5 */
/*      "CHANNEL CALIBRATION ERROR", // 6 */
/*      "PROCESSED DATA",            // 7 */
/*      "BPM CALIBRATION",           // 8 */
/*      "PHASE TABLES"               // 9 */
/*    }; */


#else


   extern  char comm_method_names[NUM_COMM_METHODS][10];
   extern  char cbi_species_names[CBI_NUM_SPECIES+1][10];
   extern  char cbi_return_code_names[CBI_NUM_FUNCTION_STATUS_VALS][30];
   extern  char func_status[20][CBI_MAX_STRING_LENGTH];
   extern  char mod_types[7][9];
   extern  char data_modes[12][CBI_MAX_STRING_LENGTH];


#endif
