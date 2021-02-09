//-----------------------------------------------------------------------+
// File         :  cbi_init_mpm_comms.c                                  |
//                                                                       |
// Description  :  Function to initialize communications channel to the  |
//                 MPMnet manager to facilitate MPM access.              |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// TODO:  Accept argument for MPMnet server type to use.                 |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_init_mpm_comms(void) {

  int chkerr;
  int retval = CBI_F_SUCCESS;

  char MPMnet_errors[16][27] = {
    "MNET_NO_ERROR",
    "MNET_BAD_RANGE",
    "MNET_BAD_NODE",
    "MNET_CANT_CONNECT",
    "MNET_CANT_AUTHENTICATE",
    "MNET_DATA_ERROR",
    "MNET_SERVER_NOT_SET",
    "MNET_PORT_NOT_SET",
    "MNET_NOT_INITIALIZED",
    "MNET_WRONG_CLIENT",
    "MNET_SEND_ERROR",
    "MNET_RECV_ERROR",
    "MNET_SERVER_LIMIT",
    "MNET_FILE_IO_ERROR",
    "MNET_PROCESS_ERROR",
    "MNET_PROCESS_NOT_FOUND"
  };

  printf("Attempting connection to MPMnet manager...\n");
  chkerr = Mnet_connect("BPM");
  printf("Mnet_connect call returned : %d\n", chkerr);

  if (chkerr != MPM_SUCCESS) {
      mneterror("Error connecting to manager.\n\n");
      /* If the call failed, find out why - e.g. too many servers already running,
       * could not connect to manager, etc */
      mneterror("MPMnet error code is %d (%s)\n\n", chkerr, MPMnet_errors[(int)fabs(chkerr)]);
      retval = CBI_F_FAILURE;
  }

  return retval;

}
