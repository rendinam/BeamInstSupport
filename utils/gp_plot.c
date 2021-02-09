/*!-----------------------------------------------------------------------+
* File         :  gp_plot.c                                               |
*                                                                         |
* Description  :  Generic plotter of data                                 |
*                 Assumes gnuplot is available in path.                   |
*                                                                         |
*                                                                         |
* Arguments    :  datfile - Pointer to string holding datafile name       |
*                 col_start - First column of data (start numbering at 1) |
*                 col_end - Last column of data                           |
*                 plot_title - String holding the desired plot title      |
*                 titles - Array of strings holding titles for each data  |
*                  series to plot.  (FIXME: add some error checking here) |
*                                                                         |
* Author       :  M.Rendina, M. Stedinger                                 |
*                                                                         |
*-------------------------------------------------------------------------+*/
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>

#include "cbi_core_includes.h"
#include "cbi_gp_plotting.h"

#define EXISTS(x) (!((x) - (x)))

int gp_plot( char *full_fname, int col_start, int col_end, PLOT_PARAMS pparams, char *plot_title, char titles[][CBI_MAX_STRING_LENGTH] ) {

  int  i=0;
  int  col;
  FILE *plotscript;
  char temp_name[20];
  char command[100];


  // Write gnuplot command file
  if ( CTL_System.RunningMode == CBI_BACKEND_MODE) {
    sprintf( temp_name, "plot.gp-%d", CTL_System.instance_ID );
    plotscript = fopen(temp_name, "w");
  } else {
    sprintf( temp_name, "plot.gp" );
    plotscript = fopen(temp_name, "w");
  }
  
  // Compose plot appearance commands
  fprintf(plotscript, "set xtics %d\n", pparams.major_ticks);
  fprintf(plotscript, "set grid\n\n");
  fprintf(plotscript, "set style data linespoints\n");
  fprintf(plotscript, "set title \"%s\"  \n", plot_title);

  //fprintf(plotscript, "set arrow from %f,-32768 to %f,32768 nohead lt 1 lw 1.2\n", supp_plotvals[0], supp_plotvals[0] );

  fprintf(plotscript, "plot '%s' ", full_fname);

  // Compose actual plot command
  int numseries = col_end - col_start;
  for (i=0; i < numseries; i++ ) {
    col = col_start + i + 1;
    if (i == 0) {  // First series
      if (col == col_end ) { // This is the first AND the last column to plot
	fprintf(plotscript, "every 1::4:3::3 using 1:%d title \"%s\"\n", col, titles[i] );
      } else {
	fprintf(plotscript, "every 1::4:3::3 using 1:%d title \"%s\", \\\n", col, titles[i] );
      }
      continue;
    }
    if (col == col_end ) {  // Last series
      fprintf(plotscript, "'' every 1::4:3::3 using 1:%d title \"%s\" \n", col, titles[i] );
    } else {
      fprintf(plotscript, "'' every 1::4:3::3 using 1:%d title \"%s\" ,\\\n", col, titles[i] );
    }
  }
  

  fprintf(plotscript, "pause -1\n");

  fclose(plotscript);



  // Generate the plot
  sprintf(command," xterm -geometry 22x1 -e gnuplot %s &", temp_name);// Allows multiple plots at the same time.
  system( command );                                   // Creates two windows 1 term/1 graphic for each, though.


  return F_SUCCESS;

}


//==============================================================================
//==============================================================================


/* #define GP_PLOT_MAX_ENGINES     10 */


/* Window WindowWithName(Display *dpy, Window top, char *name) { */

/*   Window *children, dummy; */
/*   unsigned int nchildren; */
/*   unsigned int i; */
/*   Window w = 0; */
/*   char *window_name; */
  
/*   if (XFetchName(dpy, top, &window_name) && !strcmp(window_name, name)) { */
/*     return (top); */
/*   } */
  
/*   if (!XQueryTree(dpy, top, &dummy, &dummy, &children, &nchildren)) { */
/*     return (0); */
/*   } */
  
/*   for (i = 0; i < nchildren; i++) { */
/*     w = WindowWithName(dpy, children[i], name); */
/*     if (w) { */
/*       break; */
/*     } */
/*   } */
/*   if (children) { */
/*     XFree((char *) children); */
/*   } */
/*   return (w); */
/* } */



/* // Command to use when spawning an instance of GNUplot */
/* //#define GNUPLOT  "gnuplot -persist" */
/* #define GNUPLOT  "gnuplot" */


/* int gp_plot( char *full_fname, int col_start, int col_end, char *plot_title, char titles[][CBI_MAX_STRING_LENGTH] ) { */

/*   int  i=0; */
/*   int  col; */
/*   FILE *plotscript; */
/*   char command[100]; */
  
/*   FILE *gp; */
/*   char temp_name[100] = "plot.gp"; */
/*   char cmd[100]; */
/*   char plotname[100]; */
/*   int  plot = 0; */
/*   char string[100]; */


/*   // Write gnuplot command file */
/*   //if ( CTL_System.RunningMode == CBI_BACKEND_MODE) { */
/*   //sprintf( temp_name, "plot.gp-%d", CTL_System.instance_ID ); */

/* /\*     plotscript = fopen(temp_name, "w"); *\/ */
/* /\*     if (plotscript == NULL) { *\/ */
/* /\*       perror("fopen: "); *\/ */
/* /\*     } *\/ */
/* /\*     //} else { *\/ */
/* /\*     //sprintf( temp_name, "plot.gp" ); *\/ */
/* /\*     //plotscript = fopen(temp_name, "w"); *\/ */
/* /\*     //} *\/ */
  
/* /\*   // Compose plot appearance commands *\/ */
/* /\*     errno = 0; *\/ */
/* /\*   fprintf(plotscript, "set xtics %d\n", pparams.major_ticks); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set grid\n\n"); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set style data linespoints\n"); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set title \"%s\"  \n", plot_title); *\/ */
/* /\*     perror("fopen: "); *\/ */

/* /\*   fprintf(plotscript, "set arrow from %f,305 to %f,675 nohead lt 1 lw 1.2\n", supp_plotvals[0], supp_plotvals[0] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,295 to %f,675 nohead lt 2 lw 1.2\n", supp_plotvals[1], supp_plotvals[1] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,285 to %f,675 nohead lt 3 lw 1.2\n", supp_plotvals[2], supp_plotvals[2] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,275 to %f,675 nohead lt 4 lw 1.2\n", supp_plotvals[3], supp_plotvals[3] ); *\/ */
/* /\*     perror("fopen: "); *\/ */

/* /\*   fprintf(plotscript, "set arrow from %f,305 to %f,675 nohead lt 1 lw 1.2\n", supp_plotvals[4], supp_plotvals[4] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,295 to %f,675 nohead lt 2 lw 1.2\n", supp_plotvals[5], supp_plotvals[5] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,285 to %f,675 nohead lt 3 lw 1.2\n", supp_plotvals[6], supp_plotvals[6] ); *\/ */
/* /\*     perror("fopen: "); *\/ */
/* /\*   fprintf(plotscript, "set arrow from %f,275 to %f,675 nohead lt 4 lw 1.2\n", supp_plotvals[7], supp_plotvals[7] ); *\/ */
/* /\*     perror("fopen: "); *\/ */



/* /\*   fprintf(plotscript, "plot '%s' ", full_fname); *\/ */
/* /\*     perror("fopen: "); *\/ */

/* /\*   // Compose actual plot command *\/ */
/* /\*   int numseries = col_end - col_start; *\/ */
/* /\*   for (i=0; i < numseries; i++ ) { *\/ */
/* /\*     col = col_start + i + 1; *\/ */
/* /\*     if (i == 0) {  // First series *\/ */
/* /\*       if (col == col_end ) { // This is the first AND the last column to plot *\/ */
/* /\* 	fprintf(plotscript, "using 1:%d title \"%s\"\n", col, titles[i] ); *\/ */
/* /\*       } else { *\/ */
/* /\* 	fprintf(plotscript, "using 1:%d title \"%s\", \\\n", col, titles[i] ); *\/ */
/* /\*       } *\/ */
/* /\*       continue; *\/ */
/* /\*     } *\/ */
/* /\*     if (col == col_end ) {  // Last series *\/ */
/* /\*       fprintf(plotscript, "'' using 1:%d title \"%s\" \n", col, titles[i] ); *\/ */
/* /\*     } else { *\/ */
/* /\*       fprintf(plotscript, "'' using 1:%d title \"%s\" ,\\\n", col, titles[i] ); *\/ */
/* /\*     } *\/ */
/* /\*   } *\/ */
  

/*   ////fprintf(plotscript, "pause -1\n"); */

/* /\*   fclose(plotscript); *\/ */
/* /\*     perror("fclose: "); *\/ */

/*   // Generate the plot - old method. */
/*   //sprintf(command," xterm -e gnuplot %s &", temp_name);// Allows multiple plots at the same time. */
/*   //system( command );                                   // Creates two windows 1 term/1 graphic for each, though. */


/*   // Generate the plot - new method. */
  
/*   printf("Trying to open %s\n", "/usr/bin/gnuplot"); */
/*   sprintf(cmd, "/usr/bin/gnuplot"); */

/*   plot = 0; */
/*   // 'Register' plot. */
/*   for (plot = 0; plot < GP_PLOT_MAX_ENGINES; plot++) { */
    
/*     if (gp_plots[plot].window_idx == -1) { // Free slot */
/*       gp = popen(cmd, "w"); */
/*       if (gp == NULL) { */
/* 	printf("Error opening pipe to GNUplot.\n"); */
/* 	exit(1); */
/*       } */
/*       printf("plot = %d\n", plot ); */
/*       printf("gp         = %d\n", gp); */
/*       gp_plots[plot].gp_engine = gp; */
/*       printf("engine ptr = %d\n", gp_plots[plot].gp_engine ); */
/*       gp_plots[plot].window_idx = plot; */
/*       break; */
/*     } */
/*   } */
    

/*   sprintf(plotname, "Plot-%d-%d", CTL_System.instance_ID, plot); */
  
/*   strcpy(string, ""); */
/*   sprintf(string, "set term x11 0 title \"%s\"\n\n", plotname); */
/*   if (fprintf(gp, "%s",string) < 0) { */
/*     perror("write set : "); */
/*   } */
  
/*   strcpy(string, ""); */
/*   sprintf(string, "load \"%s\"\n\n", temp_name); */
/*   errno = 0; */
/*   if (fprintf(gp, "%s", string) < 0) { */
/*     perror("load: "); */
/*   } */
  
/*   if (fflush(gp) != 0) { */
/*     perror("fflush: "); */
/*   } */
  


/*   // Plot spawned, note the index of this one for constructing alive query later. */

/*   // Later, in cleanup pass, determine whether a window exists for a known spawned */
/*   // instance.  If it does, leave it alone, if not, kill off the file descriptor at */
/*   // that time; it is no longer needed. */
/*   Display *dpy = XOpenDisplay(NULL); */
/*   int screen   = DefaultScreen(dpy); */
/*   Window top   = RootWindow(dpy, screen); */
/*   Window w = 0; */


/*   //Get a pointer to this new plot window. */
/*   //Make a couple attempts in case GNUplot takes its time spawning one. */
/*   int try; */
/*   for (try = 0; try < 5; try++) { */
/*     printf("Attempting to get window identifier for \"%s\"\n", plotname); */
/*     w = WindowWithName(dpy, top, plotname); */
/*     if (w == NULL) { */
/*       continue; */
/*     } else { */
/*       printf("Got window = %d \n", (int)w ); */
/*       gp_plots[plot].win = w; */
/*       break; */
/*     } */
/*   } */

/*   XCloseDisplay( dpy ); */

/*   return F_SUCCESS; */

/* } */




/* static XErrorHandler old_handler = (XErrorHandler) 0; */
/* int gotBadWindow = 0; */

/* static int BadWindow_ehandler( Display *dpy, XErrorEvent *error ) { */
  
/*   if (error->error_code == BadWindow) { */
/*     gotBadWindow = 1; */
/*     return 0; */
/*   } else { */
/*     fprintf(stderr, "thing\n"); */
/*     if (!old_handler) abort(); */
/*     return (*old_handler) (dpy, error); */
/*   } */

/* } */




/* // Checks to see if there are any plotting engines running that are not */
/* // displaying windows due to a user killing a plot window off manually. */
/* // If if finds any orphans, it dispatches them by closing their */
/* // file descriptor. */
/* int gp_plot_cleanup_engines( void ) { */

/*   Display *dpy = XOpenDisplay(NULL); */
/*   if (dpy == NULL) { */
/*     printf("Connection to display failed!\n"); */
/*     return CBI_F_FAILURE; */
/*   } */
/*   int screen   = DefaultScreen(dpy); */
/*   Window top   = RootWindow(dpy, screen); */
/*   Window w; */

/*   int t; */
/*   int plot; */
/*   char line[100]; */
/*   char command[100]; */
/*   char plotname[100]; */
/*   char *windowname = NULL; */
/*   char *nameptr = windowname; */
/*   FILE *wininfo; */
/*   char string[100]; */
/*   int des; */

/*   printf("Cleaning up plotting engines that no longer have windows...\n"); */

/*   XErrorHandler ehandler = 0; */
  
/*   for (plot = 0; plot < GP_PLOT_MAX_ENGINES; plot++) { */

/*     // Skip slots that have no plots associated. */
/*     if (gp_plots[plot].window_idx == -1) { */
/*       continue; */
/*     } */

/*     strcpy( plotname, "" ); */
/*     strcpy( command, "" ); */
/*     sprintf(plotname, "Plot-%d-%d", */
/* 	    CTL_System.instance_ID, */
/* 	    gp_plots[plot].window_idx ); */
/*     printf("\nPlotname : %s   ID %d \n", plotname, gp_plots[plot].win ); */

/*     old_handler = XSetErrorHandler( BadWindow_ehandler ); */
/*     //printf("Handler set\n"); */
/*     gotBadWindow = 0; */
/*     w = gp_plots[plot].win; */
/*     XFetchName(dpy, w, &windowname ); */

/*     printf(">>>%s<\n", windowname ); */

/*     if (gotBadWindow) { */
/*       printf("Killing off stale plotting engine for dismissed window.\n"); */
/*       printf("gp_engine ptr val = %d\n", gp_plots[plot].gp_engine ); */
/*       printf("plot              = %d\n", plot); */
/*       printf("window_idx        = %d\n", gp_plots[plot].window_idx ); */
/*       printf("win               = %d\n", gp_plots[plot].win ); */
/*       gp_plots[plot].window_idx = -1; */
/*       gp_plots[plot].win        = -1; */
/*       strcpy(string, ""); */
/*       //sprintf(string, " exit\n\n"); */
/*       errno = 0; */
/*       //printf("sending exit\n"); */
/*       //if (fprintf(gp_plots[plot].gp_engine, "%s", string) < 0) { */
/*       //	perror("fprintf: "); */
/*       //} */
/*       printf("closing...\n"); */
/*       des = fileno(gp_plots[plot].gp_engine); */
/*       close( des ); */
/*       //pclose( gp_plots[plot].gp_engine ); */
/*       gp_plots[plot].gp_engine  = (FILE *)NULL; */
/*       printf("after gp_engine close\n"); */
/*     } */
/*     printf("after badwindow test\n"); */
/*     old_handler = XSetErrorHandler( NULL ); */
     
/*   } //endFOR */

/*   //printf("before display close\n"); */
/*   //XCloseDisplay( dpy ); */
/*   //printf("after display close\n"); */

/*   return CBI_F_SUCCESS; */

/* } */




