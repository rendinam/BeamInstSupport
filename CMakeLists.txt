
set(LIBNAME BeamInstSupport)
cmake_minimum_required(VERSION $ENV{ACC_CMAKE_VERSION})

find_package(GTK2)

set(INC_DIRS 
  ${GTK2_INCLUDE_DIRS}
  ../include
  ../c_utils/include
  ../mpmnet/include
  ../cbi_net/include
  ../cbpmfio/include
  include
  comm_include
  cesr_intf_include
  hardware_include
)

set(SRC_DIRS 
  actions
  cesr_intf
  comm_code 
  comm_include
  commstruct_output
  communication
  file_io
  system
  user_io
  utils
)

include($ENV{ACC_BUILD_SYSTEM}/Master.cmake)
