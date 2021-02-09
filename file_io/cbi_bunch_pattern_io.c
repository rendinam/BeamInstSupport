/*!-----------------------------------------------------------------------+
* File         :  cbi_bunch_pattern_io.c                                  |
*                                                                         |
* Description  :  Several routines to facilitate loading and saving bunch |
*                 patterns to disk.                                       |
*                 - Prompt to get the category list of pre-saved bunch    |
*                   patterns available on disk, with descriptions, if     |
*                   desired.                                              |
*                 - Load the bunch pattern selected into place for the    |
*                 measurement about to be taken.                          |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_select_saved_pattern_cat( int pattern_array[] ) {

  char *func_name = (char *)__FUNCTION__;

  FILE *fp;
  char incoming_string[50] = {""};
  int retcode = 0;
  int count = 1, iter = 1;
  int input;
  int row, col;


  printf("\n\nAvailable bunch pattern categories:\n");
  fp = fopen( appconfig.bunpat_def_file, "r" );


  while ( retcode == 0 ) {
    strcpy( incoming_string, "" );
    retcode = faccess( FA_READ, fp, NULL, NULL, 1, 1, STRING, incoming_string );
    if ( strcmp(incoming_string, "") == 0 ) {
      break;
    }
    printf("%3d) %s\n", count, incoming_string );
    count++;
  }
  fclose(fp);


  fp = fopen( appconfig.bunpat_def_file, "r" );

  //-------------------------------------------------
  // Define the list of prompts and their parameters
  //-------------------------------------------------
  int retval;
  CBI_INIT_PROMPTS(1);

  int range[2];
  range[0] = 1;
  range[1] = count;

  prompts[0].supp_text_ptr    = NULL;
  prompts[0].prompt_text      = "Choose category  ";
  prompts[0].data_type        = CBI_INT;
  prompts[0].validator        = cbi_int_range_validator;
  prompts[0].valid_inputs     = range;
  prompts[0].default_value    = 1;
  prompts[0].destination      = &(input);
  prompts[0].last_val_default = CBI_TRUE;


  retval = CBI_PROMPTER();

  printf("input = %d\n", input);

  //printf("Choose category: ");
  //input = cbi_get_int();
  char category[30];
  for( iter = 0; iter < input; iter++) {
    strcpy( incoming_string, "" );
    retcode = faccess( FA_READ, fp, NULL, NULL, 1, 1, STRING, category );
  }
  fclose(fp);
  

  cbi_select_saved_pattern( (char*)category, pattern_array );


  return CBI_F_SUCCESS;

}






int cbi_select_saved_pattern( char *category, int pattern_array[] ) {

  char *func_name = (char *)__FUNCTION__;

  FILE *fp;
  int retcode = 0;
  int input;
  int row, col;

  fp = fopen( appconfig.bunpat_def_file, "r" );
  int BP_array[4][10];
  retcode = 0;
  printf("Category = %s\n", category);
  int element = 1;
  char pat_idx[5]     = {""};
  char comment_idx[5] = {""};
  char description[100];
  while( retcode == 0 ) {
    sprintf( comment_idx, "C%d", element );
    retcode = faccess( FA_READ, fp, category, comment_idx, 1, 1, STRING, description );
    if (retcode == 0) {
      printf("%2d)  %s\n", element, description);
      element++;
    }
  }
  
  //-------------------------------------------------
  // Define the list of prompts and their parameters
  //-------------------------------------------------
  int retval;
  CBI_INIT_PROMPTS(1);

  int range[2];
  range[0] = 1;
  range[1] = element;

  prompts[0].supp_text_ptr    = NULL;
  prompts[0].prompt_text      = "Choose pattern  ";
  prompts[0].data_type        = CBI_INT;
  prompts[0].validator        = cbi_int_range_validator;
  prompts[0].valid_inputs     = range;
  prompts[0].default_value    = 1;
  prompts[0].destination      = &(input);
  prompts[0].last_val_default = CBI_TRUE;


  retval = CBI_PROMPTER();

  printf("input = %d\n", input);

  ////printf("Select bunch pattern to load: ");
  ////input = cbi_get_int();

  printf("\n");

  // Load stored bunch pattern by index
  sprintf( pat_idx, "%d", input );
  retcode = faccess( FA_READ, fp, category, pat_idx, 4, 10, HEXINT, BP_array );
  fclose(fp);

  printf("\n Bunch Pattern Representation: \n");

  int i = 0;
  for ( row = 0; row < 4; row++) {
    for (col = 0; col < 10; col++) {
      printf("%x ", BP_array[row][col] );
      pattern_array[i] = BP_array[row][col];
      i++;
    }
    printf("\n");
  }

  printf("\n");


  return CBI_F_SUCCESS;

}





//-------------------------------------------------
// Gets the next index to use when saving a new
// bunch pattern to the bunch pattern definitions
// file.
//-------------------------------------------------
int cbi_new_pattern_number( char *BP_category ) {

  printf("BP_category = %s\n", BP_category);
  
  int retcode = -1;
  int pat_idx = 0;
  int BP_array[4][10];
  FILE *fp;
  char element[20];

  fp = fopen( appconfig.bunpat_def_file, "r" );

  while (retcode != 2) {
    pat_idx++;
    strcpy(element, "");
    sprintf(element, "%d", pat_idx);
    retcode = faccess( FA_READ, fp, BP_category, element, 4, 10, HEXINT, BP_array );
    //printf("element = %s,     retcode = %d\n", element, retcode);
  }

  fclose(fp);

  return pat_idx;

}




int cbi_save_bunch_pattern( int pattern_array[], char *BP_category, char *BP_name, int BP_entry_num ) {

  char *func_name = (char *)__FUNCTION__;

  int BP_array[4][10];
  int retcode, row, col, i = 0;
  int ele;
  FILE *fp;

  printf("     Category  = %s\n", BP_category);
  printf("     Entry num = %d\n", BP_entry_num);
  printf("     Name      = %s\n", BP_name);
  
  for ( row = 0; row < 4; row++) {
    for (col = 0; col < 10; col++) {
      BP_array[row][col] = pattern_array[i];
      printf("%x ", BP_array[row][col] );
      i++;
    }
    printf("\n");
  }
  printf("\n");

  char element[5];
  char tempname[50];


  printf("Writing to file...\n");


  fp = fopen( appconfig.bunpat_def_file, "r+" );

  printf("Searching for duplicate names in section...\n");
  for (ele = 1; ele < BP_entry_num; ele++) {
    strcpy(element, "");
    sprintf(element, "C%d", ele);
    // Test for existence of a pattern with the name about to be written
    // If so, return failure.  Duplicate names are not allowed.
    retcode = faccess( FA_READ, fp, BP_category, element, 1, 1, STRING, tempname );
    if (strcmp(tempname, BP_name) == 0) {
      printf("Found duplicate name\n");
      return 100; // FIXME: named constant?
    }
  }
  
  strcpy(element, "");
  sprintf(element, "C%d", BP_entry_num);
  printf("Element = %s\n", element);
  retcode = faccess( FA_WRITE, fp, BP_category, element, 1, 1, STRING, BP_name );
  if (retcode != 0) {
    return CBI_F_FAILURE;
  }

  strcpy(element, "");
  sprintf(element, "%d", BP_entry_num);
  printf("Element = %s\n", element);
  retcode = faccess( FA_WRITE, fp, BP_category, element, 4, 10, HEXINT, BP_array );
  if (retcode != 0) {
    return CBI_F_FAILURE;
  }

  printf("Pattern saved.\n");
  fflush(stdout);

  fclose(fp);

  return CBI_F_SUCCESS;

}




/* int cbi_select_saved_pattern( int pattern_array[] ) { */

/*   char *func_name = (char *)__FUNCTION__; */

/*   FILE *fp; */
/*   char incoming_string[50] = {""}; */
/*   int retcode = 0; */
/*   int count = 1, iter = 1; */
/*   int input; */
/*   int row, col; */


/*   printf("\n\nAvailable bunch pattern categories:\n"); */
/*   fp = fopen( appconfig.bunpat_def_file, "r" ); */


/*   while ( retcode == 0 ) { */
/*     strcpy( incoming_string, "" ); */
/*     retcode = faccess( FA_READ, fp, NULL, NULL, 1, 1, STRING, incoming_string ); */
/*     if ( strcmp(incoming_string, "") == 0 ) { */
/*       break; */
/*     } */
/*     printf("%3d) %s\n", count, incoming_string ); */
/*     count++; */
/*   } */
/*   fclose(fp); */


/*   fp = fopen( appconfig.bunpat_def_file, "r" ); */
/*   printf("Choose category: "); */
/*   input = cbi_get_int(); */
/*   char category[30]; */
/*   for( iter = 0; iter < input; iter++) { */
/*     strcpy( incoming_string, "" ); */
/*     retcode = faccess( FA_READ, fp, NULL, NULL, 1, 1, STRING, category ); */
/*   } */
/*   fclose(fp); */
  

/*   fp = fopen( appconfig.bunpat_def_file, "r" ); */
/*   int BP_array[4][10]; */
/*   retcode = 0; */
/*   printf("Category = %s\n", category); */
/*   int element = 1; */
/*   char pat_idx[5]     = {""}; */
/*   char comment_idx[5] = {""}; */
/*   char description[100]; */
/*   while( retcode == 0 ) { */
/*     sprintf( comment_idx, "C%d", element ); */
/*     retcode = faccess( FA_READ, fp, category, comment_idx, 1, 1, STRING, description ); */
/*     if (retcode == 0) { */
/*       printf("%2d)  %s\n", element, description); */
/*       element++; */
/*     } */
/*   } */
  
/*   printf("Select bunch pattern to load: "); */
/*   input = cbi_get_int(); */

/*   printf("\n"); */

/*   // Load stored bunch pattern by index */
/*   sprintf( pat_idx, "%d", input ); */
/*   retcode = faccess( FA_READ, fp, category, pat_idx, 4, 10, HEXINT, BP_array ); */
/*   fclose(fp); */

/*   printf("\n Bunch Pattern Representation: \n"); */

/*   int i = 0; */
/*   for ( row = 0; row < 4; row++) { */
/*     for (col = 0; col < 10; col++) { */
/*       printf("%x ", BP_array[row][col] ); */
/*       pattern_array[i] = BP_array[row][col]; */
/*       i++; */
/*     } */
/*     printf("\n"); */
/*   } */

/*   printf("\n"); */


/*   return CBI_F_SUCCESS; */

/* } */


/* //------------------------------------------------- */
/* // Gets the next index to use when saving a new */
/* // bunch pattern to the bunch pattern definitions */
/* // file. */
/* //------------------------------------------------- */
/* int cbi_new_pattern_number( char *BP_category ) { */

/*   printf("BP_category = %s\n", BP_category); */
  
/*   int retcode = -1; */
/*   int pat_idx = 0; */
/*   int BP_array[4][10]; */
/*   FILE *fp; */
/*   char element[20]; */

/*   fp = fopen( appconfig.bunpat_def_file, "r" ); */

/*   while (retcode != 2) { */
/*     pat_idx++; */
/*     strcpy(element, ""); */
/*     sprintf(element, "%d", pat_idx); */
/*     retcode = faccess( FA_READ, fp, BP_category, element, 4, 10, HEXINT, BP_array ); */
/*     //printf("element = %s,     retcode = %d\n", element, retcode); */
/*   } */

/*   fclose(fp); */

/*   return pat_idx; */

/* } */




/* int cbi_save_bunch_pattern( int pattern_array[], char *BP_category, char *BP_name, int BP_entry_num ) { */

/*   char *func_name = (char *)__FUNCTION__; */

/*   int BP_array[4][10]; */
/*   int retcode, row, col, i = 0; */
/*   int ele; */
/*   FILE *fp; */

/*   printf("     Category  = %s\n", BP_category); */
/*   printf("     Entry num = %d\n", BP_entry_num); */
/*   printf("     Name      = %s\n", BP_name); */
  
/*   for ( row = 0; row < 4; row++) { */
/*     for (col = 0; col < 10; col++) { */
/*       BP_array[row][col] = pattern_array[i]; */
/*       printf("%x ", BP_array[row][col] ); */
/*       i++; */
/*     } */
/*     printf("\n"); */
/*   } */
/*   printf("\n"); */

/*   char element[5]; */
/*   char tempname[50]; */


/*   printf("Writing to file...\n"); */


/*   fp = fopen( appconfig.bunpat_def_file, "r+" ); */

/*   printf("Searching for duplicate names in section...\n"); */
/*   for (ele = 1; ele < BP_entry_num; ele++) { */
/*     strcpy(element, ""); */
/*     sprintf(element, "C%d", ele); */
/*     // Test for existence of a pattern with the name about to be written */
/*     // If so, return failure.  Duplicate names are not allowed. */
/*     retcode = faccess( FA_READ, fp, BP_category, element, 1, 1, STRING, tempname ); */
/*     if (strcmp(tempname, BP_name) == 0) { */
/*       printf("Found duplicate name\n"); */
/*       return 100; // FIXME: named constant? */
/*     } */
/*   } */
  
/*   strcpy(element, ""); */
/*   sprintf(element, "C%d", BP_entry_num); */
/*   printf("Element = %s\n", element); */
/*   retcode = faccess( FA_WRITE, fp, BP_category, element, 1, 1, STRING, BP_name ); */
/*   if (retcode != 0) { */
/*     return CBI_F_FAILURE; */
/*   } */

/*   strcpy(element, ""); */
/*   sprintf(element, "%d", BP_entry_num); */
/*   printf("Element = %s\n", element); */
/*   retcode = faccess( FA_WRITE, fp, BP_category, element, 4, 10, HEXINT, BP_array ); */
/*   if (retcode != 0) { */
/*     return CBI_F_FAILURE; */
/*   } */

/*   printf("Pattern saved.\n"); */
/*   fflush(stdout); */

/*   fclose(fp); */

/*   return CBI_F_SUCCESS; */

/* } */
