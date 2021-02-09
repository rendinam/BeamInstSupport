/*!
*-----------------------------------------------------------------------+
* File         :  cbi_init_prompts.c                                    |
*                                                                       |
* Description  :  Initialize the elements of each entry in a list of    |
*                 user input prompt descriptors.                        |
*                 Each routine that makes use of packaged user input    |
*                 prompts must call this function in order to take      |
*                 advantage of all prompt engine features.              |
*                                                                       |
* Arguments    :  -Pointer to array of CBI_INPUT_DESCRIPTOR structures  |
*                 -(int) number of prompt descriptors in the array      |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"

#include "cbi_core_includes.h"


int cbi_init_prompts( CBI_INPUT_PROMPT_DESCRIPTOR *prompts, int num_prompts ) {
  
  int i;

  for (i = 0; i < num_prompts; i++) {
    prompts[i].active           = TRUE;
    prompts[i].prompt_text      = NULL;
    prompts[i].supp_text_ptr    = NULL;
    prompts[i].data_type        = (int)NULL;
    prompts[i].valid_inputs     = NULL;
    prompts[i].default_value    = (int)NULL;
    prompts[i].last_val_default = FALSE;
    prompts[i].destination      = NULL;
    prompts[i].harvester        = NULL;
    prompts[i].validator        = NULL;
  }

  return CBI_F_SUCCESS;

}

