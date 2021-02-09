//-----------------------------------------------------------------------+
// File         :  cbi_prompter.c                                        |
//                                                                       |
// Description  :  Processes a list of prompt descriptor structures and  |
//                 collects and stores responses appropriate to each     |
//                 entry in the descriptor list.                         |
//                                                                       |
// Arguments    :  int  num_prompts - number of prompts to display       |
//                 list of prompt descriptor structures,                 |
//                      CBI_INPUT_PROMPT_DESCRIPTOR  *pd_list,           |
//                                                                       |
//          TODO: Needs condensing across data types.                    |
//                                                                       |
// Author       :  M. Rendina                                            |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_prompter( int num_prompts, CBI_INPUT_PROMPT_DESCRIPTOR *pd_list ) {

  int debug_level = CTL_System.debug_verbosity;

  int retval = CBI_F_SUCCESS;
  int status, i, value;
  int val_list_len;

  // pointers for casting from void * type of I/O data structure
  float *fptr;
  int   *iptr;
  char  *sptr;
  
  // Function pointer for prompt support functions
  int (*fnc_ptr)()   = NULL;

  char send_this[100];

  int  prompt = 0;
  char def_val[10];
  char ptext[300];
  int  use_value;
  int  temp_comparison;


  while( prompt < num_prompts ) {

    //-----------------------------------------------
    // If this prompt has been marked as inactive,
    // skip it and move on to the next one.
    //-----------------------------------------------
    if (pd_list[prompt].active == FALSE) {
      prompt++;
      continue;
    }

    //-----------------------------------------------
    // Print any supplementary (menu) text provided
    //-----------------------------------------------
    if (pd_list[prompt].supp_text_ptr != NULL) {
      printf("%s", pd_list[prompt].supp_text_ptr );
      fflush(stdout);
    }

    //-------------------------------------------------
    // Compose default value field based on data type
    //-------------------------------------------------
    strcpy( def_val, "" );
    switch (pd_list[prompt].data_type) {
    case (CBI_INT):
      use_value = pd_list[prompt].default_value;
      if ( pd_list[prompt].last_val_default == TRUE &&
	   *(pd_list[prompt].last_value) != CBI_EMPTY_INPUT )  {
	//printf("overriding %d with %d\n", use_value, *(pd_list[prompt].last_value) );
	use_value = *(pd_list[prompt].last_value);
      }
      sprintf(def_val, "[%d]: ", use_value );
      break;
      
    case (CBI_CHAR):
      use_value = pd_list[prompt].default_value;
      if ( pd_list[prompt].last_val_default == TRUE &&
	   *(pd_list[prompt].last_value) != CBI_EMPTY_INPUT ) {
	//printf("overriding %c with %c\n", use_value, *(pd_list[prompt].last_value) );
	use_value = *(pd_list[prompt].last_value);
      } 
      sprintf(def_val, "[%c]: ", use_value );
      break;

    }

    //--------------------------------------------------
    // Compose prompt string with default value,
    // send to manager if in back-end mode, otherwise
    // print normally.
    //--------------------------------------------------
    strcpy( ptext, "" );
    if (CTL_System.RunningMode == CBI_BACKEND_MODE) {
      memset( send_this, 0, sizeof(send_this) );
      strcat( send_this, "PROMPT");
      strcat( send_this, pd_list[prompt].prompt_text );
      strcat( send_this, "\0" );
      strcat( send_this, def_val );
      if (debug_level > CBI_DEBUG_LEVEL_2) {
	  printf("Sending prompt text...\n");
      }
      fflush(stdout);
      if(send(CTL_System.mgmt_socket_IDs[0], send_this, strlen(send_this)+1, 0) == -1) {
	perror("send() error");
	fflush(stdout);
      }
    } else {
      fflush(stdout);
      strcat( ptext, pd_list[prompt].prompt_text );
      strcat( ptext, def_val );
      printf("%s", ptext );
      fflush(stdout);
    }


    //------------------------------------------------------
    // Perform input checking based on expected data type:
    //   - Check for default request
    //   - Check for previous prompt request
    //   - Check for command abort requset
    //   - Pass input through a validator routine
    //------------------------------------------------------
    switch (pd_list[prompt].data_type) {
    case (CBI_INT):
      fflush(stdout);
      iptr = (void *)(pd_list[prompt].destination);
      *iptr = cbi_get_int(); // Test non-blocking input loop in this function
      
      if (debug_level > CBI_DEBUG_LEVEL_0 && (*iptr != CBI_EMPTY_INPUT)) {
        printf(" |Input:%i| ",(int) *iptr);
      }

      // Test for default request
      if (*iptr == CBI_EMPTY_INPUT) {
        use_value = pd_list[prompt].default_value;
        if ( pd_list[prompt].last_val_default == TRUE ) {
          if ( *(pd_list[prompt].last_value) != CBI_EMPTY_INPUT ) {
            use_value = *(pd_list[prompt].last_value);
          }
        fflush(stdout);
      } 
      //printf("Default value  %d  being used.\n", use_value);
      *iptr = use_value;


      // Test for previous prompt request
      // FIXME: Make easier to use
      } else if (*iptr == -99) {
        if (prompt > 0) {
          prompt--;
        }
        continue;

	
      // Test for command abort request
      // Must cause calling prep function to return with
      // appropriate code to force redisplay of main menu.
      } else if (*iptr == -1000) {
        return CBI_F_FAILURE;


      // Data passed initial checks, now pass through
      // validator if one was provided for this prompt.
      } else {
        if (pd_list[prompt].validator != NULL) {
          fnc_ptr = pd_list[prompt].validator;
          status = (*fnc_ptr)( *iptr, pd_list[prompt].valid_inputs[0], pd_list[prompt].valid_inputs[1] );
          if (status == CBI_F_FAILURE) {
            printf("Invalid input, please try again.\n");
            continue;  // Will redisplay prompt
          }
        }

      }
      *(pd_list[prompt].last_value) = *iptr;
      if (debug_level > CBI_DEBUG_LEVEL_0) {
        printf("Saving input as last value   %d\n", *(pd_list[prompt].last_value) );
      }
      break;




    case (CBI_CHAR):
      fflush(stdout);
      temp_comparison = (int)cbi_get_char();
      
      if (debug_level > CBI_DEBUG_LEVEL_0) {
        if (temp_comparison != -1) {
          printf(" |Input:%c]| ",(char) temp_comparison);      
        }
      }

      // Test for default request
      if (temp_comparison == -1) {
        temp_comparison = (int)pd_list[prompt].default_value;
        if ( pd_list[prompt].last_val_default == TRUE &&
        *(pd_list[prompt].last_value) != CBI_EMPTY_INPUT ) {
          temp_comparison = *(pd_list[prompt].last_value);
        } 
      fflush(stdout);
      //printf("Default value  %c  being used.\n", temp_comparison);
      
      
      // Test for previous prompt request
      // Not yet implemented
      } else if (temp_comparison == -99) {
        if (prompt > 0) {
          prompt--;
        }
      continue;

	
      // Test for command abort request
      // Must cause calling prep function to return with
      // appropriate code to force redisplay of main menu.
      // Not yet implemented
      } else if (temp_comparison == -1000) {
        return CBI_F_FAILURE;
      }


      // Validate input
      val_list_len = pd_list[prompt].valid_inputs[0];
      for (value = 0; value < val_list_len; value++) {
        if ( (char)pd_list[prompt].valid_inputs[value+1] == (char)temp_comparison) {
          iptr = (void *)(pd_list[prompt].destination);
          if (pd_list[prompt].map_out == NULL) {
            *iptr = temp_comparison;
          } else {
            *iptr = pd_list[prompt].map_out[value];
          }
          break;
        }
      }

      *(pd_list[prompt].last_value) = temp_comparison;
      if (debug_level > CBI_DEBUG_LEVEL_0) {
        printf("Saving input as last value   %c\n", *(pd_list[prompt].last_value) );
      }
      break;
      


      default:
      	retval = CBI_F_FAILURE;

    }
    
    prompt++;

  } //endFOR prompt
    

  return retval;

}

