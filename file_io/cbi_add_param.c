/*!
*-----------------------------------------------------------------------+
* File         :  cbi_add_param.c                                       |
*                                                                       |
* Description  : Populates a passed-in _array_ of PARAMETER_INFO        |
*                structures by APPENDING to the elements already in the |
*                array.  A suitable sentry field is used to determine   |
*                if the target structure has been filled already with a |
*                parameter name, the number of rows, and columns of     |
*                values to expect when reading the parameter from a     |
*                file, the data type of those values, and the memory    |
*                address of the structure in which to store the values  |
*                of the parameter when an 'faccess' call is performed   |
*                using the information stored here.                     |
*                                                                       |
* Warning, bounds checking is not performed in this function.           |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_add_param( PARAMETER_INFO *pars, char *name, int rows, int cols, int type, void *ptr ) {

  int   *iptr;
  float *fptr;
  char  *sptr;
  
  int i = 0;

  while ( pars[i].rows != 0 ) { 
    i++;
  }

  strcpy( pars[i].name, name );
  pars[i].rows    = rows;
  pars[i].cols    = cols;
  pars[i].type    = type;
  pars[i].pointer = (void *)ptr;

  return F_SUCCESS;
}
