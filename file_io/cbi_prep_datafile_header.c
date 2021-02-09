/*!-----------------------------------------------------------------------+
* File         :  cbi_prep_datafile_header.c                              |
*                                                                         |
* Description  :  Writes a general descriptive data file header in ASCII  |
*                 to a given file.                                        |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"

#define DATA_FILE_VERSION          3

int cbi_prep_datafile_header(RDV3_FILE_HEADER *hdata, int filetype, int iidx, int file_idx ) {

  time_t     curtime;
  struct tm *loctime;
  curtime = time(NULL);
  loctime = localtime(&curtime);

  // Compose field mapping
  fio_addrs addrs;
  addrs[0] = (long)(&CTL_System.itype.instrument_type);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 0, addrs);

  addrs[0] = (long)&(CTL_System.meas_info.command_ID);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 1, addrs);

  addrs[0] = (long)&((*hdata).file_type);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 2, addrs);

  addrs[0] = (long)&(file_idx);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 3, addrs);

  addrs[0] = (long)&(CTL_System.meas_info.file_version);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 4, addrs);

  addrs[0] = (long)&(CTL_System.meas_info.timestamp);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 5, addrs);

  addrs[0] = (long)&(CTL_System.p_Module[iidx]->core->cbi_module_config.core_comm_struct_rev);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 6, addrs);

  addrs[0] = (long)&(CTL_System.p_Module[iidx]->core->cbi_module_config.platform_comm_struct_rev);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 7, addrs);

  addrs[0] = CBPMFIO_NO_ADDRESS;
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 8, addrs);

  addrs[0] = (long)CTL_System.meas_info.p_bunch_pattern;
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 9, addrs);

  addrs[0] = (long)&((*hdata).species_name);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 10, addrs);

  addrs[0] = (long)&(CTL_System.meas_info.num_instruments);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 11, addrs);

  addrs[0] = (long)&(CTL_System.meas_info.num_bunches);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 12, addrs);

  addrs[0] = (long)CTL_System.meas_info.p_num_turns;
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 13, addrs);

  addrs[0] = (long)CTL_System.meas_info.p_turn_spacing;
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 14, addrs);

  addrs[0] = (long)&((*hdata).timing_setup);
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 15, addrs);

  addrs[0] = CBPMFIO_NO_ADDRESS;
  cbpmfio_map_addresses_to_field(&RDV3_header_specs, 16, addrs);
  // End field mapping


  int (*fptr)()  = NULL;

  int  tsetup           = CBI_UNINITIALIZED_PARAM;
  char timing_setup[50] = "";

  int  species          = CBI_UNINITIALIZED_PARAM;
  char species_name[50] = "";

  int   reg, bit, reg_bits = 16;
  int block_A_used = 0;
  int block_B_used = 0;

  // Populate staging variables for write
  CTL_System.meas_info.file_version = DATA_FILE_VERSION;
  strcpy( CTL_System.meas_info.command_ID, CTL_System.loaded_command.name );
  strcpy( CTL_System.meas_info.timestamp, asctime(loctime) );

  sprintf((*hdata).file_type, data_modes[filetype] );

  CTL_System.meas_info.num_instruments = cbi_num_active();
  
  fptr = CTL_System.itype.tsetup_func_ptr;
  tsetup = CTL_System.meas_info.tsetup = (*fptr)();
  
  strcpy((*hdata).timing_setup, &(CTL_System.itype.tsetup_names[tsetup*CBI_MAX_TSETUP_NAME_LENGTH]) );
  
  fptr = CTL_System.itype.species_func_ptr;
  species = CTL_System.meas_info.species = (*fptr)();
  strcpy((*hdata).species_name, cbi_species_names[species] );
  
  CTL_System.meas_info.num_bunches = 0;    
  for (reg = CBI_MAX_16BIT_PATTERN_REGS-1; reg >= 0; reg--) {
    for (bit = 0; bit < reg_bits; bit++) {
      if (CTL_System.meas_info.p_bunch_pattern[reg] & (1 << bit)) {
	CTL_System.meas_info.num_bunches++;
	if (cbi_parity(bit) == 0) {
	  block_A_used = 1;
	} else {
	  block_B_used = 1;
	}
      }
    }
  }
  // If in a 14ns mode, determine if only one or both species were measured.
  // The default label for 14ns modes is DUAL species.  This overrides that
  // label with a more specific species label if only a single species was
  // examined.
  if ( cbi_parity(block_A_used + block_B_used) != 0 ) {
    if (block_A_used) {
      species = POSITRONS;
    } else {
      species = ELECTRONS;
    }
    strcpy( (*hdata).species_name, cbi_species_names[species] );
  }

  return 0;
}
