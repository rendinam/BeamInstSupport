/*!-----------------------------------------------------------------------+
* File         :  cbi_prep_cesr_header.c                                  |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

// Location in the CESR MPM database that will contain a CONDX set index number.
#define  CESR_CONDX_NODE           "CONDX SETNUM"
#define  CESR_CONDX_ELE            1


#define  CERN_CURRENT_NODE         "CSR CERN CUR"
#define  CERN_CURRENT_ELE          1
// Constants for converting raw CERN current monitor output to
// measurement in mA units defined here.
// Taken from '/nfs/cesr/mnt/const/mpmdb/gen_convert.inp'.
#define  CERN_CURR_SCALE_FACTOR    0.000954
#define  CERN_CURR_OFFSET          0.408


int cbi_prep_cesr_header(RDV3_CESR_HEADER *cdata) {

  float corrected_cern_curr;

  // Compose field mapping
  fio_addrs addrs;
  addrs[0] = (long)&(CTL_System.meas_info.cesr_condx);
  cbpmfio_map_addresses_to_field(&RDV3_cesr_specs, 0, addrs);

  addrs[0] = (long)&((*cdata).raw_current);
  cbpmfio_map_addresses_to_field(&RDV3_cesr_specs, 1, addrs);
  
  addrs[0] = (long)&(CTL_System.meas_info.CERN_current);
  cbpmfio_map_addresses_to_field(&RDV3_cesr_specs, 2, addrs);
  // End field mapping


  // Obtain values from CESR MPM
  vxgetn_c(CERN_CURRENT_NODE, CERN_CURRENT_ELE, CERN_CURRENT_ELE, &((*cdata).raw_current));

  corrected_cern_curr = (((*cdata).raw_current) * CERN_CURR_SCALE_FACTOR) + CERN_CURR_OFFSET;
  CTL_System.meas_info.CERN_current = corrected_cern_curr;
  
  vxgetn_c(CESR_CONDX_NODE, CESR_CONDX_ELE, CESR_CONDX_ELE, &(CTL_System.meas_info.cesr_condx) );


  return CBI_F_SUCCESS;

}
