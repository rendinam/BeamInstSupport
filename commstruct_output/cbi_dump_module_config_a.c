//------------------------------------------------------------------------
// File         :  cbi_dump_module_config_a.c
// Date Created :  Fri Sep 14 14:07:50 2012
//
// Description  : This file was automatically genrated by the BIParser.
//                It is not a critical component of the communication data
//                structure imaging mechanism, and as such may be edited to
//                adjust the specific formatting employed when dumping the
//                contents of a given data structure without having to 
//                re-run the BIParser.
//
//                Note: If this file is edited and the BIParser is re-run
//                      and its output installed, the changes made to this
//                      file may be lost.  Make a backup copy of this file
//                      if you edit it.
//------------------------------------------------------------------------

#include "cbi_core_includes.h"

int cbi_dump_module_config( FILE *fp, int iidx ) {

  fprintf(fp, " Communication Structure Dump for: cbi_module_config\n");
  int  v0,  v1,  v2,  v3,  v4,  v5,  v6;
  int sv0, sv1, sv2, sv3, sv4, sv5, sv6;

  fprintf(fp, "exe_type: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.exe_type);
  fprintf(fp, "exe_version: %f  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.exe_version);
  fprintf(fp, "ldr_name:\n");
  for (v0 = 0; v0 < 44; v0++) {
    fprintf(fp, "CHAR", CTL_System.p_Module[iidx]->core->cbi_module_config.ldr_name[v0] );
  }
  fprintf(fp, "\n");
  fprintf(fp, "build_timestamp: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.build_timestamp);
  fprintf(fp, "core_comm_struct_rev: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.core_comm_struct_rev);
  fprintf(fp, "platform_comm_struct_rev: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.platform_comm_struct_rev);
  fprintf(fp, "compiler_ver: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.compiler_ver);
  fprintf(fp, "lib_version: %f  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.lib_version);
  fprintf(fp, "hardware_ver: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.hardware_ver);
  fprintf(fp, "firmware_ver: %d  \n", CTL_System.p_Module[iidx]->core->cbi_module_config.firmware_ver);

  return CBI_F_SUCCESS;

}


