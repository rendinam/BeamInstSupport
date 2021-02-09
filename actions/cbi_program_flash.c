//-----------------------------------------------------------------------+
// File         : cbi_program_flash.c                                    |
//                                                                       |
// Description  : Command used to flash a packaged executable image into |
//                and instrument.                                        |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


// File globals
char *error_buf;


int cbi_program_flash_prep( void ) {
  
  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int iidx, commstatus;

  //-------------------------------------------------
  // Define the list of prompts and their parameters
  //-------------------------------------------------
  int retval, input = FALSE;
  CBI_INIT_PROMPTS(1);


  int valid_inputs[5] = {4, (int)'y', (int)'Y', (int)'n', (int)'N'};
  int map_out[4]      = {    TRUE,     TRUE,     FALSE,    FALSE};
  prompts[0].supp_text_ptr    = NULL;
  prompts[0].prompt_text      = "\n\nWould you like to begin the flash programming process for ALL active instruments? (y/n): ";
  prompts[0].data_type        = CBI_CHAR;
  prompts[0].valid_inputs     = valid_inputs;
  prompts[0].map_out          = map_out;
  prompts[0].default_value    = 'n';
  prompts[0].destination      = &(input);
  prompts[0].last_val_default = FALSE;


  retval = CBI_PROMPTER();


  cbi_close_sockets();


  if (input == FALSE ) {
    printf("Flash programming operation aborted.\n");
    return CBI_F_FAILURE;
  }

  return CBI_F_SUCCESS;

}



int cbi_net_cleanup(int sock_num) {

  // Tell server to close its end of socket
  cbi_net_close_socket(sock_num);
  
  // Close local end of socket
  cbi_net_net_close(sock_num);

  return CBI_F_SUCCESS;

}


/*
int cbi_clear_flash(int sock_num) {

  int result;
  
  // clr error_buf 
  cbi_net_clr_error();
  
  // clr flash 
  result = cbi_net_clr_flash(sock_num);
     
  // check result 
  if (result <= 0) {
    error_buf = cbi_net_get_error();
    printf("%s", error_buf);
    printf(" Error clearing digital board flash memory!\n");
  } else {
    printf(" Flash memory cleared.\n");
  }
  fflush(stdout);

  return result;

}
*/

int clear_flash(int sock_num)
{
	int result;

	/* clr error_buf */
	cbi_net_clr_error();

	/* clr flash */
	result = cbi_net_clr_flash(sock_num);

	/* check result */
	if (result <= 0) {
		error_buf = cbi_net_get_error();
		printf("%s", error_buf);
		printf(" Error clearing DSP flash!\n");
	} else {
		printf(" DSP Flash Cleared (if erasable).\n");
	}

	return result;
}


int cbi_program_flash_post(int iidx) {
  //void* flash_post( void* arg ) {

  ////int iidx = (int)arg;

  int stat;

  // Open socket
  printf("Opening socket to   %s \n", CTL_System.p_Module[iidx]->comm.ethernet.hostname);
  int sock_num = cbi_net_fdopen( CTL_System.p_Module[iidx]->comm.ethernet.hostname );

  char mem_image[MAX_FLASH_SIZE];     // a local copy of the entire FLASH chip
  int used_sectors[MAX_FLASH_SIZE/FLASH_SECTOR_SIZE]; // a list of used sectors

  int i;
  FILE *infileptr;
  char line_buffer[133];
  int line_number;
  int record_type;
  int line_data_cnt;
  int sector;
  int sector_total;
  int temp_int;
  int extended_addr_offset;
  int record_addr_offset;
  int destination_addr;
  
  if (sock_num <= 0) {
    printf(" write_flash: Unable to obtain socket to device!\n");
    fflush(stdout);
    cbi_net_cleanup(sock_num);
    //pthread_exit((void*) F_FAILURE);
    return CBI_F_FAILURE;
  }

  //infileptr = fopen(appconfig.cbi_PT_EXE_file, "r");
  infileptr = fopen(CTL_System.cfg_file, "r");

  if (infileptr == NULL) {
    //printf(" write_flash: Could not open file:  %s\n", appconfig.cbi_PT_EXE_file);
    printf(" write_flash: Could not open file:  %s\n", CTL_System.cfg_file);
    cbi_net_cleanup(sock_num);
    pthread_exit((void*) CBI_F_FAILURE);
  }

  // Disable instrument CPU
  int address   = 0x10040001;// CPU_RESET;
  int data_cnt  = 1;	     /* only 1 longword at a time */
  int xfer_size = 4;	     /* only longwords */
  const int disable = 0;
  const int enable  = 1;
  int num_words;
  printf("Disabling DSP...\n");
  num_words = cbi_net_wr_mem(sock_num, address, data_cnt, xfer_size, (char *) &disable);
  
  // Clear the local arrays
  for (i=0; i<sizeof(mem_image); i++) {
    mem_image[i] = 0xff;    // assume that a cleared FLASH is always 0xff
  }
  for (i=0; i<sizeof(used_sectors); i++) {
    used_sectors[i] = 0;    // will set to 1 when occupied
  }

  // Read in the entire file. Fill the local arrays
  line_number = 0;
  while (!feof(infileptr)) {

    // read a line
    fgets(line_buffer, sizeof(line_buffer), infileptr);
    line_number++;

    // blank lines are not allowed
    if (line_buffer == NULL) {
      printf(" Error reading file: line number = %d", line_number);
      printf(" Blank line encountered\n");
      printf(" Writing Flash Aborted\n");
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return CBI_F_FAILURE;
    }
    
    // We did not get entire line if line_buffer is full and there
    // is no end of line character
    if ((strlen(line_buffer) == sizeof(line_buffer) - 1) &&
	line_buffer[strlen(line_buffer) - 1] != '\n') {
      printf(" Error reading file: line number = %d\n", line_number);
      printf(" Line length exceeds line buffer size\n");
      printf(" Writing Flash Aborted\n");
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return CBI_F_FAILURE;
    }

    // Process the line
    if (line_buffer[strlen(line_buffer) - 1] == '\n')
      line_buffer[strlen(line_buffer) - 1] = '\0';	/* get rid of newline */

    if (strlen(line_buffer) < 11) {
      printf(" Illegal data in file\n");
      printf(" Line number %d has only %d characters.\n",
	     line_number, (int) strlen(line_buffer));
      printf(" Writing Flash Aborted\n");
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return CBI_F_FAILURE;
    }
    
    if (line_buffer[0] != ':') {
      printf(" Illegal data in file\n");
      printf(" Line number %d does not begin with :\n", line_number);
      printf(" Writing Flash Aborted\n");
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return CBI_F_FAILURE;
    }

    // look for the record type
    // Record type is chars 7-8
    sscanf(&line_buffer[INTEL_RECORD_TYPE_OFFSET], "%02x", (unsigned int *)&record_type);
    switch (record_type) {

    case INTEL_ELAR:	// extended linear address record
      // get address offset from record
      sscanf(&line_buffer[INTEL_DATA_OFFSET], "%04x",
	     (unsigned int *)&extended_addr_offset);
      // move to bits [31..16]
      extended_addr_offset = extended_addr_offset << 16;
      // set this as the next address for storing data
      destination_addr = extended_addr_offset;
      break;
      
    case INTEL_DATA:
      // get byte count for this line
      sscanf(&line_buffer[INTEL_RECORD_LENGTH_OFFSET], "%02x",
	     (unsigned int *)&line_data_cnt);
      // get address offset for this line
      sscanf(&line_buffer[INTEL_LOAD_OFFSET], "%04x",
	     (unsigned int *)&record_addr_offset);
      // compute the destination address
      destination_addr = extended_addr_offset + record_addr_offset;

      // stuff data into the 'mem_image' array
      for (i = 0; i <= line_data_cnt - 1; i++) {
	sscanf(&line_buffer[INTEL_DATA_OFFSET + 2 * i], "%02x",
	       (unsigned int *)&temp_int);
	mem_image[destination_addr] = (signed char) temp_int;
	// set the sector flag
	used_sectors[destination_addr>>8] = 1;
	// increment destination address
	destination_addr++;
      }
      break;
      
    case INTEL_EOF:
      // Clear the FLASH
      printf(" Clearing FLASH\n");
      if (clear_flash(sock_num) <= 0) {
	printf(" write_flash: Clear FLASH failed. Writing FLASH aborted\n");
	cbi_net_close_socket(sock_num);
	  cbi_net_cleanup(sock_num);
	  fflush(stdout);
	  return CBI_F_FAILURE;
      }

      // send out data for any modified sectors
      sector_total = 0;
      for (sector=0; sector<sizeof(used_sectors); sector++) {
	if (used_sectors[sector] == 1) {	
	  putchar('.');
	  fflush(stdout);
	  cbi_net_clr_error();
	  // compute the address of the start of the sector
	  destination_addr = sector<<8;
	  if (cbi_net_wr_flash(sock_num, &mem_image[destination_addr], FLASH_SECTOR_SIZE,
			       destination_addr) != FLASH_SECTOR_SIZE) {
	    error_buf = cbi_net_get_error();
	    printf("\n");
	    printf("%s", error_buf);
	    printf(" Error writing flash: fail at sector %d\n", sector);
	    printf(" Consider a retry...\n");
	    cbi_net_cleanup(sock_num);
	    fflush(stdout);
	    return CBI_F_FAILURE;
	  }
	  sector_total++;
	}
      }
      printf("\n");
      printf(" End of file found, writing flash complete.\n");
      printf(" Total sectors written = %d\n", sector_total);

      data_cnt = 1;
      num_words = cbi_net_wr_mem(sock_num, address, data_cnt, xfer_size, (char *)&enable);
      num_words = cbi_net_wr_mem(sock_num, address, data_cnt, xfer_size, (char *)&disable);
      printf("Enabling DSP...\n");
      num_words = cbi_net_wr_mem(sock_num, address, data_cnt, xfer_size, (char *)&enable);
      
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return F_SUCCESS;
      break;
      
    default:
      printf(" Unsupported record type\n");
      printf(" Writing Flash Aborted\n");
      cbi_net_cleanup(sock_num);
      fflush(stdout);
      return CBI_F_FAILURE;
    }
    
  }
  
  printf(" Error: End of file reached without End of File Record.\n");
  cbi_net_cleanup(sock_num);
  fflush(stdout);
  return CBI_F_FAILURE;
  //pthread_exit((void*) CBI_F_FAILURE);

}
