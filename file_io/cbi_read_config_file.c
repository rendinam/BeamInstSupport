/*!
*-----------------------------------------------------------------------+
* File         :  cbi_read_config_file.c                                |
*                                                                       |
* Description  :  Parse the application configuration file to gather    |
*                 run-time options such as LDR paths, device list paths,|
*                 file names, data file output directories and other    |
*                 such options and parameters.                          |
*                                                                       |
*                                                                       |
* Arguments    : pointer to character array holding full path and       |
*                name of file                                           |
*                                                                       |
* Author       :  M.Rendina                                             |
*-----------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


#define APP_CONFIG_FILE_TEMPLATE_NAME      "cbic.conf.TEMPLATE"


int cbi_read_config_file( char *filename ) {
  
  int i;

  //---------------------------------------
  // Config file token definitions
  //---------------------------------------
  // Section names
  char *config_section   = "APPLICATION_CONFIG";


  // Element names
  char *app_id         = "APPLICATION_ID";
  char *CBPM_server_id = "CBPM_SERVER_ID"; // Fixme, condense in enter_server_mode()
                                                // to take ID number from ID string.
  char paths[10][CBI_MAX_STRING_LENGTH] = {
    "CBI_PT_INST_EXE_PATH",
    "CBI_ST_INST_EXE_PATH"
  };

  char *param_file      = "PARAM_FILE";
  char *det_param_file  = "DET_PARAM_FILE";
  char *inst_param_file = "INST_PARAM_FILE";

  char *raw_dir         = "RAW_DATA_DIR";
  char *proc_dir        = "PROC_DATA_DIR";
  char *tscan_dir       = "TSCAN_DATA_DIR";
  char *phase_dir       = "PHASE_DATA_DIR";
  char *param_dir       = "PARAM_DIR";
  char *inst_param_dir  = "INST_PARAM_DIR";

  char *instr_alloc     = "ALLOCATION_FILE";

  char *timing_setup    = "DEFAULT_TIMING_SETUP";
  char *default_gain    = "DEFAULT_GAIN_SETTING";
  char *bunpat_def_file = "BUNCH_PAT_DEF_FILE";
  char *comm_method     = "COMM_METHOD";

  //-------------------------------------------

  FILE *cf;
  int tsetup;
  cf = fopen(filename, "r");
  
  if (cf == (int)NULL) {

    printf("\nApplication configuration file not found!\n\n");
    printf("Generating a new template file in the current working directory...\n");
    
    cf = fopen(APP_CONFIG_FILE_TEMPLATE_NAME, "w");
    fprintf(cf, "\n#==================================================\n");
    fprintf(cf, "# CBIC Application Configuration File\n");
    fprintf(cf, "# Please note: \n");
    fprintf(cf, "   All parameters in this file are CASE SENSITIVE.\n");
    fprintf(cf, "#==================================================\n");
    fprintf(cf, "[APPLICATION_CONFIG]\n");
    fprintf(cf, "\n");
    fprintf(cf, "   #----------------------------------\n");
    fprintf(cf, "   #  Application/Server Instance ID\n");
    fprintf(cf, "   #----------------------------------\n");
    fprintf(cf, "   .APPLICATION_ID         <instrument-type>-<#>\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #------------------------------\n");
    fprintf(cf, "   # Instrument Sofware Flashing\n");
    fprintf(cf, "   #------------------------------\n");
    fprintf(cf, "   .CBI_PT_INST_EXE_PATH   <full_path>\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #---------------------------------------------\n");
    fprintf(cf, "   # Data file output directory\n");
    fprintf(cf, "   #  (Do not provide a trailing '/' character)\n");
    fprintf(cf, "   #---------------------------------------------\n");
    fprintf(cf, "   .RAW_DATA_DIR           <path>\n");
    fprintf(cf, "   .PROC_DATA_DIR          <path>\n");
    fprintf(cf, "   .TSCAN_DATA_DIR         <path>\n");
    fprintf(cf, "   .PHASE_DATA_DIR         <path>\n");
    fprintf(cf, "   .PARAM__DIR             <path>\n");
    fprintf(cf, "   .INST_PARAM_DIR         <path>\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # File containing list of instruments to bring \n");
    fprintf(cf, "   # online and their XBUS addressing mappings.\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .INSTRUMENT_LIST        <path>\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Instrument Operational Parameters File Path\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .PARAM_FILE             <full_path>\n");
    fprintf(cf, "   .LOC_PARAM_FILE         <full_path>\n");
    fprintf(cf, "   .INST_PARAM_FILE        <full_path>\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Instrument Defaults\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Timing Setup\n");
    fprintf(cf, "   #  Valid options are:\n");
    //for (tsetup = 0; tsetup < CBI_PT_MAX_TIMING_SETUPS; tsetup++) {
    //  fprintf(cf, "   #    %s\n", tsetup_names[tsetup] );
    //}
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .DEFAULT_TIMING_SETUP     14ns\n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Gain Setting\n");
    //fprintf(cf, "   #  Valid options are: 0 - %d\n", CBI_PT_MAX_GAIN_SETTING );
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .DEFAULT_GAIN_SETTING     3 \n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Bunch Pattern Definition File\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .BUNCH_PAT_DEF_FILE       /nfs/cesr/instr/cbpm_data/tsharc/bunch_patterns.cfg \n");
    fprintf(cf, "\n\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   # Communications\n");
    fprintf(cf, "   #  Valid options are:\n");
    fprintf(cf, "   #    ETHERNET\n");
    fprintf(cf, "   #    XBUS\n");
    fprintf(cf, "   #----------------------------------------------\n");
    fprintf(cf, "   .COMM_METHOD         ETHERNET\n");
    fprintf(cf, "\n\n");
    if (fclose(cf) != 0) {
      perror("fclose: ");
    }

    printf("File ./%s created.\n\n", APP_CONFIG_FILE_TEMPLATE_NAME);
    printf("You will need to edit this file before running again \n");
    printf("to customize the behavior of the application.  Exiting.\n\n");

    exit(0);

  }  //endif (cf == (int)NULL)

  char temp_string[20];


  // TODO: Check each of these for success
  faccess( READ, cf, config_section, app_id,          1, 1, STRING, appconfig.app_ID );
  faccess( READ, cf, config_section, CBPM_server_id,  1, 1, INT,    &(appconfig.CBPM_server_ID) );

  //faccess( READ, cf, config_section, paths[0],        1, 1, STRING, appconfig.cbi_PT_EXE_file );
  faccess( READ, cf, config_section, paths[0],        1, 1, STRING, CTL_System.cfg_file );

  faccess( READ, cf, config_section, det_param_file,  1, 1, STRING, appconfig.det_param_file );
  faccess( READ, cf, config_section, inst_param_file, 1, 1, STRING, appconfig.inst_param_file );

  faccess( READ, cf, config_section, raw_dir,         1, 1, STRING, appconfig.directories[RAW_DIR] );
  faccess( READ, cf, config_section, proc_dir,        1, 1, STRING, appconfig.directories[PROC_DIR] );
  faccess( READ, cf, config_section, tscan_dir,       1, 1, STRING, appconfig.directories[TSCAN_DIR] );
  faccess( READ, cf, config_section, phase_dir,       1, 1, STRING, appconfig.directories[PHASE_DIR] );
  faccess( READ, cf, config_section, param_dir,       1, 1, STRING, appconfig.directories[PARAMS_DIR] );
  faccess( READ, cf, config_section, inst_param_dir,  1, 1, STRING, appconfig.directories[INST_CONSTS_DIR] );
  
  faccess( READ, cf, config_section, instr_alloc,     1, 1, STRING, appconfig.inst_alloc_file );


  // Timing setup
  faccess( READ, cf, config_section, timing_setup, 1, 1, STRING, appconfig.system_timing_setup_label );

  // Default gain setting
  faccess( READ, cf, config_section, default_gain, 1, 1, INT, &(appconfig.default_gain_setting) );

  // Bunch pattern definition file path read
  faccess( READ, cf, config_section, bunpat_def_file,  1, 1, STRING, appconfig.bunpat_def_file );


  // Communications method
  faccess( READ, cf, config_section, comm_method,  1, 1, STRING, temp_string );
  for (i = 0; i < NUM_COMM_METHODS; i++) {
    if ( strcmp( temp_string, comm_method_names[i] ) == 0 ) {
      printf("Setting primary communications method to  %s \n", comm_method_names[i] );
      CTL_System.comm_method = i;
      break;
    }
    if (i == NUM_COMM_METHODS-1) {
      printf(" !- Element (%s) in section %s has an invalid value!\n",
	     comm_method,
	     config_section );
      printf(">%s<\n", temp_string );
      return F_FAILURE;
    }
  }


  if (fclose(cf) != 0) {
    perror("fclose: ");
  }

  return F_SUCCESS;

}
