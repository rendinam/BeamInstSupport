/*!
*-------------------------------------------------------------------------+
* File         : cbi_gate_control.c                                       |
*                                                                         |
* Description  : Sets the gate enable bit in the status word on the       |
*                                                                         |
* Arguments    : Gate state (CBI_GATE_DISABLE) 0 LOW                      |
*                           (CBI_GATE_ENABLE)  1 HIGH                     |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_gate_control(int gate_state) {

  int stat;

  char *node = CBPM_COMMAND_NODE; 

  int ele[2];
  int gate_enable_code;

  if (gate_state == CBI_GATE_ENABLE) {
    gate_enable_code = CBI_GATE_ENABLE_CODE;
  } else {
    gate_enable_code = CBI_GATE_DISABLE_CODE;
  }

  ele[0] = CBPM_COMMAND_ELE;
  ele[1] = CBPM_COMMAND_ELE;
  stat = vxputn_c(node, ele[0], ele[1], &gate_enable_code );

  return stat;

}
