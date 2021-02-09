#ifndef CBI_COMM_STRUCT_H
#define CBI_COMM_STRUCT_H
//-----------------------------------------------------------------------+
// File         :  cbi_comm_struct.h                                     |
//                                                                       |
// Description  :  Definitions for structures that facilitate the        |
//                 mechanism of transferring the contents of a           |
//                 specially-defined communications data structure       |
//                 between an instrument and the controlling software.   |
//                                                                       |
//              Many elements are used when sending and receiving        |
//              structure contents via ETHERNET and XBUS, but some are   |
//              only employed by the XBUS method (pkt_size, etc).        |
//-----------------------------------------------------------------------+

#define MX_STRUCT_VARS     35


enum CBPM_STRUCT_RECORD {
   FIXED_REC_LENGTH,       // structure comm uses fixed length records
   VAR_REC_LENGTH          // structure comm uses variable length records
};


   //-----------------------------------------------------------
   // Configuration information is the same for all copies of 
   // a structure.
   //-----------------------------------------------------------
   typedef struct {
         char *name;                       // Name of the struct
         int   struct_tag;                    // Control system communications tag for 1st pkt - 
                                           // this tag gives a unique ID for each data structure 
                                           // that the control system handles for ALL defined DSP 
                                           // executables - renamed from ctl_tag, now equivalent
					   // to the dsp_tag
         int   exe_allowed;                // Instrument executable type for which the struct is defined.  If 
                                           // set to GENERIC_EXE, then the data transfer is valid for all
                                           // instrument executables 
                                           // (NOTE:  setting the GENERIC_EXE flag does NOT 
                                           // necessarily mean that a particular executable will fill  
                                           // the data structure with useful information 
         int   pkt_size;                   // The optimal size of packet to use when transferring this structure.
         int   num_pkts;                   // Number of pkts necessary to transfer this data structure
         int   num_vars;                   // How many variables in struct
         int   protection;                 // R/W permission for the struct
         int   record_type;                // Allow fixed or variable length packets
         int   data_type[MX_STRUCT_VARS];  // Type of each var
         int   data_count[MX_STRUCT_VARS]; // Total Count of each variable
         float ele_size[MX_STRUCT_VARS];   // Size of an element of each var
         int*  (*func_ptr[MX_STRUCT_VARS])(int,int*,int,int,int,void*); 
                                           // Conv func
         int   (*check_copy_ptr)(int, void*); // Pointer to custom check/copy data function
         int   (*io_ptr)(int, void*, int); // Stream IO function
   } COMM_KEY_CONFIG;


   //-----------------------------------------------------------
   // Data pointers depend on which module is being considered
   //-----------------------------------------------------------
   typedef struct {
         void *ptr[MX_STRUCT_VARS];        // Data pointer for each var
   } COMM_KEY_DATA;
   

   //-----------------------------------------------------------
   // COMM_KEY contains both configuration and data parts
   //-----------------------------------------------------------
   typedef struct {
         const COMM_KEY_CONFIG *p_config;
         COMM_KEY_DATA   data;
   } COMM_KEY;



#endif  // CBI_COMM_STRUCT_H
