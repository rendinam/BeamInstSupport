//-----------------------------------------------------------------------+
// File         :  cbi_rw_struct.c                                       |
//                                                                       |
// Description  :  Converts between integer xbus data buffer and         |
//                 raw data structure for communications (READ/WRITE)    |
//                                                                       |
// Arguments    :  None                                                  |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author       :  M. Palmer / M. Rendina                                |
//-----------------------------------------------------------------------+

#include "cbi_core_includes.h"


int cbi_rw_struct(int mode, int comm_method, char *pkt_name, int pkt_ele, int pkt_tag,
                      COMM_KEY *p_key, int max_words){

//-----------------------------------------------------------------------+
// Local Variables -                                                     |
//                                                                       |
// func_name  -  Pointer to name of the local function                   |
//                                                                       |
//-----------------------------------------------------------------------+

   char *func_name = "cbi_rw_struct";
   char *p_msg;
   int i,j;
   int istat;
   
   int n_pkts;
   int n_total  = 0;
   int to_trans = 0;
   int pkt;
   int tag;

   int nwords    = 0;
   int written   = 0;
   
   // Create a buffer big enough to hold the maximum size struct 
   // that can ever be read
   int data_buf[CBI_MAX_READBACK_BUF];

   int *p_buf = data_buf;

   
   int   dtype;
   int   dcount;
   int   pkt_offset;
   float   ele_size;
   void *p_data;
   int* (*p_func)(int,int*,int,int,int,void*);
   
   int vec_ele[2] = {pkt_ele, pkt_ele};

   int nexpect    = 0;
   int nread      = 0;
   int nconvert   = 0;

   int i_word;

   const COMM_KEY_CONFIG *p_cfg = p_key->p_config;

   char *p_name = p_key->p_config->name;

   //-----------------------------------------------------------------------+
   //                     EXECUTABLE CODE STARTS HERE                       |
   //-----------------------------------------------------------------------+
   // Get the number of buffers to process and calculate the expected 
   // buffer size
   n_pkts = p_cfg->num_pkts;

   /*
   // Truncate the readback on demand
   if ((mode == READ) && (n_pkts > max_pkts_to_read)) {
      n_pkts = max_pkts_to_read;
   }
   */

   // Calculate number of words in the structure to transfer
   for (i = 0; i < p_cfg->num_vars; i++) {
      dcount   = p_cfg->data_count[i];
      ele_size = p_cfg->ele_size[i];
      n_total += dcount * ele_size;
   }

   // Set the number of words to transmit to the total
   to_trans = n_total;

   //printf("\n\n\n"); // DEBUG
   //printf("n_total = %d\n", n_total);  // DEBUG

   //-------------------------------------------------
   //  Load the packet buffer for writes
   //-------------------------------------------------
   if (mode == WRITE) {

      if ((p_cfg->record_type == VAR_REC_LENGTH) && (max_words != 0)) {
         to_trans = max_words;
      }

      p_buf  =  data_buf;

      // What is this?
      for (i = 0; i < p_cfg->num_vars; i++) {
         dtype      = p_cfg->data_type[i];
         dcount     = p_cfg->data_count[i];
         p_func     = p_cfg->func_ptr[i];
         p_data     = p_key->data.ptr[i];

         for (j = 0; j < dcount; j++) {
            p_buf    = (*p_func)(mode, p_buf, dtype, 1, j, p_data);
            nconvert = p_buf - data_buf;
            if (nconvert >= to_trans) break;
         }

         if (nconvert >= to_trans) break;
	 //   p_buf      = (*p_func)(mode, p_buf, dtype, dcount, 0, p_data);
      }
   }
  

   //-----------------------------------------------------------------------+
   // Loop over all packets and read/write struct data                      |
   //-----------------------------------------------------------------------+
   for (pkt = 0; pkt < n_pkts; pkt++) {

      //    Get the correct tag
      //      tag = pkt_tag + pkt;
      //    ? previous for sharc?
      //    Next for tiger sharc
      if (pkt == 0) tag = pkt_tag;
      if (pkt != 0) tag = pkt_tag + 2048;

      /*
      // Determine how many words to transmit
      if (to_trans > MX_PKT_DATA) {
         nexpect = MX_PKT_DATA;
      } else {
	//// mcr nexpect = to_trans;
	nexpect = p_key->p_config->pkt_size;
      }
      */
      
      nexpect = p_key->p_config->pkt_size;
      


      // When handling variable length records, break before packet loop complete
      if (nexpect == 0) break;

      to_trans -= nexpect;

      // Get the buffer pointer
      p_buf = data_buf + (pkt * p_key->p_config->pkt_size);


      //
      //  READ MODE - get the specified packet from the XBUS
      //   
      if (mode == READ) {
	//printf("cbi_rw_struct: Reading packet %d / %d       --      size: %d\n", pkt+1, n_pkts, nexpect);  //DEBUG
         nwords = cbi_rw_pkt(mode, comm_method, pkt_name, vec_ele, tag, p_buf, nexpect);
         if ((nwords == CBI_VMPM_FAILURE) || (nwords > nexpect)) {
            sprintf(message,"XBUS packet READ error for struct %s from node %s %d - cbi_rw_struct FAILURE. ", 
                    p_name, pkt_name, pkt_ele);
            log_message(S_ERROR,func_name,message);
            return CBI_F_FAILURE;
         } 

         nread += nwords;

         if (nwords < nexpect) {
            if ((p_cfg->record_type == FIXED_REC_LENGTH) && (nwords < nexpect)) {
               sprintf(message,"XBUS packet READ error for struct %s from node %s %d",
                       p_name, pkt_name, pkt_ele);
               log_message(S_ERROR,func_name,message);
               sprintf(message,"Read %d words when expecting %d! - cbi_rw_struct FAILURE. ", nwords, nexpect);
               log_message(S_ERROR,func_name,message);
               return CBI_F_FAILURE;
            } 

	    // Do not read back any more words - either have an error or have hit the 
	    // end of a variable length record
            printf("Break2\n");
            break;
         }
      }
   

      //
      //  WRITE MODE - put the specified packet out on the XBUS
      //
      if (mode == WRITE) {
	//printf("cbi_rw_struct: Writing packet %d / %d       --      size: %d\n", pkt+1, n_pkts, nexpect);  // DEBUG
         nwords = cbi_rw_pkt(mode, comm_method, pkt_name, vec_ele, tag, p_buf, nexpect);
         if (nwords != nexpect) {
            sprintf(message,"XBUS packet WRITE error for struct %s from node %s %d - cbi_rw_struct FAILURE", 
                    p_name, pkt_name, pkt_ele);
            log_message(S_ERROR,func_name,message);
            return CBI_F_FAILURE;
         } 

      } // End if (mode == WRITE)

   } // End for (pkt....)



   //
   //  Put the buffer contents in the correct memory locations for reads
   //
   if (mode == READ) {
      p_buf = data_buf;

      //printf("Num vars = %d\n", p_cfg->num_vars); // DEBUG
      for (i = 0; i < p_cfg->num_vars; i++) {
         dtype      = p_cfg->data_type[i];
         dcount     = p_cfg->data_count[i];
         p_func     = p_cfg->func_ptr[i];
         p_data     = p_key->data.ptr[i];

	 //printf("addr: %d , dat field as INT : %d\n", p_buf, *p_buf); // DEBUG

	 // Convert only the necessary range of a communications structure if 
	 // reading back variable length records (NOTE:  records should end 
	 // on a specific element boundary, NOT in the middle of complex elements)
         for (j = 0; j < dcount; j++) {

	   p_buf = (*p_func)(mode, p_buf, dtype, 1, j, p_data);
	   nconvert = p_buf - data_buf;

	   if (nconvert >= nread) break;

         }
         if (nconvert >= nread) break;
      }

   }
  

   return CBI_F_SUCCESS;
} 

//-----------------------------------------------------------------------+
// End function cbi_rw_struct                                            |
//-----------------------------------------------------------------------+





