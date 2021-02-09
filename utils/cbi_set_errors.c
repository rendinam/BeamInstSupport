//-----------------------------------------------------------------------+
// File         :  cbi_set_errors.c                                      |
//                                                                       |
// Description  :  This file contains the functions -                    |
//                 cbi_set_error, cbi_get_errors                         |
//                                                                       |
// Prototypes   :  int cbi_set_error(int)                                |
//                 int cbi_get_errors(int*, int*, int)                   |
//                                                                       |
// Arguments    :  cbi_dsp_set_error  - error enumeration value          |
//                 cbi_dsp_get_errors - pointer to error word array      |
//                                 - pointer to list which will receive  |
//                                   list of error enumeration values    |
//									 number of error codes               |
//                                                                       |
// Return Values:  cbi_set_error  - DSP_F_SUCCESS/DSP_F_FAILURE          |
//                 cbi_get_errors - number of errors                     |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

// Local includes
#include "cbi_core_includes.h"


//-----------------------------------------------------------------------+
// Definitions -                                                         |
//                                                                       |
//                                                                       |
//-----------------------------------------------------------------------+

#define BIT      0x01
#define LEN_WORD 32
#define ERR_CLEAR 0
#define CBI_NO_ERR 1
#if defined(SHARC_DSP)
int cbi_set_error(int error, volatile int *error_array){
#else
int cbi_set_error(int error, int *error_array){
#endif

   int word;
   int shift;
   int mask;

#if defined(SHARC_DSP)   
   // Set our routine ID in the trace when running on the DSP
   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = SET_ERROR_ID;
#endif

   if (error != ERR_CLEAR) {
   	
   	*error_array &= ~CBI_NO_ERR; 

     // Determine which word to use and which bit to set
     word  = (error - 1) / LEN_WORD;
     shift = (error - 1) % LEN_WORD;

     // Create bit mask
     mask  = BIT << shift;

     if (word < CBI_MAX_ERROR_WORDS) *(error_array + word) |= mask;

   } else {

     // Clear error array
     for (word = 0; word < CBI_MAX_ERROR_WORDS; word++) {
       *(error_array + word) = 0;
     }
     
     *error_array |=  CBI_NO_ERR;  
   }

#if defined(SHARC_DSP)   
   // Decrement the trace when running on the DSP
   DSP_dsp_stat.num_levels--;
#endif

   return CBI_F_SUCCESS;
} 

// next routine is not needed on the DSP
//#if defined(CBI_CONTROL)
#if !defined(CBPM_INST_PLATFORM)
int cbi_get_errors(int* error_array, int *error_list, int num_error_codes) {

   int word;
   int bit;
   int n_errors = 0;
   int setbit;

//#if defined(SHARC_DSP)   
// Set our routine ID in the trace when running on the DSP
//   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = GET_ERRORS_ID;
//#endif
   int i;
   for (i = 0; i<num_error_codes; i++) {
   	  *(error_list+i) = 0;
   }

// If the NO_ERROR bit not set, then parse the error words for error
// flags
   if ((*error_array & BIT) == 0) {
      for (word = 0; word < CBI_MAX_ERROR_WORDS; word++) {
         for (bit = 0; bit < LEN_WORD; bit++) {
            setbit = (*(error_array + word) >> bit) & BIT;
            if (setbit) {
               *(error_list + n_errors++) = LEN_WORD * word + bit + 1;
            }
         }
      }
   }

//#if defined(SHARC_DSP)   
// Decrement the trace when running on the DSP
//   DSP_dsp_stat.num_levels--;
//#endif

   return n_errors;
}
//#endif
#endif



