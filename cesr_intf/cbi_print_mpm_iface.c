/*!
*-------------------------------------------------------------------------+
* File         :  cbi_print_mpm_iface_contents.c                          |
*                                                                         |
* Description  :  Checks the external interface mailbox block to gather   |
*                 action parameters provided by an external application.  |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina / M. Palmer                                  |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_print_mpm_iface_contents( CTL_INTERFACE *p_interface ) {

  int i;

  printf("\n\n\n");
  printf("lock             : %d\n", p_interface->lock);
  printf("command          : %d\n", p_interface->command);
  printf("num_bunches      : %d\n", p_interface->num_bunches);

  printf("bunch_select     : \n");
  for (i = 0; i < CBI_MAX_CTL_BUNCH_LIST; i++ ) {
    if ( (i+1) % 5 == 0 ) {
      printf("| ");
    } else {
      printf("  ");
    }
  }
  printf("\n");
  for (i = 0; i < CBI_MAX_CTL_BUNCH_LIST; i++ ) {
    printf("%d ", p_interface->bunch_id[i] );
  }
  printf("\n");

  printf("num_samples      : %d\n", p_interface->num_samples);
  printf("num_server_procs : %d\n", p_interface->num_processes);


  printf("cbpm_proc_id     : ");
  for (i = 0; i < CBI_MAX_CTL_PROCESSES; i++ ) {
    printf("%d ", p_interface->process_id[i] );
  }
  printf("\n");

  printf("command done     : ");
  for (i = 0; i < CBI_MAX_CTL_PROCESSES; i++ ) {
    printf("%d ", p_interface->command_done[i] );
  }
  printf("\n");

  printf("heartbeat        : ");
  for (i = 0; i < CBI_MAX_CTL_PROCESSES; i++ ) {
    printf("%d ", p_interface->heartbeat[i] );
  }
  printf("\n");

  printf("file_type        : ");
  for (i = 0; i < CBI_MAX_CTL_PROCESSES; i++ ) {
    printf("%d ", p_interface->file_type[i] );
  }
  printf("\n");

  printf("file_id          : ");
  for (i = 0; i < CBI_MAX_CTL_PROCESSES; i++ ) {
    printf("%d ", p_interface->file_id[i] );
  }
  printf("\n");

  printf("num_turns        : %d\n", p_interface->num_turns);
  printf("num_shots        : %d\n", p_interface->num_shots);
  printf("turn_select      : %d\n", p_interface->turn_select);

  return F_SUCCESS;

}

