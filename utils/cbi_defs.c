//-----------------------------------------------------------------------+
// cbi_defs.c
//-----------------------------------------------------------------------+
#include "cbi_constants.h"
#include "cbi_control_types.h"

#include "cbi_cesr_interface.h"


CBI_CTL_MODULE      CTL_Module[CBI_MAX_INSTRUMENTS];
CBI_CTL_MODULE      CTL_Module_temp;
CBI_MANAGE          CTL_System;
CBI_APP_CONFIG      appconfig;
CBI_MANAGER_CONFIG  mgrconfig;

// This array holds the communication structure collections common to all
// instrument types.
CBI_DATA          core_comm_structs[CBI_MAX_INSTRUMENTS];  // was tsharc_global
CBI_DATA          core_comm_structs_temp;



//---------------------------------
// From cbi_cesr_intf_constants.h
//---------------------------------
CTL_INTERFACE_MONITOR CTL_CBPM_Intf_Monitor;
CTL_INTERFACE_PARAMS  CTL_CBPM_Intf_Params;


//---------------------------------
// From cbi_communication.h
//---------------------------------
int cbi_comm_typeout = NO;
int max_pkts_to_read = 128;


//---------------------------------
// From cbi_names.h
//---------------------------------
char comm_method_names[NUM_COMM_METHODS][10] = {
     "XBUS",
     "ETHERNET" };


char cbi_species_names[CBI_NUM_SPECIES+1][10] = {
     "POSITRONS",
     "ELECTRONS",
     "DUAL"       };


char cbi_return_code_names[CBI_NUM_FUNCTION_STATUS_VALS][30] = {
     "SUCCESS",
     "SUCCESS w/ GATE ENABLE",
     "SUCCESS NO GATE ENABLE",
     "WARNING",
     "HANDSHAKE TIMEOUT",
     "FAILURE",
     "COMMUNICATIONS ERROR",
     "CHANGED GAIN" };


 char func_status[20][CBI_MAX_STRING_LENGTH] = {
     "DSP_F_SUCCESS",
     "DSP_F_WARNING",
     "DSP_F_FAILURE",
     "DSP_F_CHANGED_GAIN",
     "none" };


 char mod_types[7][9] = {"N/A     ", 
			 "BSM     ", 
			 "BPM     ", 
			 "FLM     ", 
			 "FLMA    ", 
			 "ERL_BPM ",
			 "BPM W/FE"};


 char data_modes[12][CBI_MAX_STRING_LENGTH] = {
     "NONE",                      // 0
     "TIME SCAN",                 // 1
     "RAW DATA",                  // 2
     "RAW DATA - EXTENDED",       // 3
     "CHANNEL CALIBRATION",       // 4
     "BUNCH FIND",                // 5
     "CHANNEL CALIBRATION ERROR", // 6
     "PROCESSED DATA",            // 7
     "BPM CALIBRATION",           // 8
     "PHASE TABLES"               // 9
   };


