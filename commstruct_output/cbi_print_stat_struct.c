///-----------------------------------------------------------------------+
/// File         :  cbi_print_stat_struct.c                               |
///                                                                       |
/// Description  :  Dumps the contents of the CBI_STAT struct to the      |
///                 console.                                              |
///                                                                       |
/// Arguments    :  Index of instrument's master struct.                  |
///                                                                       |
/// Author       :  M. Rendina                                            |
///-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_print_stat_struct(int iidx, FILE *fp) {

  int ID, num_levels;
  fprintf(fp, "    DSP_STAT Structure Contents\n");
  fprintf(fp, "state      : %d\n", (*(CTL_System.p_Module[iidx]->core)).cbi_stat.state);
  fprintf(fp, "status     : %d\n", (*(CTL_System.p_Module[iidx]->core)).cbi_stat.status);
  fprintf(fp, "num_levels : %d\n", (*(CTL_System.p_Module[iidx]->core)).cbi_stat.num_levels);
  num_levels = ((*(CTL_System.p_Module[iidx]->core)).cbi_stat.num_levels);

  for (int i = 0; i < 15; i++) {
    ID = (*(CTL_System.p_Module[iidx]->core)).cbi_stat.trace[i];
    fprintf(fp, "trace[%d]  : %s ", i, CTL_System.itype.instrument_routine_names + (ID*CBI_MAX_ROUTINE_NAME_LENGTH) );
    if (i == (num_levels-1) ) {
      fprintf(fp, "<--\n");
    } else {
      fprintf(fp, "\n");
    }
  }

  return F_SUCCESS;

}

