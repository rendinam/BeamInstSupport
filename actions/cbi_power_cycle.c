//-----------------------------------------------------------------------+
// File         : cbi_power_cycle.c                                      |
//                                                                       |
// Description  : Cycles the power supply of an instrument.              |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"

#define POWER_DOWN_WAIT_SECS   4
#define POWER_UP_WAIT_SECS   7


int cbi_power_cycle_prep( void ) {

  char *func_name = (char *)__FUNCTION__;

  int status, iidx;
  int ele[2], power_cyc_elem;
  int PowerRelayEnabled;
  int zero = 0;


  printf("Turning off power to all active instruments...\n");
  fflush(stdout);

  for (iidx = 0; iidx < CTL_System.n_Modules; iidx++) {
    if (CTL_System.p_Module[iidx]->active == CBI_ACTIVE) {
    	
      power_cyc_elem          = CTL_System.p_Module[iidx]->comm.xbus.element;
      	PowerRelayEnabled = 1;

      // If this is a DAQ Lab test instrument, PowerRelay is not Enabled
      if (CTL_System.p_Module[iidx]->comm.xbus.adr_node[9] == 'T') {
	//printf("\'TST\' instrument.\n");
	PowerRelayEnabled = 0;
      }
    	
      ele[0] = ele[1] = power_cyc_elem;
      if (PowerRelayEnabled) {
      	status = vxputn_c(CBI_POWER_CYCLE_NODE, ele[0], ele[1], &zero );
      	if (status != MPM_SUCCESS) {
		printf("MPMnet write of 0 to address 0x(%x) for %s FAILED!\n",
	       	CBI_POWER_CYCLE_NODE,
	       	CTL_System.p_Module[iidx]->comm.ethernet.hostname );
      	}
      }
    }
  }

  printf("Waiting for %d seconds for power supply bleed-down...\n", POWER_DOWN_WAIT_SECS);
  sleep(POWER_DOWN_WAIT_SECS);
  printf("Turning on power supplies...\n");

  return CBI_F_SUCCESS;

}





int cbi_power_cycle_post( int iidx ) {

  char *func_name = (char *)__FUNCTION__;

  int status;
  int ele[2], power_cyc_elem;
  int  PowerRelayEnabled;
  int one = 1;

  power_cyc_elem          = CTL_System.p_Module[iidx]->comm.xbus.element;
  PowerRelayEnabled = 1;

  // If this is a DAQ Lab test instrument, PowerRelay is not Enabled
  if (CTL_System.p_Module[iidx]->comm.xbus.adr_node[9] == 'T') {
    //printf("\'TST\' instrument. \n");
    PowerRelayEnabled = 0;
  }

  ele[0] = ele[1] = power_cyc_elem;
  if (PowerRelayEnabled) {
	  status = vxputn_c(CBI_POWER_CYCLE_NODE, ele[0], ele[1], &one );
	  if (status != MPM_SUCCESS) {
	    printf("MPMnet write of 1 to address 0x(%x) for %s FAILED!\n",
		   CBI_POWER_CYCLE_NODE,
		   CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	  }
  }

  return CBI_F_SUCCESS;

}


int cbi_power_cycle_final( void ) {
	
  printf("Waiting for %d seconds for Cold Fire to boot back up...\n", POWER_UP_WAIT_SECS);
  sleep(POWER_UP_WAIT_SECS);	
	
  return CBI_F_SUCCESS;
}
