cmake_minimum_required (VERSION 3.5)

#####################################
#  Adding various code directories  #
#####################################

add_subdirectory(dwt1D)
add_subdirectory(dwt2D)
add_subdirectory(dwt3D)

if (CUDA_FOUND AND USE_CUDA)
#  add_subdirectory(dwt2Dcuda)
  add_subdirectory(dwt2Dcuda)
#  add_subdirectory(dwt2Dcuda)
endif ()
