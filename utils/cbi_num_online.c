/*!-----------------------------------------------------------------------+
* File         :  num_online.c                                            |
*                                                                         |
* Description  :  Returns number of currently online devices.             |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

#include "CESR_platform.h"
#include "utilities.h"
#include "messages.h"  // c_utils

#include "cbi_core_includes.h"


int num_online() {
  
  int inst, count = 0;

  for (inst = 0; inst < CBI_MAX_INSTRUMENTS; inst++) {
    if (CTL_System.p_Module[inst]->active == CBI_MAINT_1 ||
	CTL_System.p_Module[inst]->active == CBI_MAINT_2 ||
	CTL_System.p_Module[inst]->active == CBI_MAINT_3 ||
	CTL_System.p_Module[inst]->active == CBI_MAINT_4 ||
	CTL_System.p_Module[inst]->active == CBI_ACTIVE ||
	CTL_System.p_Module[inst]->active == CBI_INACTIVE ) {
      count++;
    }
  }

  return count;
}

