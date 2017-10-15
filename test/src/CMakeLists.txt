cmake_minimum_required(VERSION 3.5)

####################
#  Lib Dependancy  #
####################

include_directories( "${PRJ_ROOT_DIR}/src" )
set(lib_deps Solver)

################
#  Build test  #
################

# Linear operators
add_lib_executable(dwt_1D ${lib_deps})
add_lib_executable(dwt_2D ${lib_deps})
add_lib_executable(dwt_3D ${lib_deps})

if (CUDA_FOUND AND USE_CUDA)
  # Linear operators
  #add_cuda_lib_executable(dwtcuda_1D ${lib_deps})
  #add_cuda_lib_executable(dwtcuda_2D ${lib_deps})
  #add_cuda_lib_executable(dwtcuda_3D ${lib_deps})
endif ()

##############
#  Unit test #
##############
# Linear operators
add_test(NAME dwt_1D COMMAND dwt_1D)
add_test(NAME dwt_2D COMMAND dwt_2D)
add_test(NAME dwt_3D COMMAND dwt_3D)

if (CUDA_FOUND AND USE_CUDA)
  # Linear operators
  #  add_test(NAME dwtcuda_1D COMMAND dwtcuda_1D)
  #  add_test(NAME dwtcuda_2D COMMAND dwtcuda_2D)
  #  add_test(NAME dwtcuda_3D COMMAND dwtcuda_3D)
endif ()

##################
#  Config files  #
##################
#set(DATA_FILES
#  "${TEST_ROOT_DIR}/common/data/data.dat"
#)
#install(FILES ${DATA_FILES}
#  DESTINATION ${TEST_BIN_DIR} COMPONENT data)
