#ifndef CBI_COMMUNICATION_H
#define CBI_COMMUNICATION_H
//-----------------------------------------------------------------------+
// File         :  cbi_CO_communication.h                                |
//                                                                       |
// Description  :  This header file defines parameters, globals, and     |
//                 function prototypes for the communications functions. |
//                                                                       |
// Author       :  M. Palmer/J. Kern                                     |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


//-----------------------------------------------------------------------+
// Other headers needed by this header file:                             |
//                                                                       |
//-----------------------------------------------------------------------+
#include "cbi_core_includes.h"


//-----------------------------------------------------------------------+
// Globals:                                                              |
//                                                                       |
//-----------------------------------------------------------------------+
#if defined (CBI_CO_NO_EXTERN)
  ////int cbi_comm_typeout = NO;
#else
  extern int cbi_comm_typeout;
#endif

  //HARDWIRE?
#if defined (PKT_LIMIT_NO_EXTERN)
  ////int max_pkts_to_read = 128;
#else
   extern int max_pkts_to_read;
#endif


//-----------------------------------------------------------------------+
// Function prototypes:                                                  |
//                                                                       |
//-----------------------------------------------------------------------+

  extern int cbi_gp_struct(int, int, int, CBI_CTL_MODULE*, int);  
  extern int cbi_rw_struct(int, int, char*, int, int, COMM_KEY*, int);
  extern int cbi_rw_pkt(int, int, char*, int[], int, int*, int);
  extern int cbi_f_ctl2dsp(int);
  extern int cbi_f_dsp2ctl(int);
  extern int cbi_c2i(char command[4]);
  extern int cbi_handshake(int, CBI_CTL_MODULE*);
  extern int cbi_get_errors(int*,int*,int);
  extern int cbi_comm_sleep(int);
  extern int cbi_rw_xvec(int,int,char*,int[],int*);
  extern int cbi_rw_mvec(int,int,char*,int[],int*);

  extern int cbi_init_a(CBI_CTL_MODULE*);
  

#if defined(SHARC_DSP)
  extern int cbi_set_error(int, volatile int *);
#else
  extern int cbi_set_error(int, int *);
#endif



#if defined (__cplusplus)
}
#endif



#endif // CBI_COMMUNICATION_H



