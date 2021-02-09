/*!
*-------------------------------------------------------------------------+
* File         :  cbi_poke.c                                              |
*                                                                         |
* Description  :  Pokes a value into a memory location on the BPM via     |
*                 CBInet.                                                 |
*                 NOTE: This should use the CBInet client library, but    |
*                 instead it just composes a cbi_cmd command line and     |
*                 executes that program to perform the write since I know |
*                 that works, and it's faster than setting up all the     |
*                 proper calls and socket openings for proper CBInet      |
*                 library use.                                            |
*                 The 'cbi_cmd' program needs to be in the user's path!   |
*                                                                         |
* Arguments    :  Index of instrument in master management list (integer) |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"
#include "cbi_net_linux.h"


int cbi_poke( int iidx, int addr, int value) {
  
  int socketfd = 0;
  int num_words = 0;
  int data = 0;
  
  int debug_level = CTL_System.debug_verbosity;


  if( cbi_net_initialize_sockets() <= 0) {
    printf("cbi_net_init FAILED\n");
    cbi_net_close_socket(socketfd);
    cbi_net_net_close(socketfd);
    exit(-1);
  }

  socketfd = cbi_net_fdopen((char *)CTL_System.p_Module[iidx]->comm.ethernet.hostname);
  if (socketfd > 0) {
    if (debug_level > CBI_DEBUG_LEVEL_3) {
      printf("cbi_net: Target set to node %s with socket fd %d.\n", 
	     CTL_System.p_Module[iidx]->comm.ethernet.hostname, 
	     socketfd);
    }
  } else {
    printf("Bad socket\n");
    cbi_net_close_socket(socketfd);
    cbi_net_net_close(socketfd);
    exit(-1);
  }

  cbi_net_clr_error();

  int *valptr = &(value);
  int mcf_adr   = addr;
  int xfer_size = 4;
  int data_cnt  = 1;  // number of 'packets'
  char *error_buf;

  char valstring[20];

  sprintf(valstring, "%d", value);  // Change value into a string for passing to CBInet function
  num_words = cbi_net_wr_mem(socketfd, addr, data_cnt, xfer_size, valstring);

  if (num_words <= 0) {
    error_buf = cbi_net_get_error();
    printf("cbi_net: %s\n", error_buf);
    printf("cbi_net: Word count mismatch, wrote %d instead of %d!\n", num_words, data_cnt);
    cbi_net_close_socket(socketfd);
    cbi_net_net_close(socketfd);
    return -1;
  }

  // Close socket
  cbi_net_close_socket(socketfd);
  cbi_net_net_close(socketfd);
  return F_SUCCESS;


}
