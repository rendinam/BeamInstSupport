//-----------------------------------------------------------------------+
// File         :  cbi_get_bid.c                                         |
//                                                                       |
// Description  :  This file contains the functions -                    |
//                 bis_get_bid,                                          |
//                 bis_bid2species, bis_bid2train, bis_bid2bunch         |
//                                                                       |
// Prototypes   :  int bis_get_bid(int, int, int)                        |
//                 int bis_bid2species(int)                              |
//                 int bis_bid2train(int)                                |
//                 int bis_bid2bunch(int)                                |
//                                                                       |
// Arguments    :  bis_get_bid    - species, train, and bunch            |
//                 bis_bid2xxxxxx - bunch_id                             |
//                                                                       |
// Return Values:  bis_get_bid     - bunch_id                            |
//                 bis_bid2species - species                             |
//                 bis_bid2train   - train                               |
//                 bis_bid2bunch   - bunch                               |
//                                                                       |
// Author       :  M. Palmer                                             |
//                                                                       |
// Modifications:  Taken from cbpm_get_bid.c - 4/9/2009                  |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// Include Files -                                                       |
//                                                                       |
//   NOTE - By convention, include files should not specify a PATH for   |
//          ready portability to non-UNIX platforms.                     |
//                                                                       |
// CESR includes   -                                                     |
//                                                                       |
// CESR_platform.h     -  CESR processor/system configuration file.      |
//                                                                       |
// Local includes  -                                                     |
//-----------------------------------------------------------------------+
//#include "CESR_platform.h"
//#include "cbi_CO_utils.h"

#include "cbi_core_includes.h"

/* #if defined(SHARC_DSP)    */
/* // Need to include DSP specific .h files */
/* #include "cbpm_CO_types.h" */
/* #include "cbi_PT_system.h" */
/* #endif */

//-----------------------------------------------------------------------+
// Definitions -                                                         |
//                                                                       |
// SPECIAL NOTE (4/9/2009) SPECIES_SHIFT needs to be changed to 24       |
//                         TRAIN_SHIFT   needs to be changed to 16       |
//                         for full support of new CESR bunch-train      |
//                         options (ie, single long trains with 4ns      |
//                         bunch spacing)                                |
//                                                                       |
//-----------------------------------------------------------------------+
#define SPECIES_SHIFT 16
#define TRAIN_SHIFT    8
#define BUNCH_SHIFT    0
#define BYTE 0xFF

int cbi_get_bid(int species, int train, int bunch) {

   int word = 0;

#if defined(SHARC_DSP)   
// Set our routine ID in the trace when running on the DSP
   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = GET_BID_ID;
#endif

// Store species, train, and bunch in individual bytes of the bunch_id
   word  = species << SPECIES_SHIFT;
   word |= train   << TRAIN_SHIFT;
   word |= bunch   << BUNCH_SHIFT;


#if defined(SHARC_DSP)   
// Decrement the trace when running on the DSP
   DSP_dsp_stat.num_levels--;
#endif

   return word;
} 


//-------------------------------------
//-------------------------------------
int cbi_bid2species(int bunch_id) {

   int species;

#if defined(SHARC_DSP)   
// Set our routine ID in the trace when running on the DSP
   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = BID2SPECIES_ID;
#endif


   species = BYTE & (bunch_id >> SPECIES_SHIFT);


#if defined(SHARC_DSP)   
// Decrement the trace when running on the DSP
   DSP_dsp_stat.num_levels--;
#endif

   return species;
}


//-------------------------------------
//-------------------------------------
int cbi_bid2train(int bunch_id) {

   int train;

#if defined(SHARC_DSP)   
// Set our routine ID in the trace when running on the DSP
   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = BID2TRAIN_ID;
#endif


   train = BYTE & (bunch_id >> TRAIN_SHIFT);


#if defined(SHARC_DSP)   
// Decrement the trace when running on the DSP
   DSP_dsp_stat.num_levels--;
#endif

   return train;
}



//-------------------------------------
//-------------------------------------
int cbi_bid2bunch(int bunch_id) {

   int bunch;

#if defined(SHARC_DSP)   
// Set our routine ID in the trace when running on the DSP
   DSP_dsp_stat.trace[DSP_dsp_stat.num_levels++] = BID2BUNCH_ID;
#endif


   bunch = BYTE & (bunch_id >> BUNCH_SHIFT);


#if defined(SHARC_DSP)   
// Decrement the trace when running on the DSP
   DSP_dsp_stat.num_levels--;
#endif

   return bunch;
}





