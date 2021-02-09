#ifndef CBI_CONV_PROTOS_H
#define CBI_CONV_PROTOS_H
//-----------------------------------------------------------------------+
// File         :  cbi_conv_protos.h                                     |
//                                                                       |
// Description  :  Prototypes for functions used to convert fundamental  |
//                 data types between different internal representations.|
//-----------------------------------------------------------------------+


int *cbi_float_convert(int, int*, int, int, int, void*);
int *cbi_int_convert  (int, int*, int, int, int, void*);

// Relocate or deprecate this.
int  cbi_struct_io(int, void *, int);


#endif //CBI_CONV_PROTOS_H
