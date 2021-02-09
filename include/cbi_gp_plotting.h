#ifndef CBI_GP_PLOTTING_H
#define CBI_GP_PLOTTING_H
/*!
*-----------------------------------------------------------------------+
* File         :  cbi_gp_plotting.h                                     |
*                                                                       |
* Description  :  Parameters for controlling the behavior and use of    |
*                 GNUPlot plotting routines.                            |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif

//-----------------------------------------
// Structure to hold collection of
// plotting parameters. 
// These are used to specify the 
// configuration and labelling of a plot.
//-----------------------------------------
typedef struct {
  int  col_start;
  int  col_end;
  int  major_ticks;
  char plot_title[CBI_MAX_STRING_LENGTH];
  char series_names[][CBI_MAX_STRING_LENGTH];
} PLOT_PARAMS;


#define MAX_PLOT_SERIES    10


#if defined (__cplusplus)
}
#endif


#endif //  CBI_GP_PLOTTING_H
