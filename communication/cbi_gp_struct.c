//-----------------------------------------------------------------------+
// File         :  cbi_gp_struct.c                                       |
//                                                                       |
// Description  :  Gets/Puts (hence the "gp") any of the communications  |
//                 structures in the DSPs.  This function ASSUMES that   |
//                 information in the structure of origin is valid and   |
//                 justs takes care of the transfer of information.      |
//                                                                       |
// Arguments    :  int mode              - READ/WRITE                    |
//                 int structure tag                                     |
//                 CBPM_CTL_MODULE *cms  - control module structure for  |
//                                         module of interest            |
//                                                                       |
// Return Value :  Status - F_SUCCESS/F_FAILURE                          |
//                                                                       |
// Author(s)    :  M. Palmer / J. Kern / M. Rendina                      |
//-----------------------------------------------------------------------+

// Hardware-specific provisions
#include "dig_board_6048-113.h"

// Local includes
#include "cbi_core_includes.h"


int cbi_gp_struct(int rwmode, int comm_method, int struct_tag, CBI_CTL_MODULE *cms, int words_to_xfer) {

  //-----------------------------------------------------------------------+
  // Local Variables -                                                     |
  //                                                                       |
  // func_name  -  Pointer to name of the local function                   |
  //                                                                       |
  //-----------------------------------------------------------------------+
   char *func_name = (char *)__FUNCTION__;
   
   int debug_level = CTL_System.debug_verbosity;

   char *p_node;   //Specific to xbus comm_method
   int   pkt_ele;  //Specific to xbus comm_method
   int   istat;

   COMM_KEY *p_key;  
   int dsp_tag;
   int exe_flag;
   int exe_type;

   int (*p_func)(int,void*);

   char *rw_names[2] = { "READ", "WRITE" };
   
   char *err_fmt[2] = {
      "Unable to READ  (struct_tag = %d) from:  %13s %d\n",
      "Unable to WRITE (struct_tag = %d) to  :  %13s %d\n"
   };

   //int words_to_xfer = 0;  // 0 - means transfer the whole structure

   int istruct;
   int idummy;
   int i;
   int cnt;

   //-----------------------------------------------------------------------+
   //                     EXECUTABLE CODE STARTS HERE                       |
   //-----------------------------------------------------------------------+

   //------------------------------------
   // Get proper tag for the DSP struct
   //------------------------------------
   p_key   = &(cms->key[ cms->tag2key_map[struct_tag]]);

   const COMM_KEY_CONFIG *p_cfg = p_key->p_config;

   if (comm_method == XBUS) {

     // Get node and element info
     p_node  = (cms->comm).xbus.pkt_node;
     pkt_ele = (cms->comm).xbus.element;
     
     
     //  If mode is WRITE, make sure that any specified copy/check routine runs before 
     //  downloading data to the remote processor.
     if (rwmode == WRITE) {  
       if ((p_func = (p_key->p_config)->check_copy_ptr)  != NULL) {
	 istat = (*p_func)(WRITE,(void *)cms);
	 if (istat != F_SUCCESS) {
	   sprintf(message,"Copy/Check Failure during XBUS WRITE for %s ele:%d", p_node, pkt_ele  );
	   log_message(S_ERROR,func_name,message);
	   return CBI_F_FAILURE;
	 }
       }
     }

     //-----------------
     //  TRANSFER DATA
     //-----------------
     if ((rwmode == READ) || (rwmode == WRITE)) {
       // Read or Write specified struct
       istat = cbi_rw_struct(rwmode, comm_method, p_node, pkt_ele, struct_tag, p_key, 0);
       if (istat != F_SUCCESS) {
	 sprintf(message, err_fmt[rwmode], struct_tag, p_node, pkt_ele);
	 log_message(S_ERROR, func_name, message);
	 return CBI_F_FAILURE;
       }
       
     } else {
       idummy = sprintf(message, 
			"Invalid transfer mode specified:  %d\n",rwmode);
       log_message(S_ERROR, func_name, message);
       return CBI_F_FAILURE;
     }
     
     if (rwmode == READ) {
       if ((p_func = (p_key->p_config)->check_copy_ptr)  != NULL) {
	 istat = (*p_func)(READ,(void *)cms);
	 if (istat != F_SUCCESS) {
	   sprintf(message,"Copy/Check Failure during READ for (?)");
	   log_message(S_ERROR,func_name,message);
	   return CBI_F_FAILURE;
	 }
       }
     }
     

     //-----------------------------------------------------------------
     

   } else if (comm_method == ETHERNET) {

     //---------------------------------------------------------------
     // First step, get the on-board address of the beginning of the
     // remote structure that will be retrieved or updated.
     //---------------------------------------------------------------
     char *error_buf;
     int  xferd_words;
     int  xfer_size    = 4;    // in BYTES as per cbi_net specification.
     int  address      = CBI_6048_113_PKT_ADDR_TABLE + struct_tag;
     int  data_cnt     = 1;
     int  data         = 0;

     // Drop out if there is no socket available. i.e. The open (performed earlier, elsewhere) failed.
     if (cms->comm.ethernet.socket_ID == CBI_SOCKET_CLOSED) {
       return CBI_F_FAILURE;
     }


     xferd_words    = cbi_net_rd_mem(cms->comm.ethernet.socket_ID, address, data_cnt, xfer_size, (char *)&data);
     if (xferd_words != data_cnt) {
       error_buf = cbi_net_get_error();
       sprintf(message, "cbi_net: %s", error_buf);
       log_message(S_BLANK,func_name,message);
       return CBI_F_FAILURE;
       //===========================================================================
       //===========================================================================

     }
     

     
     //---------------------------------------------------------
     // Calculate number of words in the structure to transfer
     //---------------------------------------------------------
     int dcount, n_total = 0;
     float ele_size;

     // If a positive words_to_xfer value was provided, set up to transfer that many
     // words.  Otherwise, transfer the contents of the entire structure.
     if (words_to_xfer != CBI_FULL_STRUCT) {
       n_total = words_to_xfer;
     } else {
       for (i = 0; i < p_cfg->num_vars; i++) {
	 dcount   = p_cfg->data_count[i];
	 ele_size = p_cfg->ele_size[i];
	 n_total += dcount * ele_size;
       }
     }
     
     data_cnt = 32000; // Max size of a single transfer in words
     // Derived from MAX_REQ_BYTES in cbi_net_common.h
     int xferred  = 0;
     // If number of words to transfer is larger than that max size, break it into
     // packets.  Otherwise just transfer the whole thing with a single CBInet call.
     if (n_total < data_cnt) {
     	 data_cnt = n_total;
     }
     int xfers = n_total / data_cnt;
     
     int *ptr = (int *)p_key->data.ptr[0];
     
     
     for ( i = 0; i < xfers; i++) {
       cbi_net_clr_error();
       // Switch here based on read/write mode:
       if (rwmode == READ) {
	 xferd_words = cbi_net_rd_mem(cms->comm.ethernet.socket_ID, data, data_cnt, xfer_size, (char *)ptr);

       } else {
	 xferd_words = cbi_net_wr_mem(cms->comm.ethernet.socket_ID, data, data_cnt, xfer_size, (char *)ptr);
       }
       if (xferd_words != data_cnt) {
	 error_buf = cbi_net_get_error();
	 sprintf(message, "cbi_net: %s - instrument %s", error_buf, cms->comm.ethernet.hostname);
	 log_message( S_ERROR, message, func_name );
	 return CBI_F_FAILURE;
	 //===========================================================================
	 //===========================================================================

       }

       data    = data + data_cnt;
       xferred = xferred + data_cnt;
       ptr     = ptr + data_cnt;
     }
     
       
     // Transfer any leftovers outside of the integer number of CBInet packets...
     if (xferred != n_total) {
       data_cnt = n_total - xferred;
       xferd_words       = cbi_net_rd_mem(cms->comm.ethernet.socket_ID, data, data_cnt, xfer_size, (char *)ptr);
       xferred = xferred + data_cnt;
     }

     
   } // end comm mode
   

   // If we made it here, we were successful      
   return CBI_F_SUCCESS;   
   
}
//-----------------------------------------------------------------------+
// End function cbi_gp_struct                                            |
//-----------------------------------------------------------------------+
