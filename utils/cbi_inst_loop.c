/*!
*-------------------------------------------------------------------------+
* File         :  cbi_inst_loop.c                                         |
*                                                                         |
* Description  :  Iterates over all active instruments, executing the     |
*                 function specified.                                     |
*                                                                         |
* Arguments    :  None                                                    |
*                                                                         |
* Author       :  M. Rendina                                              |
*-------------------------------------------------------------------------+*/

#include "cbi_core_includes.h"


int cbi_inst_loop( int beg_idx, int end_idx, void (*p_func), int parallel, int num_args, ... ) {

  char *func_name = (char *)__FUNCTION__;
  int debug_level = CTL_System.debug_verbosity;

  int iidx = 0;
  int stat;

  int file_ID_state   = CBI_FILE_ID_NOT_SET;

  int success_tally = 0;

  //-----------------------------------------------------
  // Function signatures that this iterator will accept:
  typedef int (*P_F0)();
  typedef int (*P_F1)(int);
  typedef int (*P_F2)(int, int);


  P_F0 fptr0;
  P_F1 fptr1;
  P_F2 fptr2;

  //-----------------------------------------------------

  // These facilitate variadic function calls below
  va_list  ap;
  int      arg;
  va_start( ap, num_args );


  // One thread will be spawned for each active instrument.
  int       MAX_THREADS = cbi_num_active(); 
  pthread_t threads[MAX_THREADS];
  int       thread_iidx[MAX_THREADS];
  int       thread;
  int       status;


  //---------------------------------------------
  // Function to call requires no arguments
  //  (prep, execute, and wait calls here)
  //---------------------------------------------
  if (num_args == 0) {
    arg = va_arg(ap, int);

    fptr0 = (P_F0) p_func;
    for (iidx = beg_idx; iidx < end_idx; iidx++) {
      if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE  ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_1 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_2 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_3 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_4     ) {

	CTL_System.p_Module[iidx]->status = stat = (*fptr0) (iidx);

	if (stat == CBI_F_SUCCESS) {
	  success_tally++;
	} else {
	  // If instrument is already in the midst of a recovery process,
	  // and another error occurs, (such as might be the case when
	  // communications with the instrument are completely inoperative)
	  // this will disable the instrument entirely to prevent an
	  // infinite regress of recovery attempts.
	  if (CTL_System.p_Module[iidx]->in_recovery == CBI_TRUE) {
	    CTL_System.p_Module[iidx]->active = CBI_INACTIVE;
	    sprintf(message, "Problem during recovery process for %s.  DEACTIVATING instrument.\n",
		    CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	    log_message( S_ERROR, func_name, message );
	  } else {
	    sprintf(message, "Execute or wait call failed for instrument %s @ %s",
		    CTL_System.p_Module[iidx]->comm.ethernet.hostname,
		    CTL_System.p_Module[iidx]->det.location);
	    log_message( S_ERROR, func_name, message );
	  }

	} //endIF
	
      }
    }
    
  }



  int num_spawned = 0;
  //-------------------------------------------------
  // Function to call requires the standard 
  // argument for looping over instruments (iidx)
  // For post-type calls.
  //-------------------------------------------------
  if (num_args == 1) {
    
    arg = va_arg(ap, int);
    
    fptr1 = (P_F1) p_func;


    if (parallel == TRUE) {
      // -- Parallel method
      for (iidx = beg_idx; iidx < end_idx; iidx++) {
	
	if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE  ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_1 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_2 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_3 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_4     ) {
	  
	  // Check on status of call chain up to this point.
	  // If the status of the execution chain for this instrument 
	  // is not without errors, skip subsequent calls (i.e. post call).
	  if (CTL_System.p_Module[iidx]->status == CBI_F_SUCCESS) {

	    CTL_System.p_Module[iidx]->status = stat = pthread_create( &(threads[num_spawned]), NULL, (void *)fptr1, (void *)iidx );
	    if (stat == 0) {
	      thread_iidx[num_spawned] = iidx;
	      num_spawned++;
	    } else {
	      // Problem spawning thread.  Drop MAX_THREADS by one here so that 
	      // the threhold below reflects the actual number of threads in need
	      // of eventual cleanup.
	      perror("pthread_create: ");
	      MAX_THREADS--;
	      CTL_System.p_Module[iidx]->status = CBI_F_FAILURE;
	      sprintf( message, "Could not spawn thread instance [%d] for instrument index [%d]!", 
		       num_spawned, iidx);
	      log_message(S_ERROR, func_name, message);
	      sprintf(message, "Post function failed to execute for instrument %s.\n",
		      CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	      log_message(S_ERROR, func_name, message);
	    }

	  } else {	    
	    sprintf(message, "Skipping post call for instrument  %s  due to upstream error.",
		    CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	    log_message(S_INFO, func_name, message);
	  } //endIF (status)
	  
	} //endIF (active)

	// 'num_spawned' threshold check in the overall iidx loop is here so that the 
	// option is available to spawn threads in arbitrarily-sized batches, the size
        // of the batch to be used as a tunable parameter for performance checking.
	//        Not used at present.  May not be necessary at all, in which case,
	//        two loops may make more sense here.
	//
	// Wait until all expected threads have been spawned, then block 
	// execution here until all have exited.
	if (num_spawned == MAX_THREADS) { 
	  for (thread = 0; thread < num_spawned; thread++) {
	    if ( CTL_System.p_Module[ thread_iidx[thread] ]->active == TRUE ) {

	      pthread_join( threads[thread], (void *)&(status) );
	      CTL_System.p_Module[ thread_iidx[thread] ]->status = status;

	      if (debug_level > CBI_DEBUG_LEVEL_1) {
		printf("%s returned: %d\n", 
		       CTL_System.p_Module[ thread_iidx[thread] ]->comm.ethernet.hostname,
		       status );
	      }
		
	      if (CTL_System.p_Module[ thread_iidx[thread] ]->status == CBI_F_SUCCESS) {
		success_tally++;
	      } else {
		sprintf(message, "Post function failed for instrument %s.",
			CTL_System.p_Module[ thread_iidx[thread] ]->comm.ethernet.hostname );
		log_message(S_ERROR, func_name, message);
		
		if (CTL_System.p_Module[ thread_iidx[thread] ]->in_recovery == CBI_TRUE) {
		  CTL_System.p_Module[ thread_iidx[thread] ]->active = CBI_INACTIVE;
		  sprintf(message, "Problem during recovery process for %s. DEACTIVATING instrument.\n",
			  CTL_System.p_Module[ thread_iidx[thread] ]->comm.ethernet.hostname );
		  log_message( S_ERROR, func_name, message );
		}
	      } //endIF ->status

	    }
	  }
	  num_spawned = 0;
	  printf("\n");
	}
	
      } //endFOR iidx
      

    } else {  // (parallel == FALSE)


      // -- Serial method
      for (iidx = beg_idx; iidx < end_idx; iidx++) {
	
	if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE  ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_1 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_2 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_3 ||
	     CTL_System.p_Module[iidx]->active == CBI_MAINT_4     ) {
	  
	  // Check on status of call chain up to this point.
	  // If the status of the execution chain for this instrument
	  // is not without errors, skip subsequent calls (i.e. post call).
	  if (CTL_System.p_Module[iidx]->status == CBI_F_SUCCESS) {
	    
	    
	    CTL_System.p_Module[iidx]->status = stat = (*fptr1) (iidx);
	    
	    if (stat == CBI_F_SUCCESS) {
	      success_tally++;
	    } else {
	      
	      sprintf(message, "Post function failed for instrument %s.",
		      CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	      log_message(S_ERROR, func_name, message);
	      
	      if (CTL_System.p_Module[iidx]->in_recovery == CBI_TRUE) {
		CTL_System.p_Module[iidx]->active = CBI_INACTIVE;
		sprintf(message, "Problem during recovery process for %s. DEACTIVATING instrument.\n",
			CTL_System.p_Module[iidx]->comm.ethernet.hostname );
		log_message( S_ERROR, func_name, message );
	      }
	    }
	    
	  } else {
	    
	    sprintf(message, "Skipping post call for instrument  %s  due to upstream error.",
		    CTL_System.p_Module[iidx]->comm.ethernet.hostname );
	    log_message(S_INFO, func_name, message);
	    
	  } //endIF (status)
	  
	} //endIF (active)
      
      } //endFOR


    } //endIF parallel


  }  // endIF num_args
    



  //----------------------------------------------
  // Function to call requires two arguments
  //  i.e. Structure set copy and transmit 
  //       function
  //----------------------------------------------
  if (num_args == 2) {
    
    arg = va_arg(ap, int); 
    fptr2 = (P_F2) p_func;
    
    for (iidx = beg_idx; iidx < end_idx; iidx++) {
      if ( CTL_System.p_Module[iidx]->active == CBI_ACTIVE  ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_1 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_2 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_3 ||
	   CTL_System.p_Module[iidx]->active == CBI_MAINT_4     ) {

	stat = (*fptr2) (iidx, arg);

	if (stat == CBI_F_SUCCESS) {
	  CTL_System.p_Module[iidx]->status = CBI_F_SUCCESS;
	  success_tally++;
	} else {
	  CTL_System.p_Module[iidx]->status = CBI_STRUCT_COPY_FAILURE;
	  // FIXME: Generalize message
	  sprintf(message, "Structure set transfer failure temp-->instrument %s @ %s",
		  CTL_System.p_Module[iidx]->comm.ethernet.hostname,
		  CTL_System.p_Module[iidx]->det.location);
	  log_message(S_ERROR, func_name, message);
	}

      }
    }
    
  }
  

  // Reset file ID indicator
  file_ID_state = CBI_FILE_ID_NOT_SET;  
  

  return success_tally;

}
