/*!
*-------------------------------------------------------------------------+
* File         :  cbi_compare_xbus_and_ethernet.c                         |
*                                                                         |
* Description  :  Compares the values of a struct in two copies           |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Stedinger                                            |
*-------------------------------------------------------------------------+*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "utilities.h"
#include "messages.h"

#include "cbi_core_includes.h"

int compare_Arrays(int* Array1, int* Array2, int numElements) {
	
	int word;
	int same = 1;

	for ( word = 0; word < numElements; word++) {

	same = same && (*Array1 == *Array2);
	if (!same) {
		break;
	}
		Array1++;
		Array2++;

    } //endFOR word
    
    return same;
}

int isArrayAllZeros(int* Array1, int numElements) {
	
	int word;
	int allZeros = 1;

	for ( word = 0; word < numElements; word++) {

	allZeros = allZeros && (*Array1 == 0);
	if (!allZeros) {
		break;
	}
		Array1++;

    } //endFOR word
    
    return allZeros;
}

int cbi_compare_xbus_and_ethernet(FILE *fp, int iidx, int struct_tag, int verboseTypeout, int* same, int* xbusAllZeros, int* ethernetAllZeros, int(*fptr)(FILE*,int), int* XBusEnabled ) {
	
    int debug_level = CTL_System.debug_verbosity;	
	
	#define CBPM_MAX_COMPARE_WORDS	10000
	int struct_size, struct_numwords;
	int status;
	int XBusArray[CBPM_MAX_COMPARE_WORDS];
	int EthernetArray[CBPM_MAX_COMPARE_WORDS];
	*same = 1;
	*ethernetAllZeros = 1;
	*xbusAllZeros = 1;

	int *ptr_orig, *ptr_Xbus, *ptr_Ethernet;
	 const COMM_KEY         *p_key_src, *p_key_dest;

	//Setup variables
	p_key_src = &((CTL_System.p_Module[iidx])->key[ (CTL_System.p_Module[iidx])->tag2key_map[struct_tag]]);
	ptr_orig  = p_key_src->data.ptr[0];
	struct_numwords   = cbi_compute_comm_struct_size( p_key_src->p_config );
	struct_size =  struct_numwords*4; 	

	//Make sure structure size is allowed
	if (struct_size == 0) {
		fprintf(fp,"Error struct_size is 0, %i", status);
		return status;
	}  
	else if (struct_numwords > CBPM_MAX_COMPARE_WORDS)
	{
		fprintf(fp,"Error struct_size is %i which is greater than %i the max, %i", struct_numwords, CBPM_MAX_COMPARE_WORDS, status);
		return status;
	}

	//ETHERNET	
	status = cbi_gp_struct(READ, ETHERNET, struct_tag, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
	if (status != 0) {
		fprintf(fp,"Error in READ,  ETHERNET, %i, %i\n", struct_tag, status);
		return status;
	}
	memcpy(EthernetArray, ptr_orig,struct_size);

	//XBUS
	if (*XBusEnabled) {
		status = cbi_gp_struct(READ,  XBUS, struct_tag, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
		if (status == 6) {
			*XBusEnabled = 0;
			status = 0;
		}
		if (status != 0) {
			fprintf(fp,"Error in READ,  XBUS, %i, %i\n", struct_tag, status);
			return status;
		}
		memcpy(XBusArray, ptr_orig,struct_size);
		*same = compare_Arrays(XBusArray,EthernetArray,struct_numwords);

		if (verboseTypeout || !(*same)) {
			fprintf(fp,"\n**********************************\nXbus dump: \n");
			status = (*fptr) ( fp, iidx );
		}
		*xbusAllZeros = isArrayAllZeros(XBusArray,struct_numwords);
		
		if (!(*same)) {
			printf("\nXbus and ethernet read of structure not the same.\n");
			printf("**********************************\nEthernet dump: \n");
			status = cbi_gp_struct(READ,  ETHERNET, struct_tag, CTL_System.p_Module[iidx], CBI_FULL_STRUCT);
	
			status = (*fptr) ( stdout, iidx );
	
			printf("\n**********************************\n\n");						
		}
	}

	*ethernetAllZeros = isArrayAllZeros(EthernetArray,struct_numwords);
	
	return status;
}
