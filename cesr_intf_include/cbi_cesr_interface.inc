!------------------------------------------------------------------------+
! File         :  ctl_cbpm_interface.inc                                 |
!                                                                        |
! Description  :  Defines parameters and structures for a beam           |
!                 instrumentation system MPM mailbox interface           |
!                   To be used with FORTRAN                              |
!                                                                        |
! Author       :  M. Palmer                                              |
!------------------------------------------------------------------------+

!------------------------------------------------------------------------+
! Maximum size of bunch list arrays                                      |
!------------------------------------------------------------------------+
      INTEGER    CBPM_MAX_BUNCH_LIST
      PARAMETER (CBPM_MAX_BUNCH_LIST = 45)

!------------------------------------------------------------------------+
! Return parameters for the cbpm_request function                        |
!------------------------------------------------------------------------+

      INTEGER CBPM_REQUEST_SUCCESS            ! SUCCESS
      INTEGER CBPM_REQUEST_PROC_ERROR         ! No available CBPM processes
      INTEGER CBPM_LOCK_ERROR                 ! Semaphore lock error
      INTEGER CBPM_SLOCK_ERROR                ! Software lock error
      INTEGER CBPM_BAD_PROC_ERROR             ! Corrupt CBPM process table 
      INTEGER CBPM_INPUT_ERROR                ! Inputs error
      INTEGER CBPM_TIMEOUT_ERROR              ! Timeout error
      INTEGER CBPM_MISC_ERROR                 ! Miscellaneous error

      PARAMETER(CBPM_REQUEST_SUCCESS    =  0)
      PARAMETER(CBPM_REQUEST_PROC_ERROR =  1)
      PARAMETER(CBPM_LOCK_ERROR         =  2)
      PARAMETER(CBPM_SLOCK_ERROR        =  3)
      PARAMETER(CBPM_BAD_PROC_ERROR     =  4)
      PARAMETER(CBPM_INPUT_ERROR        =  5)
      PARAMETER(CBPM_TIMEOUT_ERROR      =  6)
      PARAMETER(CBPM_MISC_ERROR         = 99)

!------------------------------------------------------------------------+
! Parameters to request actions by the new CESR BPM readout system       |
!------------------------------------------------------------------------+
      INTEGER CBPM_EXIT_FLAG             ! Return CBPM processes to user 
                                         ! control
      INTEGER CBPM_NOCMD_FLAG            ! No command
      INTEGER CBPM_IDLE_BPM_FLAG         ! Idle all readout modules
      INTEGER CBPM_ORBIT_FLAG            ! Take orbit data
      INTEGER CBPM_PHASE_FLAG            ! Take phase data
      INTEGER CBPM_HEP_DATA_FLAG         ! Take continuous HEP orbit data
      INTEGER CBPM_CONT_DATA_FLAG        ! Take continuous USER-specified 
                                         ! orbit data
      INTEGER CBPM_MULTI_BUNCH_FLAG      ! Take multi-sample orbit data for 
                                         ! for multiple bunches
      INTEGER CBPM_TURNS_ORBIT_FLAG      ! Take multi-shot injected trajectory
                                         ! data to obtain orbit information for 
                                         ! specific turns
      INTEGER CBPM_INJ_TRAJ_FLAG         ! Take CBPM Injection Transient data 
                                         ! and write to file
      INTEGER CBPM_PEDESTALS_FLAG        ! Take CBPM Pedestal data 
      INTEGER CBPM_SHAKER_DATA_FLAG      ! Take CBPM SHAKER turns data 
                                         ! and write to file


      INTEGER CBPM_WRITE_BTNS_FLAG       ! Write CBPM Buttons file


      INTEGER CBPM_COMMAND_DISABLE       ! Disable the CBPM COMMAND interface
      INTEGER CBPM_COMMAND_ENABLE        ! Enable the CBPM COMMAND interface

      PARAMETER(CBPM_EXIT_FLAG        =   -1)
      PARAMETER(CBPM_NOCMD_FLAG       =    0)
      PARAMETER(CBPM_IDLE_BPM_FLAG    =    1)
      PARAMETER(CBPM_ORBIT_FLAG       =    2)
      PARAMETER(CBPM_PHASE_FLAG       =    3)
      PARAMETER(CBPM_HEP_DATA_FLAG    =    4)
      PARAMETER(CBPM_CONT_DATA_FLAG   =    5)
      PARAMETER(CBPM_MULTI_BUNCH_FLAG =    6)
      PARAMETER(CBPM_TURNS_ORBIT_FLAG =    7)
      PARAMETER(CBPM_INJ_TRAJ_FLAG    =    8)
      PARAMETER(CBPM_PEDESTALS_FLAG   =    9)
      PARAMETER(CBPM_SHAKER_DATA_FLAG =   10)

      PARAMETER(CBPM_WRITE_BTNS_FLAG  =  100)

      PARAMETER(CBPM_COMMAND_DISABLE  = 1000)
      PARAMETER(CBPM_COMMAND_ENABLE   = 1001)


!------------------------------------------------------------------------+
! Flags specifying whether to wait for a command to finish (or not)      |
!------------------------------------------------------------------------+
      INTEGER CBPM_PROCESS_WAIT         ! Wait for command to return
      INTEGER CBPM_PROCESS_NOWAIT       ! Don't wait for command to return
      
      PARAMETER(CBPM_PROCESS_WAIT   = 0)
      PARAMETER(CBPM_PROCESS_NOWAIT = 1)


!------------------------------------------------------------------------+
! CBPM process command return values                                     |
!------------------------------------------------------------------------+
      INTEGER CBPM_CMD_CLEAR          ! Cleared value of a command_done word
      INTEGER CBPM_CMD_WORKING        ! Monitoring process is working
      INTEGER CBPM_CMD_DONE           ! Action completed successfully
      INTEGER CBPM_CMD_ERROR          ! Error occurred during action

      PARAMETER(CBPM_CMD_CLEAR   = 0)
      PARAMETER(CBPM_CMD_WORKING = 1)
      PARAMETER(CBPM_CMD_DONE    = 2)
      PARAMETER(CBPM_CMD_ERROR   = 3)


!------------------------------------------------------------------------+
!  CSRBPM DSTAT and CSRBPM PSTAT return values - Indicates the status of |
!  CSRBPM DDATA (also EDATA) information on a detector-by-detector basis |
!  for orbit data.  Also used to indicate the status of CSRBPM PDATA     |
!  information for phase data.                                           |
!                                                                        |
!  CBPM_NO_DATA   - Data uninitialized                                   |
!  CBPM_BAD_DATA  - Error in acquiring data for this detector            |
!  CBPM_GOOD_DATA - Data is OK                                           |
!  CBPM_OLD_DATA  - Data is stale                                        |
!------------------------------------------------------------------------+
      INTEGER CBPM_NO_DATA  
      INTEGER CBPM_BAD_DATA 
      INTEGER CBPM_GOOD_DATA
      INTEGER CBPM_OLD_DATA 

      PARAMETER(CBPM_NO_DATA   = -2)
      PARAMETER(CBPM_BAD_DATA  =  0)
      PARAMETER(CBPM_GOOD_DATA =  1)
      PARAMETER(CBPM_OLD_DATA  =  3)


!------------------------------------------------------------------------+
! Parameters to specify electrons and positrons to the interface routine |
!------------------------------------------------------------------------+
      INTEGER CESR_ELECTRON
      INTEGER CESR_POSITRON

      PARAMETER(CESR_ELECTRON = -1)
      PARAMETER(CESR_POSITRON =  1)





