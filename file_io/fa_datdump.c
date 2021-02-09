/*!
*-------------------------------------------------------------------------+
* File         :  fa_datdump.c                                            |
*                                                                         |
* Description  :  Dumps the contents of a given data structure            |
*                 (or portion thereof) to an open file pointer (disk file,|
*                 stdout, etc...) in a sensible way.                      |
*                                                                         |
*                                                                         |
* Arguments    :  Pointer to open file                                    |
*                 2-element array with range of Y (columns) values        |
*                 2-element array with range of X (columns) values        |
*                 Data type specifier                                     |
*                 Output format specifier string                          |
*                 Pointer to array of values to write to file             |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int fa_datdump( FILE *fp, int Yrange[], int Xrange[], int type, char *format, void *data ) {

  int x, y;
  // pointers for casting from void * type of data structure to dump
  int   *iptr;
  float *fptr;
  char  *sptr;


  if (type == INT || type == HEXINT) {
    iptr = (void *)(data);
    for (y = Yrange[0]; y < Yrange[1]; y++) {
      for (x = Xrange[0]; x < Xrange[1]; x++) {
	fprintf( fp, format, *(iptr + (y*Xrange[1]) + x) );
      }
      fprintf(fp, "\n");
    }
  }


  if (type == FLOAT) {
    fptr = (void *)(data);
    for (y = Yrange[0]; y < Yrange[1]; y++) {
      for (x = Xrange[0]; x < Xrange[1]; x++) {
	fprintf( fp, format, *(fptr + (y*Xrange[1]) + x) );
      }
      fprintf(fp, "\n");
    }
  }

  if (type == STRING) {
    sptr = (void *)(data);
    fprintf(fp, "%s\n", (sptr+0) );
  }


  return F_SUCCESS;

}



