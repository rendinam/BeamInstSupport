//------------------------------------------------------------------------
// File         :  cbi_dump_cmd_a.c
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

int cbi_dump_cmd( FILE *fp, int iidx ) {

  fprintf(fp, " Communication Structure Dump for: cbi_cmd\n");
  int  v0,  v1,  v2,  v3,  v4,  v5,  v6;
  int sv0, sv1, sv2, sv3, sv4, sv5, sv6;

  fprintf(fp, "cmd: %d  \n", CTL_System.p_Module[iidx]->core->cbi_cmd.cmd);
  fprintf(fp, "cmd_status: %d  \n", CTL_System.p_Module[iidx]->core->cbi_cmd.cmd_status);
  fprintf(fp, "error:\n");
  for (v0 = 0; v0 < CBI_MAX_ERROR_WORDS; v0++) {
    fprintf(fp, "%d  ", CTL_System.p_Module[iidx]->core->cbi_cmd.error[v0] );
  }
  fprintf(fp, "\n");
  fprintf(fp, "handshake: %d  \n", CTL_System.p_Module[iidx]->core->cbi_cmd.handshake);

  return CBI_F_SUCCESS;

}


