/*!-----------------------------------------------------------------------+
* File         :  cbi_datafile_name.c                                     |
*                                                                         |
* Description  :  Generates a unique data file name based on the type of  |
*                 data that was collected.  Stores a standardized file    |
*                 specifier and index number along with a new file        |
*                 indicator so that those pieces of info may be published |
*                 to the database for supported commands when in server   |
*                 mode.                                                   |
*                                                                         |
* Arguments   : -filetype (integer)                                       |
*                 1 - Time Scan              (TS)                         |
*                 2 - Raw Data               (RD)                         |
*                 3 - Raw Data EXTENDED      (RDe)                        |
*                 4 - Chan Cal Data          (CC)                         |
*                 5 - Find bunch scan        (FB)                         |
*                 6 - Chan Cal Error         (CE)                         |
*                 7 - Processed data         (PROC)                       |
*                 8 - Calibration Log        (CAL)                        |
*                 9 - Betatron phase tables  (BP)                         |
*                10 - Instrument Parameters  (PARAMETERS)                 |
*              -Pointer to string for returning the file name             |
*              -Index of device struct                                    |
*              -Flag whether or not to increment the filename index       |
*                     during the function call.                           |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_datafile_name_single( int filetype, char *filename, int file_idx) {
  
  CTL_CBPM_Intf_Monitor.FileStatus = CBI_CTL_NEW_FILE;

  CTL_CBPM_Intf_Monitor.FileID = file_idx;
  CTL_System.output_file_index = file_idx;


  if (filetype == TIME_SCAN){ // Time scan data
    sprintf(filename, "TS-%.6d.dat", file_idx);
  }
  if (filetype == RAW){ // Turn-by-turn (raw) data
    sprintf(filename, "RD-%.6d.dat", file_idx);
    CTL_CBPM_Intf_Monitor.FileType = CBPM_TBT_FILE;
  }
  if (filetype == RAW_EXTENDED){ // Timing stability data
    sprintf(filename, "RDe-%.6d.dat", file_idx);
  }
  if (filetype == CHAN_CAL){ // Channel calibration data
    sprintf(filename, "CC-%.6d.dat", file_idx);
  }
  if (filetype == FIND_BUNCH){ // Find_bunch data
    sprintf(filename, "FB-%.6d.dat", file_idx);
  }
  if (filetype == CHAN_CAL_ERROR){ // Channel calibration error
    sprintf(filename, "CE-%.6d.dat", file_idx);
  }
  if (filetype == PROC){ // Processed data
    sprintf(filename, "PROC-%.6d.dat", file_idx);
    CTL_CBPM_Intf_Monitor.FileType = CBPM_BUTTON_FILE;
  }
  if (filetype == CALIBRATION_LOG){ // Calibration Data/Log
    sprintf(filename, "CAL-%.6d.dat", file_idx);
  }
  if (filetype == PHASE_TABLES){ // Betatron phase tables
    sprintf(filename, "BP-%.6d.dat", file_idx);
  }
  if (filetype == PARAMETERS){ // Instrument Operational Parameters
    sprintf(filename, "instrument_parameters.%.6d", file_idx);
  }

  return F_SUCCESS;
}



int cbi_datafile_name(int filetype, char *filename, int iidx, int file_idx) {

  
  CTL_CBPM_Intf_Monitor.FileStatus = CBI_CTL_NEW_FILE;

  CTL_CBPM_Intf_Monitor.FileID = file_idx;
  CTL_System.output_file_index = file_idx;

  char numstring[4] = "";
  numstring[0] = CTL_System.p_Module[iidx]->comm.ethernet.hostname[5];
  numstring[1] = CTL_System.p_Module[iidx]->comm.ethernet.hostname[6];
  numstring[2] = CTL_System.p_Module[iidx]->comm.ethernet.hostname[7];
  int hostname_num = atoi( numstring );

  if (filetype == TIME_SCAN){ // Time scan data
    sprintf(filename, "TS-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == RAW){ // Turn-by-turn (raw) data
    sprintf(filename, "RD-%.6d-%.03d.dat", file_idx, hostname_num);
    CTL_CBPM_Intf_Monitor.FileType = CBPM_TBT_FILE;
  }
  if (filetype == RAW_EXTENDED){ // TRANSITIONAL turn-by-turn data extended header info
    sprintf(filename, "RDe-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == CHAN_CAL){ // Channel calibration data
    sprintf(filename, "CC-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == FIND_BUNCH){ // Find_bunch data
    sprintf(filename, "FB-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == CHAN_CAL_ERROR){ // Channel calibration error
    sprintf(filename, "CE-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == PROC){ // Processed data
    sprintf(filename, "PROC-%.6d-%.03d.dat", file_idx, hostname_num);
    CTL_CBPM_Intf_Monitor.FileType = CBPM_BUTTON_FILE;
  }
  if (filetype == CALIBRATION_LOG){ // Calibration Data/Log
    sprintf(filename, "CAL-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == PHASE_TABLES){ // Betatron phase tables
    sprintf(filename, "BP-%.6d-%.03d.dat", file_idx, hostname_num);
  }
  if (filetype == PARAMETERS){ // Instrument Operational Parameters
    sprintf(filename, "instrument_parameters.%.6d", file_idx);
  }

  return F_SUCCESS;

}
