/*!
*-------------------------------------------------------------------------+
* File         :  print_online_devs.c                                     |
*                                                                         |
* Description  :  Print a listing of all online devices, their hostnames, |
*                 IP addresses, status information physical location and  |
*                 activity state.                                         |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_print_online_list( FILE *fp ) {
  
  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int j, k; 
  char active;
  int fpga_maj;
  int fpga_min;
  int fpga_id[4];
  int module_type;
  char location[12];

  char status[2][3] = {"--", "OK"};
  int h_status, t_status;

  char ldr_name[30] = "";

  int iidx = 0;
  
  int disp_idx;
  if (CTL_System.RunningMode == CBI_BACKEND_MODE) {
    disp_idx = CTL_System.inst_quant_offset + 1;
  } else {
    disp_idx = 1;
  }

  if (debug_level < CBI_DEBUG_LEVEL_1) {
      system("clear");
  }


  int (*fptr)()   = NULL;
  fptr = CTL_System.itype.supp_info_func_ptr;


  fprintf(fp, "      Location      Hostname    IP Address        Timing   XBUS Node        Module Type        FE-FPGAs     BUILD ID  \n");
  fprintf(fp, "               Active                           HB  |                  Ele.           DB-FPGA                         ");

  // Call any provided supplementary info printing function here in 'column label' mode.
  if (fptr != NULL) {
    (*fptr)(iidx, fp, CBI_LABELS);
  }
  fprintf(fp, "\n");

  fprintf(fp, "----------------------------------------------------------------------------------------------------------------------");

  // Call any provided supplementary info printing function here in 'formatting divider' mode.
  if (fptr != NULL) {
    (*fptr)(iidx, fp, CBI_DIVIDER);
  }
  fprintf(fp, "\n");


  while( strcmp( CTL_System.p_Module[iidx]->comm.ethernet.hostname, "") ) {


    if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE ) {
      active = '*';
    } else if( CTL_System.p_Module[iidx]->active == CBI_MAINT_1 ) {
      active = '1';
    } else if( CTL_System.p_Module[iidx]->active == CBI_MAINT_2 ) {
      active = '2';
    } else if( CTL_System.p_Module[iidx]->active == CBI_MAINT_3 ) {
      active = '3';
    } else if( CTL_System.p_Module[iidx]->active == CBI_MAINT_4 ) {
      active = '4';
    } else {
      active = ' ';
    }

    //location  snip out location part of detector name

    fpga_id[0]  = 0;
    fpga_id[1]  = 0;
    fpga_id[2]  = 0;
    fpga_id[3]  = 0;
    module_type = 0;
    fpga_maj    = 0;
    fpga_min    = 0;


    h_status = t_status = FALSE;

    if( CTL_System.p_Module[iidx]->active != CBI_MAINT_1 &&
	CTL_System.p_Module[iidx]->active != CBI_MAINT_2 &&
	CTL_System.p_Module[iidx]->active != CBI_MAINT_3 &&
	CTL_System.p_Module[iidx]->active != CBI_MAINT_4    ) {
      
      // Get Heartbeat status
      if ( cbi_check_heartbeat(iidx) == TRUE ) {
	h_status = TRUE;

	fpga_id[0]  = CTL_System.p_Module[iidx]->core->cbi_ident.fe_fpga_id[0];
	fpga_id[1]  = CTL_System.p_Module[iidx]->core->cbi_ident.fe_fpga_id[1];
	fpga_id[2]  = CTL_System.p_Module[iidx]->core->cbi_ident.fe_fpga_id[2];
	fpga_id[3]  = CTL_System.p_Module[iidx]->core->cbi_ident.fe_fpga_id[3];
	
	module_type = CTL_System.p_Module[iidx]->core->cbi_ident.module_type;
	
	fpga_maj = CTL_System.p_Module[iidx]->core->cbi_ident.fpga_maj;
	fpga_min = CTL_System.p_Module[iidx]->core->cbi_ident.fpga_min;
	
	// Only get timing integrity if heartbeat is OK.
	if ( cbi_check_timing_integrity(iidx) == TRUE ) {
	  t_status = TRUE;
	}

      }


      fprintf(fp, "%3d)   %-9s %c  %-9s  %-14s   %2s  %2s   %12s  %2d  %10s   %d.%d   %2d,%2d,%2d,%2d  %010d     ",
	      disp_idx,
	      CTL_System.p_Module[iidx]->det.location,
	      active,
	      CTL_System.p_Module[iidx]->comm.ethernet.hostname,
	      CTL_System.p_Module[iidx]->comm.ethernet.IP_address,
	      status[h_status],
	      status[t_status],
	      CTL_System.p_Module[iidx]->comm.xbus.pkt_node,
	      CTL_System.p_Module[iidx]->comm.xbus.element,
	      mod_types[module_type],
	      fpga_maj, fpga_min,
	      fpga_id[0], fpga_id[1], fpga_id[2], fpga_id[3],
	      CTL_System.p_Module[iidx]->core->cbi_module_config.build_timestamp );


      // Call any provided supplementary info printing function here in 'field values' mode
      if (fptr != NULL) {
	(*fptr)(iidx, fp, CBI_VALUES);
      }


      // End this line
      fprintf(fp, "\n");


    } else {

      fprintf(fp, "%3d)   %-9s %c  %-9s  %-14s   %2s  %2s   %12s   %2d \n",
	      disp_idx,
	      CTL_System.p_Module[iidx]->det.location,
	      active,
	      CTL_System.p_Module[iidx]->comm.ethernet.hostname,
	      CTL_System.p_Module[iidx]->comm.ethernet.IP_address,
	      status[h_status],
	      status[t_status],
	      CTL_System.p_Module[iidx]->comm.xbus.pkt_node,
	      CTL_System.p_Module[iidx]->comm.xbus.element   );


    } // endIF != CBI_MAINTENANCE
    fflush(stdout);



    /*
    fprintf(fp, "     %010d      %010d      %-14s \n", 
	    p_tsharc_ctl_mod->core->cbi_module_config.core_comm_struct_rev,
	    p_tsharc_ctl_mod->core->cbi_module_config.platform_comm_struct_rev,
	    p_tsharc_ctl_mod->core->cbi_module_config.ldr_name
	    );
    */

    disp_idx++;
    iidx++;
  }

  if (CTL_System.RunningMode == CBI_BACKEND_MODE) {
    printf("-end-\n");
  }

  return CBI_F_SUCCESS;
}

