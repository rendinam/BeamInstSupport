/*!
*-----------------------------------------------------------------------+
* File         :  cbi_manager_input_handler.c                           |
*                                                                       |
* Description  :  Responsible for displaying the menu of commands,      |
*                 maintaining synchronization with the back-end         |
*                 processes, and multiplexing input to them.            |
*           // FIXME: Generalize to max supported back-ends.            |
*                                                                       |
* Arguments    :  None                                                  |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

// max number of bytes we can get at once
#define MAXDATASIZE     300

int server_mode_active = FALSE;
int server_mode_break  = FALSE;



int cbi_exit_server_mode( int *keepalive_needed ) {

  int input = CBI_EMPTY_INPUT;

  while (TRUE) {
    printf("Enter 'q' to exit server mode: ");
    input = cbi_get_char_local();
    if ((char)input == 'q') {
      server_mode_break   = TRUE;
      *(keepalive_needed) = FALSE;
      break;
    }
  }

  return CBI_F_SUCCESS;
}



int cbi_manager_input_handler( CBI_COMMAND commands[], int num_cmds ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  char buf[256]; // buffer for client data
  int nbytes;
  int connections = 2;

  int input_val = CBI_EMPTY_INPUT;
  int input = CBI_EMPTY_INPUT;
  char sinput[MAXDATASIZE] = {""};


  // Array of nulls used to wiping MPM server control mailbox arrays.
  int i, vals[100];
  for (i = 0; i < 100; i++) {
    vals[i] = 0;
  }


  char intext[20];
  int prompt_input = FALSE;
  
  char err[20];


  struct  timeval timeout;  // Timeout struct for controlling select()
  int     sret;
  fd_set  socks;       // temp file descriptor list for select()
  int     fdmax;       // maximum file descriptor number
  int     highsock = CTL_System.mgmt_socket_IDs[connections];
  int     cxn;

  char bufs[2][MAXDATASIZE];
  int receipts = 0;

  pthread_t  KA_thread, INPUT_thread;


  for(;;) {
    
    input_val = CBI_EMPTY_INPUT;


    // Checks to be performed once per manager-->back-ends polling loop
    if (server_mode_break == TRUE) {
      
      server_mode_break = FALSE;
      // Terminate and clean up server mode keyboard break thread. 
      if (debug_level > CBI_DEBUG_LEVEL_1) {
	printf("Joining input thread...\n");
      }
      pthread_join( INPUT_thread, NULL );
      // Terminate and clean up MPMnet 'keepalive' loop thread.
      if (debug_level > CBI_DEBUG_LEVEL_1) {
	printf("Joining keepalive thread...\n");
      }
      pthread_join( KA_thread, NULL );
      
      printf("Received local server mode break.  Instructing remote\n");
      printf("programs to break out of server mode VIA MPM mailbox wipes...  ");
      int val = -1;
      vxputn_c(CBPM_CONTROL_NODE, 2, 2, &val);
      cbi_sleep(400);
      val = 0;
      
      vxputn_c(CBPM_CONTROL_NODE, 1, 100, &vals[0]);
      cbi_sleep(500);
      printf(" done.\n");
    } //endIF server_mode_break


    FD_ZERO(&socks);

    // Prior to every time select gets called, the socket collection
    // must be set.
    for (cxn = 0; cxn < connections; cxn++) {
      FD_SET( CTL_System.mgmt_socket_IDs[cxn], &socks );
    }

    strcpy(buf, "");
	
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    sret = select(sizeof(socks)*8, &socks, NULL, NULL, &timeout);

    if (sret < 0) {
      perror("select");
      exit(1);
    }
    if (sret == 0) {
      // Noting ready to read, just show that this is still alive.
      ////printf("-");
      fflush(stdout);
    } else {
      
      
      for (cxn = 0; cxn < connections; cxn++) {

	if ( FD_ISSET(CTL_System.mgmt_socket_IDs[cxn], &socks) ) {
	  ////printf("recv ");
	  if ((nbytes = recv(CTL_System.mgmt_socket_IDs[cxn], buf, sizeof(buf), 0)) <= 0) {
	    // got connection closed or error from client
	    if (nbytes == 0) {
	      printf("socket %d hung up\n", CTL_System.mgmt_socket_IDs[cxn]);
	      printf("Quitting...\n");
	      exit(0);
	    } else {	    
	      sprintf(err, "[%d]recv", cxn );
	      perror(err);
	      continue;
	      //break; //new
	    }
	    close(CTL_System.mgmt_socket_IDs[cxn]); 
	    
	  } else { 
	    if (debug_level > CBI_DEBUG_LEVEL_1) {
	      printf("[%d] Buf received = \"%s\"\n", cxn+1, buf);
	    }
	    receipts++;
	    strcpy(bufs[cxn], "");
	    strcpy(bufs[cxn], buf);
	  }
	} //endIF ISSET
	
      } //endFOR connections
      

      // Check that all requests have been received on all connections and that
      // they all match, ensuring synchronization between manager and back-ends.
      if (receipts == connections) {
	if (debug_level > CBI_DEBUG_LEVEL_1) {
	  printf("Received status from all back-ends.\n");
	}
	receipts = 0;
      } else {
	// Go back up top and wait for more responses.
	continue;
      }



      //-----------------------
      // Handle MENU requests
      //-----------------------
      if ( strcmp(buf, "MENU") == 0 ) {
	for (cxn = 0; cxn < connections; cxn++) {
	  memset(buf, 0, sizeof(buf) );
	  sprintf(buf, "GO\0");
	  printf("-----> Sending GO comand to back-end #%d\n", cxn);
	  fflush(stdout);
	  if(send(CTL_System.mgmt_socket_IDs[cxn], buf, strlen(buf)+1, 0) == -1) {
	    perror("send() error");
	  }
	}
	while (input_val == CBI_EMPTY_INPUT) {
	  input_val = cbi_query_action( commands, num_cmds );
	}
      }
	  


      //-------------------------
      // Handle PROMPT requests
      //-------------------------
      if ( strstr(buf, "PROMPT") != NULL ) {
	printf("%s", &(buf[6]) );
	fgets(intext, sizeof intext, stdin);
	fflush(stdout);
	strcat(intext, "\0");
	for (cxn = 0; cxn < connections; cxn++) {
	  if (debug_level > CBI_DEBUG_LEVEL_3) {
	    printf("Sending prompt response to back-end %d\n", cxn+1);
	  }
	  if(send(CTL_System.mgmt_socket_IDs[cxn], intext, strlen(intext)+1, 0) == -1) {
	    perror("send() error");
	  }
	}
      }



      //---------------------------
      // Handle FILENAME requests
      //---------------------------
      char name_info[MAXDATASIZE] = {""};
      char xfer[MAXDATASIZE]      = {""};
      if ( strcmp(buf, "FILENAME?") == 0 ) {
	printf("\nAll back-ends ready to write file...\n\n");
	for (cxn = 0; cxn < connections; cxn++) {
	  if(send(CTL_System.mgmt_socket_IDs[cxn], name_info, MAXDATASIZE, 0) == -1) {
	    perror("File_ID send() error");
	    return CBI_F_FAILURE;
	  }
	  printf("Sent filename: \"%s\"\n", name_info);
	  
	  // Receive (new or echoed) full filename
	  printf("Waiting for return of full filename...\n");
	  recv(CTL_System.mgmt_socket_IDs[cxn], name_info, MAXDATASIZE, 0);
	  printf("received: \"%s\"\n", name_info);
	  
	  // Wait for back-end to signal file write completion
	  // TODO: Allow for reasonable timeout.  If that occurs,
	  //       ensure sensible failover to next back-end with
	  //       appropriate error notification.  If the first
	  //       back-end fails, the failover will need to ensure
	  //       the file header information gets written as well.
	  strcpy( xfer, "" );
	  recv(CTL_System.mgmt_socket_IDs[cxn], xfer, MAXDATASIZE, 0);
	  printf("Back-end %d is done writing the file  [%s]\n", cxn+1, xfer);
	}

	if (server_mode_active) {
	  printf("\nEnter 'q' to exit server mode: ");
	}

      }



      //---------------------------------
      // Handle PARAMETER SAVE requests
      //---------------------------------
      if ( strcmp(buf, "PARAM_FILENAME?") == 0 ) {
	printf("\nAll back-ends ready to write intermediate parameter file...\n\n");
	char input = 'n';		
	printf("\n\nWould you like to save the timing parameters to an intermediate file? (y/n): ");
	input = cbi_get_char_local();

	if (input == 'n' || input == 'N') {
	  strcpy( xfer, "" );
	  strcpy( xfer, "ABORT\0" );
	  printf("Timing values not saved.\n");
	  // Send abort to back-ends.
	  printf("---> Sending abort...\n");
	  for (cxn = 0; cxn < connections; cxn++) {
	    if(send(CTL_System.mgmt_socket_IDs[cxn], xfer, MAXDATASIZE, 0) == -1) {
	      perror("send() error on abort notice");
	    }
	    printf("[%d] Sent ABORT\n", cxn+1);
	  }

	} else {

	  for (cxn = 0; cxn < connections; cxn++) {

	    strcpy( xfer, name_info );
	    if(send(CTL_System.mgmt_socket_IDs[cxn], xfer, MAXDATASIZE, 0) == -1) {
	      perror("File_ID send() error");
	      return CBI_F_FAILURE;
	    }
	    printf("[%d] Sent filename: \"%s\"\n", cxn+1, name_info);

	    // Receive (new or echoed) full filename
	    strcpy( xfer, "" );
	    printf("[%d] Waiting for return of full filename...\n", cxn+1);
	    recv(CTL_System.mgmt_socket_IDs[cxn], xfer, MAXDATASIZE, 0);
	    strcpy( name_info, xfer );
	    printf("[%d] received: \"%s\"\n", cxn+1, name_info);

	    // TODO: timeout on this recv for fault-tolerance
	    recv(CTL_System.mgmt_socket_IDs[cxn], xfer, MAXDATASIZE, 0);
	    if ( strcmp(xfer, "FILE WRITE DONE\0") == 0 ) { 
	      printf("Back-end %d is done writing the file  [%s]\n", cxn+1, xfer);
	    }
	    fflush(stdout);

	  }
	}
      } //end STRCMP



      //---------------------------------------
      // Handle instrument SELECTION requests
      //---------------------------------------
      char *selptr = NULL;
      char *delim = "-";
      char selection_specs[CBI_MAX_BACKENDS][CBI_SELEC_SPEC_LENGTH];
      if ( strcmp(buf, "SELECTION") == 0 ) {
	printf("All back-ends ready for instrument selection...\n\n");
	cbi_manager_selection_handler(connections, selection_specs);
	strcpy( sinput, "" );
	sinput[0] = (char)input;
	for (cxn = 0; cxn < connections; cxn++) {
	  // If selection for a given back-end requires multi-stage input,
	  // break it out into stages by token and send each successively.
	  if (selection_specs[cxn][0] == '-') {
	    if (debug_level > CBI_DEBUG_LEVEL_2) {
	      printf("[%d] multi-stage selection command: %s\n", cxn+1, selection_specs[cxn] );
	    }
	    selptr = strtok( selection_specs[cxn], delim );
	    if(send(CTL_System.mgmt_socket_IDs[cxn], selptr, 4, 0) == -1) {
	      perror("send() error");
	    }
	    cbi_sleep(50); // FIXME: Request confirmation for each segment to ensure back-end receives.
                           //        Make multiple send attempts until confirmed.
	    while ( selptr != NULL ) {
	      selptr = strtok( NULL, delim );
	      if(send(CTL_System.mgmt_socket_IDs[cxn], selptr, 4, 0) == -1) {
		perror("send() error");
	      }
	      cbi_sleep(50);
	    }

	  } else {
	    if (debug_level > CBI_DEBUG_LEVEL_2) {
	      printf("[%d] single-stage selection command: %s \n",
		     cxn+1,
		     selection_specs[cxn] );
	    }
	    if(send(CTL_System.mgmt_socket_IDs[cxn], selection_specs[cxn], 1, 0) == -1) {
	      perror("send() error");
	    }
	    cbi_sleep(50);
	  } //endIF == '-'

	}
	printf("\n");
	fflush(stdout);
      }


      //------------------------------
      // Handle SERVER mode requests
      //------------------------------
      int        stat;
      //pthread_t  KA_thread, INPUT_thread;
      if ( strcmp(buf, "SERVER") == 0 ) {
	sprintf(message, "All back-ends are confirmed to be in SERVER MODE");
	log_message( S_DINFO, "", message );

	// Spawn Mnet 'keepalive' loop thread to prevent broken pipe error
	// that occurs when the connection to the MPMnet server times out due
	// to inactivity.  Pass in a reference to the variable that will be snooped
	// upon to trigger termination of the keepalive loop.
	server_mode_active = TRUE;
	stat = pthread_create( &(KA_thread), NULL, (void *)&(cbi_MPMnet_keepalive), &(server_mode_active) );
	if (debug_level > CBI_DEBUG_LEVEL_1) {
	  printf("KA_thread stat    = %d\n", stat);
	}
	stat = pthread_create( &(INPUT_thread), NULL, (void *)&(cbi_exit_server_mode), &(server_mode_active) );
	if (debug_level > CBI_DEBUG_LEVEL_1) {
	  printf("INPUT_thread stat = %d\n", stat);
	}

      } //endIF strcmp 




    } //endIF sret
    
  } //endFOR ;;
  

  return CBI_F_SUCCESS;

}
