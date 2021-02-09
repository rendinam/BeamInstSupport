///-----------------------------------------------------------------------+
/// File         :  cbi_print_cmd_struct.c                                |
///                                                                       |
/// Description  :  Dumps the contents of the CBI_CMD struct to the       |
///                 console.                                              |
///                                                                       |
/// Arguments    :  Index of instrument's master struct.                  |
///                                                                       |
/// Author       :  M. Rendina                                            |
///-----------------------------------------------------------------------+

#include "cbi_core_includes.h"

#define MAX_SHAKESTAT_STRING_LENGTH     50

int cbi_print_cmd_struct(int iidx, FILE *fp) {

  char shake_stat[20][MAX_SHAKESTAT_STRING_LENGTH] = { // FIXME: centralize
    "ILLEGAL_HANDSHAKE_0",
    "CMD_NEW",
    "CMD_WORKING",
    "CMD_DONE",
    "CMD_ERROR"
  };


   printf("---------------------------------------------------\n");
   printf("         DSP_CMD Structure Contents\n");
   //printf("cmd: %d        (%s)\n",(*(CTL_System.p_Module[iidx]->core)).cbi_CO_cmd.cmd, 
   //	  cbi_PT_cmd_names[(*(CTL_System.p_Module[iidx]->core)).cbi_CO_cmd.cmd] );
   printf("cmd_status: %d (%s)\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.cmd_status, 
	  func_status[(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.cmd_status] );
   printf("error0: %d\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.error[0] );
   printf("error1: %d\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.error[1] );
   printf("error2: %d\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.error[2] );
   printf("error3: %d\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.error[3] );
   printf("handshake: %d  (%s)\n",(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.handshake, 
	  shake_stat[(*(CTL_System.p_Module[iidx]->core)).cbi_cmd.handshake ]);
   printf("---------------------------------------------------\n");

   return 1;

}
