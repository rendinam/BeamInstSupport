/*!
*-----------------------------------------------------------------------+
* File         :  cbi_get_char.c                                        |
*                                                                       |
* Description  :  Waits until user enters a keystroke and returns it    |
*                 as a character or passes it along to a CBI back-end   |
*                 instance as per application mode.                     |
*                                                                       |
* Arguments    :  None                                                  |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


#define MAXDATASIZE   300


char cbi_get_char(void) {

  char text[20];
  char *newline;
  char buf[MAXDATASIZE];
  memset( buf, 0, sizeof(buf) );
  int be;  // Back-end instance loop var

  fflush(stdout);

  if ( CTL_System.RunningMode == CBI_BACKEND_MODE ) {

    while(recv(CTL_System.mgmt_socket_IDs[0], buf, MAXDATASIZE, 0) != 0) {
      fflush(stdout);
      newline = strchr(buf, '\n');
      if (newline != NULL ) {
	*newline = '\0';
      } 
      if ( strlen(buf) == 0 ) {
	return -1;
      }
      return buf[0];
    }


  } else {
    
    if ( fgets(text, sizeof text, stdin) != NULL ) {
      newline = strchr(text, '\n'); /* search for newline character */
      if ( newline != NULL ) {
	*newline = '\0'; /* overwrite trailing newline */
      }
    } 
    // Return indicator of request for default (empty input)
    if (strlen(text) == 0) {
      return -1;
    }
    // If in MANAGER_MODE, pass data on normally, but also
    // send a copy to all listening back-ends.
    if ( CTL_System.RunningMode == CBI_MANAGER_MODE ) {
      for (be = 0; be < CTL_System.num_mgmt_connections; be++) {
	if(send(CTL_System.mgmt_socket_IDs[be], text, strlen(text), 0) == -1) {
	  perror("send() error");
	}
      }
    }
    return text[0];
    
  }
  
  
}



//-----------------------------------------------------------------------
// Gets a character from terminal input for local application use only.
// Does not hook into two-tier operation.
//-----------------------------------------------------------------------
char cbi_get_char_local(void) {

  char text[20];
  char *newline;

  if ( fgets(text, sizeof text, stdin) != NULL ) {
    newline = strchr(text, '\n'); /* search for newline character */
    if ( newline != NULL ) {
      *newline = '\0'; /* overwrite trailing newline */
    }
  } 
  // Return indicator of request for default (empty input)
  if (strlen(text) == 0) {
    return -1;
  }

  return text[0];
  
}


