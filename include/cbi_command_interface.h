#ifndef CBI_COMMAND_INTERFACE_H
#define CBI_COMMAND_INTERFACE_H
//-----------------------------------------------------------------------+
// File         :  cbi_command_interface.h                               |
//                                                                       |
// Description  :  Defines parameters which are used to synchronously    |
//                 initiate remote instrument actions by means of the    |
//                 9 bit commnad word encoded in the CESR turns marker   |
//                 signal.                                               |
//                                                                       |
// Author       :  M. Palmer                                             |
//-----------------------------------------------------------------------+

//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif


// Specification of the CBPM COMMAND node.  Command words are written into 
// element 1 of the "CBPM COMMAND" node.  Only the lowest 7 bits are used 
// by the system.
#define CBPM_COMMAND_NODE         "CBPM COMMAND"
#define CBPM_COMMAND_ELE          1


// The lowest 2 bits of the command word come from hardware trigger lines
#define HDW_TRIGGER_BITS 2


// Command bit 0 (DSP reads this out as the bit 2) - resets DSP turn counter 
#define SYNCHRONIZE_TURN_CTR      0x1
#define DSP_SYNCHRONIZE_TURN_CTR  (SYNCHRONIZE_TURN_CTR << HDW_TRIGGER_BITS)


// Command bit 1 (DSP reads this out as the bit 3) - enables collect_adc data 
// collection when DSPs are in "use_enable" mode 
#define COLLECT_ADC_ENABLE        0x2
#define DSP_COLLECT_ADC_ENABLE    (COLLECT_ADC_ENABLE << HDW_TRIGGER_BITS)


//-----------------------------------------------------------------------+
// The following provides for C++ compatibility (ie, C++ routines can    |
// explicitly use this include file).                                    |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
}
#endif



#endif  // CBI_COMMAND_INTERFACE_H


