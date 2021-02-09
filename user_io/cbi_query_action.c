//-----------------------------------------------------------------------+
// File         :  cbi_query_action.c                                    |
//                                                                       |
// Description  :  Prompts user on the console for the next action to    |
//                 perform.  Otherwise known as the main menu function.  |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_query_action( CBI_COMMAND commands[], int num_cmds ) {

   char *func_name = "enter_action";
   int value;
   char plural;
   int command;


   printf("\n\n\n\n                      v%d.%d.%d%s\n\n",
	  CTL_System.version.major,
	  CTL_System.version.minor,
	  CTL_System.version.patchlevel,
	  CTL_System.version.modifier     );


   if (CTL_System.RunningMode == CBI_MANAGER_MODE) {
     printf("     MENU OF ACTIONS:\n");
   } else {

     printf("-----------------------------\n");
     if( cbi_num_active() == 1 ) {
       plural = ' ';
     } else {
       plural = 's';
     }
     printf("  Commanding  %d  BPM%c \n", cbi_num_active(), plural );
     printf("    of  %d  online.\n", num_online() );
     
     printf("-----------------------------\n");
   }


   if (CTL_System.RunningMode != CBI_BACKEND_MODE) {
     for (command = 0; command < num_cmds; command++) {
       if (commands[command].type != HIDDEN_TYPE) {
	 printf("%3d) %s\n", command, commands[command].name);
       }
     }
   
     printf("---------------------------\n");
     printf("Enter action: ");

   }


    int   nbytes; 
    char *menu_ready = "MENU\0"; 
    char  sbuf[10] = {""}; 


    // Set recv timeout value
    struct timeval tv;
    tv.tv_sec = 1;


    if (CTL_System.RunningMode == CBI_BACKEND_MODE) {

     while(TRUE) {
       
       printf("Sending menu ready indicator...\n");
       fflush(stdout);
       if(send(CTL_System.mgmt_socket_IDs[0], menu_ready, strlen(menu_ready)+1, 0) == -1) {
	 perror("send() error");
	 close(CTL_System.mgmt_socket_IDs[0]);
	 exit(1);
       }
       nbytes = -1;
       strcpy(sbuf, ""); //new
       if ((nbytes = recv(CTL_System.mgmt_socket_IDs[0], sbuf, sizeof(sbuf), 0)) <= 0) {
	 // got connection closed or error from client
	 if (nbytes == 0) {
	   printf("socket %d hung up\n", CTL_System.mgmt_socket_IDs[0]);
	 } else {
	   perror("recv");
	 }
	 close(CTL_System.mgmt_socket_IDs[0]);
	 
       } else {	    // we got some data from a client
	 printf("Got response from manager: \"%s\"\n", sbuf);
	 if (strcmp(sbuf, "GO") == 0) {
	   printf("Response accepted!\n");
	   break;
	 } 
       } //endIF recv
       cbi_sleep(600);
       
     }
     
    }
    
    
    char buf[25];
    value = cbi_get_int();
    //printf("VALUE = %d\n", value);
    
    
    return value;
    
}
