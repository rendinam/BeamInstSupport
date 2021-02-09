/*!-----------------------------------------------------------------------+
* File         :  cbi_xbus_poke.c                                         |
*                                                                         |
* Description  :  Pokes a value into a memory location on the BPM via     |
*                 XBUS communication method.                              |
*                                                                         |
* Arguments    :  Index of instrument in master management list (integer) |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_xbus_poke( int iidx, int addr, int value ) {

  int *adr_ptr = &(addr);
  int *val_ptr = &(value);

  int stat, vec_ele[2];

  vec_ele[0] = CTL_System.p_Module[iidx]->comm.xbus.element;
  vec_ele[1] = CTL_System.p_Module[iidx]->comm.xbus.element;

  stat = vxputn_c(CTL_System.p_Module[iidx]->comm.xbus.adr_node, vec_ele[0], vec_ele[1], adr_ptr);
  if (stat == MPM_FAILURE) {
    return CBI_F_FAILURE;
  }
  stat = vxputn_c(CTL_System.p_Module[iidx]->comm.xbus.dat_node, vec_ele[0], vec_ele[1], val_ptr);
  if (stat == MPM_FAILURE) {
    return CBI_F_FAILURE;
  }

  return CBI_F_SUCCESS;

}
