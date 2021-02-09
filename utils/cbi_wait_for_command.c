/*!
*-----------------------------------------------------------------------+
* File         :  cbi_wait_for_command.c                                |
*                                                                       |
* Description  :  Waits for handshake to be completed between control   |
*                 program and instrument's DSP.                         |
*                                                                       |
* Arguments    :  Index of master control struct for the device         |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

//ACC-release includes
#include "CESR_platform.h"  // Platform macro definitions
#include "utilities.h"      //
#include "messages.h"       // Status and logging message capabilities

#include "cbi_core_includes.h"


#define MAX_SHAKESTAT_STRING_LENGTH      50
#define HANDSHAKE_CHECK_TRIES            45
#define MAX_HANDSHAKE_CHECK_INTERVAL      2


int cbi_wait_for_command(int iidx) {

  char *func_name = (char *)__FUNCTION__;

  int debug_level = CTL_System.debug_verbosity;

  int usleep_param = 50000;

  // Check for custom handshake wait value.
  // If none present, use the default defined above.
  int NUM_TRIES = HANDSHAKE_CHECK_TRIES;
  if (CTL_System.handshake_timeout_tries != 0) {
    NUM_TRIES = CTL_System.handshake_timeout_tries;
  }
  
  if (debug_level > CBI_DEBUG_LEVEL_4) {
    printf("WAITING FOR   %d   tries before giving up on completion handshake.\n", NUM_TRIES );
  }

  enum shake_stats {
    ILLEGAL_HANDSHAKE_0,
    CMD_NEW,
    CMD_WORKING,
    CMD_DONE,
    CMD_ERROR,
    NUM_CMD_HANDSHAKE_VALS
  };

  char shake_stat_names[20][MAX_SHAKESTAT_STRING_LENGTH] = { // FIXME: centralize
    "ILLEGAL_HANDSHAKE_0",
    "CMD_NEW",
    "CMD_WORKING",
    "CMD_DONE",
    "CMD_ERROR"
  };

  // Variable for holding the calendar time in seconds to gauge timeout status during
  // handshake check.
  time_t t_secs1, t_secs2;


  int handshake, retval = CBI_F_SUCCESS;
  int tries = 0;
  int k, i=0;

  // Wait here for the command to finish, either by success or error
  //handshake = CTL_System.p_Module[iidx]->core->cbi_cmd.handshake;

  // Spawn a timer thread to check for timeout condition
  int pt_retval;
  CTL_System.p_Module[iidx]->handshake_timeout == FALSE;

  printf("<%s> ", CTL_System.p_Module[iidx]->comm.ethernet.hostname);


  // Initial handshake comparison with communications check outside of 
  // loop for rapid success looping when all or most instruments have already
  // completed their handshake.
  k = cbi_gp_struct(READ, CTL_System.comm_method, CBI_CMD_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);

  if (k != CBI_F_SUCCESS) {
    sprintf(message, "Communications failure during handshake check for instrument %s !", CTL_System.p_Module[iidx]->comm.ethernet.hostname );
    log_message(S_ERROR, func_name, message);
    return CBI_F_COMM_ERROR;
  }                                                            
  handshake = CTL_System.p_Module[iidx]->core->cbi_cmd.handshake;


  while (handshake == CBI_CMD_NEW_HANDSHAKE || handshake == CBI_CMD_WORKING_HANDSHAKE) {

    // Get current time in seconds 
    time( &t_secs1 );
    i++;
    
    k = cbi_gp_struct(READ, CTL_System.comm_method, CBI_CMD_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);

    if (k != CBI_F_SUCCESS) {
      sprintf(message, "Communications failure during handshake check for instrument %s @ %s!", 
	      CTL_System.p_Module[iidx]->comm.ethernet.hostname,
	      CTL_System.p_Module[iidx]->det.location);
      log_message(S_ERROR, func_name, message);
      return CBI_F_COMM_ERROR;
    }

    handshake = CTL_System.p_Module[iidx]->core->cbi_cmd.handshake;

    printf(".");
    fflush(stdout);
    time( &t_secs2 );

    // If wall-clock time exceeds X seconds inbetween handshake checks,
    // there is a communication problem, don't bother waiting for the
    // rest of handshake check loop to complete, it will waste too much
    // time.  Return error code to allow instrument to be placed into
    // recovery mode.
    if (difftime(t_secs2, t_secs1) > MAX_HANDSHAKE_CHECK_INTERVAL) {
      printf("Communications timeout between handshake polls!\n");
      retval = CBI_F_COMM_ERROR;
      return retval;
    }


    // Progress spinner
    printf("-\b");
    fflush(stdout);
    usleep(usleep_param);
    printf("\\\b");
    fflush(stdout);
    usleep(usleep_param);
    printf("|\b");
    fflush(stdout);
    usleep(usleep_param);
    printf("/\b");
    fflush(stdout);
    usleep(usleep_param);
    printf("-\b");
    fflush(stdout);
    usleep(usleep_param);


    //-----------------------------------------------------------------------------------
    // If the number of handshake check tries (dots in progress bar)
    // exceeds the try limit, give up on this instrument. // FIXME: Change to wall-clock time.
    //  
    // Only the first instrument to time out will cause the full timeout wait period to
    // elapse.  Subsequent instruments will do the briefest check possible and fall 
    // through if not yet done with their command.
    //-----------------------------------------------------------------------------------
    if (  (i > NUM_TRIES)  ||  
	  ( (CTL_System.handshake_timeout_flag == TRUE) && 
	    (handshake == CBI_CMD_NEW_HANDSHAKE || handshake == CBI_CMD_WORKING_HANDSHAKE) )   ){

      sprintf(message, "Handshake timed out for instrument %s !", CTL_System.p_Module[iidx]->comm.ethernet.hostname );
      log_message(S_ERROR, func_name, message);
      CTL_System.p_Module[iidx]->core->cbi_cmd.handshake = CBI_CMD_TIMEOUT_HANDSHAKE;
      CTL_System.handshake_timeout_flag = TRUE;

      // Setup and send the IDLE command to force-break the instrument
      // out of a stuck loop.
      sprintf(message, "Attempting a CBI_IDLE_CMD to abort remote operation...\n");
      log_message(S_INFO, func_name, message);
      cbi_init_command( iidx, CBI_IDLE_CMD );
      cbi_execute_command( iidx );
      retval = CBI_F_HANDSHAKE_TIMEOUT;
      return retval;
    }

  } // endWHILE handshake


  if ( (handshake <= (NUM_CMD_HANDSHAKE_VALS-1)) && (handshake >= CBI_ILLEGAL_HANDSHAKE_0) ) {
    printf("  %s  (%d)\n", shake_stat_names[handshake], handshake );
    // Special case handling of a successfully returned handshake value signifying remote error.
    if (handshake == CBI_CMD_ERROR_HANDSHAKE) {
      retval = CBI_F_HANDSHAKE_ERROR;
    }
  } else {
    printf("  UNKNOWN HANDSHAKE VALUE (%d)!\n", handshake );
    retval = CBI_F_HANDSHAKE_ERROR;
  }


  // TESTING
  k = cbi_gp_struct(READ, CTL_System.comm_method, CBI_DEBUG_TAG, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);


  return retval;

}








