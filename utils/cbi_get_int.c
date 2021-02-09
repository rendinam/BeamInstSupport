/*!
*-----------------------------------------------------------------------+
* File         :  cbi_get_int.c                                         |
*                                                                       |
* Description  :  Waits until user enters a keystroke and returns it    |
*                 as an integer or passes it along to a CBI back-end    |
*                 application as per the application's mode.            |
*                                                                       |
* Arguments    :  None                                                  |
*                                                                       |

* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define MAXDATASIZE   300


int cbi_get_int(void) {

  char text[20];
  int  number;
  char buf[MAXDATASIZE];
  memset( buf, 0, sizeof(buf) );
  int be;  // Back-end instance loop var

  fflush(stdout);

  if ( CTL_System.RunningMode == CBI_BACKEND_MODE ) {

    while (recv(CTL_System.mgmt_socket_IDs[0], buf, MAXDATASIZE, 0) != 0) {
      fflush(stdout);
      if ( sscanf(buf, "%d", &number) == 1 ) {
	return number;
      }
      return CBI_EMPTY_INPUT;
    }

  } else  {

    if ( fgets(text, sizeof text, stdin) ) {
      if ( sscanf(text, "%d", &number) == 1 ) {
	// If in MANAGER_MODE, pass data on normally, but also
	// send a copy to all listening back-ends.
	if ( CTL_System.RunningMode == CBI_MANAGER_MODE ) {
	  for (be = 0; be < CTL_System.num_mgmt_connections; be++) {
	    if(send(CTL_System.mgmt_socket_IDs[be], text, strlen(text), 0) == -1) {
	      perror("send() error");
	    }
	  }
	}
	return number;
      }
    }
    return CBI_EMPTY_INPUT;

  }

}



int cbi_get_int_local(void) {

  char text[20];
  int  number;

  if ( fgets(text, sizeof text, stdin) ) {
      if ( sscanf(text, "%d", &number) == 1 ) {
	return number;
      }
  }
  return CBI_EMPTY_INPUT;
}
