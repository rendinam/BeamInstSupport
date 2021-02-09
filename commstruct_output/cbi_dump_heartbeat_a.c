//------------------------------------------------------------------------
// File         :  cbi_dump_heartbeat_a.c
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

int cbi_dump_heartbeat( FILE *fp, int iidx ) {

  fprintf(fp, " Communication Structure Dump for: cbi_heartbeat\n");
  int  v0,  v1,  v2,  v3,  v4,  v5,  v6;
  int sv0, sv1, sv2, sv3, sv4, sv5, sv6;

  fprintf(fp, "heartbeat: %d  \n", CTL_System.p_Module[iidx]->core->cbi_heartbeat.heartbeat);
  fprintf(fp, "timing_integrity: %d  \n", CTL_System.p_Module[iidx]->core->cbi_heartbeat.timing_integrity);
  fprintf(fp, "turns_seen: %d  \n", CTL_System.p_Module[iidx]->core->cbi_heartbeat.turns_seen);

  return CBI_F_SUCCESS;

}


