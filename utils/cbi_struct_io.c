//-----------------------------------------------------------------------+
// File         :  cbi_struct_io.c                                       |
//                                                                       |
// Description  :  Placeholder utility function for reading/writing the  |
//                 contents of a communication structure.                |
//                                                                       |
// Arguments    :  int mode       - READ/WRITE mode                      |
//                 void* vctl_mod - void pointer to CBPM_CTL_MODULE      |
//                 struct                                                |
//                 int ctl_tag    - tag from STRUCT_TAGS enum            |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  Ben Rock                                              |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_struct_io(int mode, void *vctl_mod, int ctl_tag) {
   
   char *func_name = "cbi_dsp_struct_io";

   printf("Called cbi_struct_io() [placeholder].\n");

   return F_SUCCESS;
}
