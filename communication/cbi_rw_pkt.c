//-----------------------------------------------------------------------+
// File         :  cbi_dsp_rw_pkt.c                                      |
//                                                                       |
// Description  :  Converts between integer xbus data buffer and         |
//                 raw data structure for communications (READ/WRITE)    |
//                                                                       |
// Arguments    :                                                        |
//                                                                       |
// Return Value :  Number of words transferred                           |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_rw_pkt(int rwmode, int comm_method, char *pkt_node, int vec_ele[2], int pkt_tag, 
                int *data_buf, int data_cnt){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+
   
   int nxwords[2];
   int dummy_pkt_tag[2];

   char *func_name = "rw_pkt";
   int nwords=0;
   int i,j;
   int i_word;
   char *p_msg;
   int istat;
   int *p_buf=data_buf;
   //-----------------------------------------------------------------------+
   //                     EXECUTABLE CODE STARTS HERE                       |
   //-----------------------------------------------------------------------+
   dummy_pkt_tag[0]=pkt_tag;
   //
   //  READ MODE
   //  
   // If in READ mode, get the specified packet from the XBUS
   // Do one retry if needed
   if (rwmode == READ) {
        nxwords[0]=0;
      if (comm_method == XBUS) {
        if (vugetn_c(pkt_node, vec_ele[0], vec_ele[1], p_buf, &dummy_pkt_tag[0], &nxwords[0]) != 1) {
           istat = vugetn_c(pkt_node, vec_ele[0], vec_ele[1], p_buf, &dummy_pkt_tag[0], &nxwords[0]);
           if ( istat != 1 ) {
              nwords = CBI_VMPM_FAILURE;
           }
        }
        nwords=nxwords[0]; 
      } else {
	return CBI_F_FAILURE;
      }

      //    Do a dump of the READ buffer if in debug mode
      //      if (CTL_System.TypeoutMode == TYPEOUT_DBG) {
      if (cbi_comm_typeout != NO) {
         sprintf(message, 
              "READ dump for %15s, ele = %3d, tag = %3d",
              pkt_node, vec_ele[0], pkt_tag);
         log_message(S_INFO,func_name,message);
         sprintf(message,"%3d word communications buffer:\n",data_cnt);
         log_message(S_BLANK,func_name,message);
         for (i = 0; i < 32; i++) {
            p_msg = message;
            for (j = 0; j < 8; j++) {
               i_word = 8*i + j;
               if (i_word >= data_cnt)    break;
               if (j == 0) p_msg += sprintf(p_msg,"%3d:  ",i_word);
               p_msg += sprintf(p_msg, "0X%08x ",data_buf[i_word]);
            }
            log_message(S_BLANK,func_name,message);
            if (i_word >= data_cnt) break;
         }
      }     

   } // End if (rwmode == READ)
   

//
//  WRITE MODE
//
// If in WRITE mode, put the specified packet out on the XBUS
   if (rwmode == WRITE) {
      if (comm_method == XBUS) {
	// Do ONE retry if necessary
        nwords = data_cnt;
        nxwords[0]=nwords;
        if (vuputn_c(pkt_node, vec_ele[0], vec_ele[1], data_buf, &dummy_pkt_tag[0], &nxwords[0]) != 1) {
           istat  = vuputn_c(pkt_node, vec_ele[0], vec_ele[1], data_buf, &dummy_pkt_tag[0], &nxwords[0]);

	   // Clear the nwords value if packet operation failed
           if ( istat != 1 ) {
              nwords = CBI_VMPM_FAILURE;
           } 

        }
      } else {
	return CBI_F_FAILURE;
      }
   
      // Do a dump of the WRITE buffer if in debug mode
      //if (CTL_System.TypeoutMode == TYPEOUT_DBG) {
      if (cbi_comm_typeout != NO) {
         sprintf(message, 
                 "WRITE dump for %15s, ele = %3d, tag = %3d",
                 pkt_node, vec_ele[0], pkt_tag);
         log_message(S_INFO,func_name,message);
         sprintf(message,"%3d word communications buffer:\n",data_cnt);
         log_message(S_BLANK,func_name,message);
         for (i = 0; i < 32; i++) {
            p_msg = message;
            for (j = 0; j < 8; j++) {
               i_word = 8*i + j;
               if (i_word >= data_cnt)    break;
               if (j == 0) p_msg += sprintf(p_msg,"%3d:  ",i_word);
               p_msg += sprintf(p_msg, "0X%08x ",data_buf[i_word]);
            }
            log_message(S_BLANK,func_name,message);
            if (i_word >= data_cnt) break;
         }
      }     

   } // End if (rwmode == WRITE)

   return nwords;
} 
//-----------------------------------------------------------------------+
// End function cbi_rw_pkt                                               |
//-----------------------------------------------------------------------+






