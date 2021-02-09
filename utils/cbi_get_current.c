/*!-----------------------------------------------------------------------+
* File         :  cbi_get_current.c                                       |
*                                                                         |
* Description  :  Get the machine current currently measured              |
*                 2 functions:                                            |
*                                                                         |
*                 get_cern_current - get CERN current from MPM DB         |
*                                                                         |
*                 get_species_current - get species specific total current|
*                       from MPM DB                                       |
*                                                                         |
* Arguments    :                                                          |
*                                                                         |
* Author       :  M.Stedinger                                             |
*                                                                         |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define  CERN_CURRENT_NODE         "CSR CERN CUR"
#define  CERN_CURRENT_ELE          1
// Constants for converting raw CERN current monitor output to
// measurement in mA units defined here.  Taken from 'gen_convert.inp'.
#define  CERN_CURR_SCALE_FACTOR    0.000954
#define  CERN_CURR_OFFSET          0.408


float get_cern_current() {
	
    int   raw_cern_curr_val;
    float corrected_cern_curr;    	
	vxgetn_c(CERN_CURRENT_NODE, CERN_CURRENT_ELE, CERN_CURRENT_ELE, &raw_cern_curr_val);
    corrected_cern_curr = (raw_cern_curr_val * CERN_CURR_SCALE_FACTOR) + CERN_CURR_OFFSET;
    CTL_System.meas_info.CERN_current = corrected_cern_curr;
    
    return CTL_System.meas_info.CERN_current;
}

#define  SPECIES_CURRENT_NODE           "CSR CURRENTS"
#define  ELE_CURRENT_ELE          		1
#define  POS_CURRENT_ELE          		2

float get_species_current(int species) {
	
	int ele = 0;
	int raw_species_current;
	float species_current;
	
	switch (species) {
		case (POSITRONS) :
			ele = POS_CURRENT_ELE;
			break;
		case (ELECTRONS) :
			ele = ELE_CURRENT_ELE;
			break;
		default:
			return -1;
	}
	
	vxgetn_c(SPECIES_CURRENT_NODE, ele, ele, &raw_species_current);
	species_current = raw_species_current * 0.01;
	
	return species_current;
}
