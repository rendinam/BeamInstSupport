//------------------------------------------------------------------------
// File         :  cbi_dump_stat_a.c
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

int cbi_dump_stat( FILE *fp, int iidx ) {

  fprintf(fp, " Communication Structure Dump for: cbi_stat\n");
  int  v0,  v1,  v2,  v3,  v4,  v5,  v6;
  int sv0, sv1, sv2, sv3, sv4, sv5, sv6;

  fprintf(fp, "state: %d  \n", CTL_System.p_Module[iidx]->core->cbi_stat.state);
  fprintf(fp, "status: %d  \n", CTL_System.p_Module[iidx]->core->cbi_stat.status);
  fprintf(fp, "num_levels: %d  \n", CTL_System.p_Module[iidx]->core->cbi_stat.num_levels);
  fprintf(fp, "trace:\n");
  for (v0 = 0; v0 < CBI_MAX_TRACE_LEVELS; v0++) {
    fprintf(fp, "%d  ", CTL_System.p_Module[iidx]->core->cbi_stat.trace[v0] );
  }
  fprintf(fp, "\n");

  return CBI_F_SUCCESS;

}


