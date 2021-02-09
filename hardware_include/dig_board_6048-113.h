#ifndef DIG_BOARD_6048_113_H
#define DIG_BOARD_6048_113_H
//------------------------------------------------------------------------+
// File : dig_board_6048-113.h                                            |
//                                                                        |
// Description : This header defines constants and pointers specific to   |
//               use of a beam instrument digital mainboard, otherwise    |
//               known as the digital board or DSP board, revision        |
//               6048-113.                                                |
//                                                                        |
// Author : C. Strohman / M. Palmer / M. Rendina                          |
//------------------------------------------------------------------------+


#define CBI_6048_113_PKT_ADDR_TABLE   0x10001000
#define CBI_6048_113_PKT_SIZE_TABLE   0X10010000


//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


#if defined (INST_NO_EXTERN)

   volatile int *p_pkt_adr_table      = (int *)CBI_6048_113_PKT_ADDR_TABLE;
   volatile int *p_pkt_siz_table      = (int *)CBI_6048_113_PKT_SIZE_TABLE;


#else  

   extern volatile int *p_pkt_adr_table;
   extern volatile int *p_pkt_siz_table;

#endif // (INST_NO_EXTERN)


#if defined (__cplusplus)
}
#endif


#endif // DIG_BOARD_6048_113_H
