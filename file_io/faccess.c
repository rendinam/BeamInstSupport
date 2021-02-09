/*!
*-----------------------------------------------------------------------+
* File         :  faccess.c                                             |
*                                                                       |
* Description  :  Allows reading of one or more elements from a         |
*                 config-style (sections and elements style) text file  |
*                 into a target array provided via pointer reference.   |
*                                                                       |
*                 Allows the flexible insertion of array contents into  |
*                 the same type of file and association of the data     |
*                 with a section and element name.                      |
*                                                                       |
*                 The supported text file format (config-style) is      |
*                 defined to be a file divided into sections, each      |
*                 setion having some number of elements.  Some number   |
*                 of value fields are associated with each element, are |
*                 whitespace delimited on a single line, and can span   |
*                 multiple lines of values provided the number of       |
*                 values within a given element is the same on each     |
*                 line of that element.                                 |
*                                                                       |
* Dependencies :  faccess.h                                             |
*                 fa_datdump.c                                          |
*                                                                       |
* Arguments    : -Pointer to open file handle                           |
*                -Pointer to section name string (with delimiter chars) |
*                -Pointer to element name string                        |
*                -Number of rows of values found in the element         |
*                -Number of columns of values found in the element      |
*                -Data type code of values found in element             |
*                -Pointer to data structure that will receive the       |
*                 element values requested from the file.               |
*                                                                       |
* Behavior     :  If an empty element string is provided as an input    |
*                 argument, the function will run in 'discover' mode.   |
*                 It will return the first section name on this call    |
*                 and will continue returning subsequently found        |
*                 section names on subsequent calls until no more       |
*                 section names are encountered.  The function will     |
*                 then return a code of '1' for 'Section not found'.    |
*                 Example:                                              |
*     faccess( FA_READ, fp, NULL, NULL, NULL, NULL, STRING, &string )   |
*                 grabs the first/next section name from the file,      |
*                 storing it in the array 'string'.                     |
*                                                                       |
*                 TODO: Element discover mode.                          |
*                                                                       |
* Return Codes :                                                        |
*                           FA_READ / FA_WRITE mode                     |
*                 -1             - File not found                       |
*                  0             - Access completed successfully        |
*                                                                       |
*                           FA_READ mode only                           |
*                  0             - Access completed successfully        | 
*                  1             - Section not found                    |
*                  2             - Section was found but does not       |
*                                  contain element                      |
*                  3             - Successful read, but obtained fewer  |
*                                  values than expected.                |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>  
#include <errno.h>
#include <unistd.h>

//TODO:  extern a configuration struct here for controlling how faccess operates during runtime.

int   replace( char *string, char old, char new); // Replace all instances of char in a string.
char  *replace_str(char *st, char *orig, char *repl);
int   strip( char *string );
int spaces( FILE *fp, int padding );

#include "faccess.h"

#define debug_mode    FA_FALSE


// Support function defined below main function
int retrieve_payload( char result[], int type, int yctr, int xctr, int Xdim, void *recip, int count );


#define ELEMENT_NAME_WIDTH     36
#define MAX_FILE_LINE_LENGTH   2048


int faccess( int mode, FILE *fp, char *section_p, char *element_p, int Ydim, int Xdim, int type, void *recip ) {


  //int section_open = FALSE;
  static int section_open = FA_FALSE;

  // Static temp pointer to allow a single temp file opening
  // per session.
  static FILE *tfp;

  int starting_pos = ftell(fp);

  int string_count = 0;

  // Whether or not faccess is being operated in 'session' mode or
  // single-access mode.
  // Providing a target section AND a NULL recipient structure signifies a session start.
  int *recip_ptr = (void *)(recip); // Attach to recip pointer to allow examination.
  int session_opening = FA_FALSE;

  char c;
  char *temp_filename = "FA_temporary_file";

  //--------------------------------
  // Test for session mode closing.
  //--------------------------------
  if ( (section_p == NULL) && (recip_ptr == NULL) ) {
    //printf("FACCESS- -- Leaving SESSION MODE...\n\n");

    if (mode == FA_WRITE) {
      // Write out remaining F->T, close all, delete temp.
      while ( (c = fgetc(fp)) != EOF ) {
	fputc( c, tfp );
      }
      rewind(fp);
      rewind(tfp); 
      
      // clear/remove destination file  
      ftruncate((int) fp, 0);  // FIXME: warning on file descriptor vs file pointer
      
      // Reset file positions to beginning
      rewind(fp);
      if (fclose(tfp) != 0) {
	perror("fclose: ");
      }
      
      // Re-open as read
      tfp = fopen(temp_filename, "r");
      
      // Transfer file contents.
      while ( (c = fgetc(tfp)) != EOF ) {
	fputc( c, fp );
      }
      if (fclose(tfp) != 0) {
	perror("fclose :");
      }
      if (debug_mode == FA_TRUE) {
	printf("SESSION FA_WRITE - removing temp file\n");
      }
      remove(temp_filename);
    }

    // Rewind operational file in preparation for a 
    // potential new standalone call or session opening.
    rewind(fp);
    section_open = FA_FALSE;

    return 0;

  } // endIF section_p / recip_ptr


  //--------------------------------
  // Test for session mode opening.
  //--------------------------------
  if ( (section_p != NULL) && (recip_ptr == NULL) ) {
    //printf("Processing SESSION open or SECTION CLOSE\n");
   
    // If another session-opening call is made
    // while a given section is open, this will
    // close the SECTION, leaving the session open,
    // so that a new section may be provided by a
    // fresh opening call of the same form.
    if (section_open == FA_TRUE) {
      section_open = FA_FALSE;
      return 0;
    } 

    if (starting_pos == 0) {
      session_opening = FA_TRUE;
      //printf("\n\nFACCESS -- Entering SESSION MODE...\n");
    }

  }


  char line[MAX_FILE_LINE_LENGTH];
  char temp[MAX_FILE_LINE_LENGTH] = "";
  int length;     // Length of pre-stripped line
  int len;        // length of stripped line
  int elenamelen; // Length of stripped element name
  
  int xctr = 0;
  int yctr = 0;

  char COMMENT_CHAR  = '#';
  char delims[]      = " ";
  char *sect_char    = "[";
  char *sect_char2   = "]";
  char *elem_char    = ".";
  char *blank        = " ";
  char *lf_char      = "\n";
  char *comment_char = "#";
  char *empty_char   = "";
  char *result       = NULL;
  int line_num       = 0;


  int returncode = 2;

  // Create local working copies of the section and element
  // names so that they may be modified, in strip() etc, as
  // necessary.
  char section[FA_MAX_STRING_LENGTH] = {"\0"};
  char element[FA_MAX_STRING_LENGTH] = {"\0"};

  // Decorate the section and element names for token matching.
  // The decorations used are defined in the list of character
  // constants above.
  if (section_p != NULL) {
    sprintf( section, sect_char );
    strcat( section, section_p );
    strcat( section, sect_char2 );
  }
  if (element_p != NULL) {
    sprintf( element, elem_char );
    strcat( element, element_p );
  }

  int new_element_line  = FA_FALSE;
  int tot_vals          = Ydim * Xdim;
  int vals_read         = 0;

  // Temporary holding variables for values read in
  int   ipayload;
  float fpayload;
  char  spayload[FA_MAX_STRING_LENGTH];

  int element_found = FA_FALSE;

  // pointers for casting from void * type of I/O data structure
  char  format[20];
  char  fmt_temp[5];
  int   requested_field_width;
  int   float_precision;

  // Compose the formatting specifiers for the requested data type
  if (type == HEXINT) {
    requested_field_width = 7;
    sprintf( format, "%% -" );  // Left-justify
    sprintf( fmt_temp, "%xx", requested_field_width );
    strcat( format, fmt_temp );
  }
  if (type == INT) {
    requested_field_width = 7;
    sprintf( format, "%% -" );  // Left-justify
    sprintf( fmt_temp, "%dd", requested_field_width );
    strcat( format, fmt_temp );
  }
  if (type == FLOAT) {
    requested_field_width = 13;
    float_precision       = 5;
    sprintf( format, "%% -" );  // Left-justify
    sprintf( fmt_temp, "%d.", requested_field_width );
    strcat( format, fmt_temp );
    strcpy( fmt_temp, "" );
    sprintf( fmt_temp, "%df", float_precision);
    strcat( format, fmt_temp ); 
  }
  if (type == STRING) {
    requested_field_width = 12;
    strcpy( format, "%s" );
  }

  int Xrange[2];
  int Yrange[2];

  int i; // generic counter
  float max_field_width;
  int elem_name_pad, value_pad, val_line_pad;
  int charcount;
  int poscnt, pos1, pos2, section_pos, posA, posB;
  int indent = 0;


  // For writing
  char pad[50];

  int midfile = FA_FALSE;

  elenamelen = strlen(element);

  if (fp == (int)NULL ) {

    printf("\n[fr] File not found!\n\n");
    return(-1);
    
  }


  //------------------------------
  // Line-by-line parsing loop:
  //------------------------------
  while (fgets(line, MAX_FILE_LINE_LENGTH, fp) != NULL) { 

    line_num++;
    length = strlen(line);
    len    = strip(line); // Chop off leading and trailing whitespace.


    // For each file line that is not empty and is not a comment
    // I.e. skip comment lines and blank lines for processing.
    //-----------------------------------------------------------
    if ( line[0] == COMMENT_CHAR || line[0] == (int)NULL ) {
      continue;
    }
    
    // Look for element in section.  If Y dimention (rows) > 1
    // flag as an open element to allow acquisition of multiple
    // lines of values.
    //-----------------------------------------------------------
    if (section_open == FA_TRUE) {


      // If another section is found here, exit
      // line-reading loop.  Section does not
      // contain the necessary element, process
      // this case below.
      if ( strncmp(line, sect_char, 1) == 0 ) {
	if (debug_mode) {
	  printf("Section open and new section found!\n");
	}
	// Slide to beginning of line, also inject a LF char when
	// inserting the new element
	fseek(fp, -length, SEEK_CUR);
	pos1 = ftell(fp);
	pos2 = pos1;
	if (debug_mode) {
	  printf("sec pos1 = %d\n", pos1);
	}
	midfile = FA_TRUE;
	break;
      }

      
      if ( strncmp(line, elem_char, 1) == 0 ) {
	if (debug_mode) {
	  printf("An element was encountered... -%s-\n", line);
	  printf("Looking for : %s\n", element_p);
	}
	// Grab first token on line, see if it's the
	// requested element name.
	result = strtok( line, delims );
	
	if ( strcmp(element, result) == 0 ) {
	  if (debug_mode) {
	    printf("ELEMENT FOUND: %s\n", element);
	  }
	  returncode = 0;
	  // Read/Write switch here
	  // If FA_READ,  just extract the info at this point.
	  // If FA_WRITE, set snip marker before element. 
	  //           If in a session, a copy will occur later from F-->T 
	  //           from starting_pos-->pos1 set snip marker after element
	  if (mode == FA_READ) {
	    for (yctr = 0; yctr < Ydim; yctr++) {
	      for (xctr = 0; xctr < Xdim; xctr++) {
		if (yctr == 0 && xctr == 0) {
		  result = strtok( NULL, delims );
		} else if (yctr != 0 && xctr == 0) {
		  result = strtok( line, delims );
		} else if ( xctr != 0 ) {
		  result = strtok( NULL, delims );
		}
		// TODO: If result == NULL, There were fewer tokens on the
		//       line than expected, stop filling the output array(s)
		//       and return, leaving unvisited elements of the array to
		//       fill with the values they had upon first entry to this
		//       function.
		//       Signify this condition with a specific return val?
		if (result == NULL) {
		  section_open = FA_FALSE;
		  return 3;
		}

		strip(result);
		retrieve_payload( result, type, yctr, xctr, Xdim, recip, string_count );
		vals_read++;
		if (type == STRING) {
		  string_count++;
		}
		// If all expected values have been retrieved, we're done.
		if (vals_read == tot_vals) { 
		  // Reset the file pointer back to the beginning of the file
		  // to leave it ready for any subsequent calls to this function
		  // operating on the same preexisting file pointer.
		  if (starting_pos == 0) {
		    rewind(fp);
		    section_open = FA_FALSE;
		  } 
		  // Section found, element found, FA_READ completed.
		  return 0;
		}
	      } // ENDfor xctr in Xdim
	      // Line processed, get next one.
	      fgets(line, MAX_FILE_LINE_LENGTH, fp);
	      strip(line);
	    } //ENDfor yctr in Ydim
	  } else { // mode == FA_WRITE  element already exists, replacement write
	    midfile = FA_TRUE;
	    fseek(fp, -length, SEEK_CUR);
	    pos1 = ftell(fp);
	    if (debug_mode) {
	      printf("ele pos1 = %d\n", pos1);
	    }
	    int accum = 0;
	    // Scan until next element, section, blank line, comment line, or EOF 
	    // to get position of other snip.
	    fgets(line, MAX_FILE_LINE_LENGTH, fp);
	    if (debug_mode) {
	      printf("FIRST: %s\n", line);
	    }
	    accum += strip(line);
	    while (fgets(line, MAX_FILE_LINE_LENGTH, fp) != NULL) {
	      length = strip(line);
	      if (  (strncmp(line, sect_char,  1) == 0) || (strncmp(line, elem_char,    1) == 0)   ||
		    (strncmp(line, empty_char, 1) == 0) || (strncmp(line, comment_char, 1) == 0) )  {
		break;
	      }
	      accum += length;
	    }
	    if (debug_mode) {
	      printf("LAST : %s\n", line);
	    }
	    pos2 = pos1 + accum;

	    break;



	  } // endIF mode
	  
	  
	} // ENDif requested element found
      } // ENDif any element found
      
      
    } //endIF section_open
    
    
    //-----------------------------------------------------------
    // Look for section in file.  Flag as open if found.
    //-----------------------------------------------------------
    // If line holds a section label...
    if ( strncmp(line, sect_char, 1) == 0 ) {
      if (debug_mode) {
	printf("A section was encountered...\n");
      }

      result = strtok( line, delims );

      ////////////////////////////////////
      // If in "section-discover" mode.
      // (i.e. FA_READ mode with no section name input.),
      // return the adorned section name and exit with
      // success.  If no additional section names are
      // found, return with section failure code (1)
      // below.
      if ( section_p == NULL && recip_ptr != NULL ) {
	// Strip off decoration characters before returning name.
	strcpy( temp, result);
	replace(temp, (char)(*sect_char), ' ');
	replace(temp, (char)(*sect_char2), ' ');
	strip(temp);
	strcpy(recip, temp);
	section_open = FA_FALSE;
	return 0;
      }
      ////////////////////////////////////

      if (section_open == FA_FALSE && strcmp(section, result) == 0) {
	if (debug_mode) {
	  printf("Section open: %s\n", section);
	}
	section_open = FA_TRUE;

	// If this is a faccess session-opening invocation,
	// make file location the beginning of the line
	// after the line containing the target section's name.
	// and open a new temporary file for appending.
	if (session_opening == FA_TRUE && recip_ptr == NULL) {
	  int presect_pos = ftell(fp);

	  if (mode == FA_WRITE) { ////////// new 25-Feb-2010
	    // Create working file
	    if (debug_mode == FA_TRUE) {
	      printf("Opening temporary file: %s\n", temp_filename);
	    }
	    tfp = fopen(temp_filename, "a");
	    
	    fseek( fp, 0, SEEK_SET );
	    for (poscnt = 0; poscnt < presect_pos; poscnt++) {
	      c = fgetc(fp);
	      fprintf( tfp, "%c", c );
	    }
	    fflush(tfp);
	  } /////////////// / new 25-Feb-2010

	  fseek( fp, presect_pos, SEEK_SET );

	  return 0;
	}

	returncode  = 1;
	continue;
      }
    }   
    
  } // endWHILE lines in file------------------------------------------



  
  // The next section was found when searching the
  // target section, -OR- the end of the file was
  // reached.  In either case, the requested section 
  // did not contain the necessary element.  
  // Inject the element now for FA_WRITE mode or rewind 
  // and return with failure code for FA_READ mode.
  if (section_open == FA_TRUE) {

    if (mode == FA_READ) {
      if (debug_mode) {
	printf("MODE(%d): Section (%s) does not contain element (%s).\n", mode, section, element);
      }
      returncode = 2;
      rewind(fp);
      section_open = FA_FALSE;
      return returncode;
    }
      
    // Create working file if this is a standalone mode call,
    // otherwise the temp file has already been created as part 
    // of a session and tfp is available for use.
    if (starting_pos == 0) {
      tfp = fopen(temp_filename, "a");
      if (tfp == NULL) {
	perror("ERROR opening temp file with fopen()");
      }
    }
    
    // Copy original file to working file up to pos1 position
    if (midfile == FA_FALSE) { // If insertion needs to happen at END of file,
      pos1 = ftell(fp);     // pos1 is the same as pos2, otherwise, these have
      pos2 = pos1;          // already been set correctly up above.
    }
    if (debug_mode) {
      printf("pos1 = %d\n", pos1);
      printf("pos2 = %d\n", pos2);
    }
    fseek( fp, starting_pos, SEEK_SET ); ///////////////////////// examine
    if (debug_mode) {
      printf("starting_pos = %d\n", starting_pos);
    }
    fseek( tfp, 0, SEEK_END );

    for (poscnt = starting_pos; poscnt < pos1; poscnt++) {
      c = fgetc(fp);
      fprintf( tfp, "%c", c );
    }
    fflush(tfp);

    // First-stage copy complete

    //--------------------------------------------------------
    // Perform insertion of data and appending of remainder
    // of file if not in session mode.
    //--------------------------------------------------------
    if (debug_mode) {
      printf("Pre element write session check... session_opening = %d\n", session_opening);
    }

    // Skip any insertion if this is a session-opening call
    if (session_opening == FA_FALSE) { 
      // Insert the new element into the working file
      elenamelen    = strip(element);
      elem_name_pad = ELEMENT_NAME_WIDTH - elenamelen;
      if (elem_name_pad < 0) elem_name_pad = 0;
      
      value_pad     = requested_field_width;
      val_line_pad  = indent + elenamelen + elem_name_pad;
      
      // Output element name and values that belong on the same line.
      if (debug_mode) {
	printf("Inserting new element...\"%s\"\n", element);
      }
      spaces(tfp, indent);
      fprintf(tfp, "%s", element);
      spaces(tfp, elem_name_pad);
      
      
      // Calc ranges for fa_datdump call
      Yrange[0] = 0;
      Yrange[1] = 1;
      Xrange[0] = 0;
      Xrange[1] = Xdim;
      fa_datdump( tfp, Yrange, Xrange, type, format, recip );

      // Output remaining rows
      for (yctr = 1; yctr < Ydim; yctr++) {
	// Pad space for lines that appear beneath element name
	spaces(tfp, val_line_pad);
	Yrange[0] = yctr;
	Yrange[1] = yctr+1;
	Xrange[0] = 0;
	Xrange[1] = Xdim;
	fa_datdump( tfp, Yrange, Xrange, type, format, recip );
      }

      // Jump to point in original file immediately after 
      // inserted block.
      fseek( fp, pos2, SEEK_SET );

      // If this is a call within a session, exit now,
      // preserving source and destination file locations.
      if (starting_pos != 0) { 
	fseek( tfp, 0, SEEK_END); // testing
	return 0;
      }

      // Restore the rest of the original.  Only reaches
      // this point if it is not a call within a session
      // (standalone call).
      while ( (c = fgetc(fp)) != EOF ) {
	fputc( c, tfp );
      }
      rewind(fp);
      rewind(tfp); 

      // clear/remove destination file  
      ftruncate((int) fp, 0);  // FIXME: warning on file descriptor vs file pointer

      // Reset file positions to beginning
      rewind(fp);
      if (fclose(tfp) != 0) {
	perror("fclose: ");
      }

      
      // Re-open as read
      tfp = fopen(temp_filename, "r");


      // Transfer file contents.
      while ( (c = fgetc(tfp)) != EOF ) {
	fputc( c, fp );
      }
      
      // Close temporary working file
      fclose(tfp);

    } //endIF != session


    if (session_opening == FA_FALSE) {
      remove(temp_filename);
      rewind(fp);
    }
    
    //// Standalone call Success
    section_open = FA_FALSE;
    return 0;

  } // endIF section_open

  


  //-------------------------------------------------------
  // If we make it this far, the requested section 
  // was not found. If this was a FA_WRITE request,
  // append the requested section name to the end of file.
  // Otherwise, return with appropriate failure code for
  // FA_READ attempt.
  //-------------------------------------------------------
  if ( !section_open ) { 

    if (mode == FA_READ) {
      returncode = 1;
      return returncode;
    }
    
    if (debug_mode) {
      printf("APPENDING new section name (%s) to end of file.\n", section );
    }
    fseek( fp, 0, SEEK_END ); 
    fprintf( fp, "\n%s\n", section );
    rewind(fp); 
    // Write contents of element also, if it was provided.
    return 0; 
    
  }



} // end faccess()





//----------------------------------------------------------------------------
// Support function accepts string containing a value to translate from a
// file read operation and converts it to the requested type, then loads it
// into the provided data array based on the indices provided.
//  int count - Used when retrieving a list or array of strings that need to
//              be stored in an array of strings passed in as 'recip'.
//----------------------------------------------------------------------------
int retrieve_payload( char result[], int type, int yctr, int xctr, int Xdim, void *recip, int count ) {
  
  // pointers for casting from void * type of I/O data structure
  float *fptr;
  int   *iptr;
  char  *sptr;

  // Temporary holding variables for values read in
  int   ipayload;
  float fpayload;
  char  spayload[FA_MAX_STRING_LENGTH];

  // Potential TODO: Generalize all to use more modern strtol
  // Allows for automatic integer base negotiation by '0x' characters.

  if (type == HEXINT) {
    iptr = (void *)(recip);
    ipayload = (int)strtol(result, NULL, 16);
    *(iptr + (yctr*Xdim) + xctr) = ipayload;
    if (debug_mode) {
      printf("value as hex:%x<<\n", ipayload);
    }
  }	
  if (type == INT) {
    iptr = (void *)(recip);
    ipayload = atoi(result);
    *(iptr + (yctr*Xdim) + xctr) = ipayload;
    if (debug_mode) {
      printf("value as int:%d<<\n", ipayload);
    }
  }		    
  if (type == FLOAT) {
    fptr = (void *)(recip);
    fpayload = atof(result);
    *(fptr + (yctr*Xdim) + xctr) = fpayload;
    if (debug_mode) {
      printf("value as flo:%f<<\n", fpayload);
    }
  }
  if (type == STRING) {
    sptr = (void *)(recip+(count*10));
    strcpy( sptr, result );
    if (debug_mode) {
      printf("value as str:%s<<\n", sptr);
    }
  }

  
  return 0;

}

