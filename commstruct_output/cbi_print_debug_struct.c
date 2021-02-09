///-----------------------------------------------------------------------+
/// File         :  cbi_print_debug_struct.c                              |
///                                                                       |
/// Description  :  Dumps the contents of the CBI_DEBUG struct to the     |
///                 console.                                              |
///                                                                       |
/// Arguments    :  Index of the master control structure for the         |
///                 instrument to control.                                |
///                                                                       |
/// Author       :  M. Rendina                                            |
///-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_print_debug_struct(int iidx, FILE *fp) {

  int i, write_ptr;
  int     r1, d1;
  int i2, r2, d2;
  int i3, r3, d3;
  int routine, debug;
  fprintf(fp, "---------------------------------------------------\n");
  fprintf(fp, "    DSP_DEBUG Structure Contents\n");
  fprintf(fp, "write_ptr: %d\n", (*(CTL_System.p_Module[iidx]->core)).cbi_debug.write_ptr);
  write_ptr = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.write_ptr;

  int val_per_col = ceilf( CBI_MAX_DEBUG_WORDS/3 );

  fprintf(fp, "Entry   FID   Value\n");
  for (i=0; i < val_per_col; i++) { 
    r1 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.routine[i];
    d1 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.debug[i];

    r2 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.routine[i+val_per_col];
    d2 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.debug[i+val_per_col];
    i2 = i + val_per_col;

    r3 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.routine[i+(2*val_per_col)];
    d3 = (*(CTL_System.p_Module[iidx]->core)).cbi_debug.debug[i+(2*val_per_col)];
    i3 = i + (2*val_per_col);

    fprintf(fp, "%3d) %21s\t%6d\t\t%3d) %21s\t%6d\t\t%3d) %21s\t%6d",
	    i,
	    CTL_System.itype.instrument_routine_names + (r1*CBI_MAX_ROUTINE_NAME_LENGTH),
	    d1,
	    i2,
	    CTL_System.itype.instrument_routine_names + (r2*CBI_MAX_ROUTINE_NAME_LENGTH),
	    d2,
	    i3,
	    CTL_System.itype.instrument_routine_names + (r3*CBI_MAX_ROUTINE_NAME_LENGTH),
	    d3 );


    if (i == (write_ptr-1) ) {
      fprintf(fp, "<--\n");
    } else {
      fprintf(fp, "\n");
    }
  }

  return F_SUCCESS;

}

