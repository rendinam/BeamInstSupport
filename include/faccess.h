#ifndef FACCESS_H
#define FACCESS_H
/*!
*-----------------------------------------------------------------------+
* File         :  faccess.h                                             |
*                                                                       |
* Description  :  Constants header for the faccess file I/O routine.    |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

//-----------------------------------------------------------------------+
//  The following provides for C++ compatibility (ie, C++ routines can   |
//  explicitly use this include file).                                   |
//-----------------------------------------------------------------------+
#if defined (__cplusplus)
extern "C" {
#endif



//-------------------------------------
//   Boolean types
// FA_TRUE  
// FA_FALSE
//-------------------------------------
#define FA_TRUE    1
#define FA_FALSE   0

#define FA_READ    0
#define FA_WRITE   1

#define FA_MAX_FILE_SECTIONS   200

#define FA_MAX_STRING_LENGTH   100


enum FA_DATA_TYPES {
  INT    = 1, 
  FLOAT  = 2,
  STRING = 3,
  HEXINT = 4
};



extern int   faccess( int mode, FILE *fp, char *section, char *element, int Ydim, int Xdim, int type, void *recip );
extern int   extract_section_order( FILE *fp, char sections[][FA_MAX_STRING_LENGTH] );
extern int   fa_datdump( FILE *fp, int Yrange[], int Xrange[], int type, char *format, void *data );



#if defined (__cplusplus)
}
#endif



#endif //  FACCES_H
