/*!-----------------------------------------------------------------------+
* File         :  cbi_xbus_peek.c                                         |
*                                                                         |
* Description  :  Retrieves a value into a memory location on the BPM via |
*                 XBUS communication method.                              |
*                                                                         |
* Arguments    :  Index of instrument in master management list (integer) |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_xbus_peek( int iidx, int addr, int* rval ) {

  int *adr_ptr = &(addr);
  *rval = 0;
  int *rval_ptr = rval;
  int status = CBI_F_SUCCESS;
  
  int vec_ele[2];
  vec_ele[0] = CTL_System.p_Module[iidx]->comm.xbus.element;
  vec_ele[1] = CTL_System.p_Module[iidx]->comm.xbus.element;

  status = vxputn_c(CTL_System.p_Module[iidx]->comm.xbus.adr_node, vec_ele[0], vec_ele[1], adr_ptr);
  if (stat == MPM_FAILURE) {
    return CBI_F_FAILURE;
  }
  status = vxgetn_c(CTL_System.p_Module[iidx]->comm.xbus.dat_node, vec_ele[0], vec_ele[1], rval_ptr);
  if (stat == MPM_FAILURE) {
    return CBI_F_FAILURE;
  }

  return CBI_F_SUCCESS;

}
