cmake_minimum_required(VERSION 3.5)

#################
#  Generic Part #
#################
project(Solver)
set(OUTPUT_NAME Solver)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

###############
#  Cuda Part  #
###############
if (CUDA_FOUND AND USE_CUDA)
  set(sources_cuda
    cuda/utils/Constant.cu

    #utils
    cuda/utils/DeviceManager.cu

    #Field
    #Field/CoeffContainer.cu
)
  set(headers_cuda
    cuda/utils/ManagedAllocator.cu.h

    #utils
    cuda/utils/DeviceManager.cu.h
  )
endif ()
################
#  /Cuda Part  #
################i

###############
#  Src files  #
###############
set(sources
# Field
  Field/CoeffContainer.cpp

# Operator
  Operator/Wavelet/Filters.cpp
  Operator/Wavelet/Wavelet.cpp
  Operator/Wavelet/Wavelet1D.cpp
  Operator/Wavelet/Wavelet2D.cpp
  Operator/Wavelet/Wavelet3D.cpp
  # cuda source files
  ${sources_cuda}
)

set(headers
# Field
  Field/Filters.h

# Operator
  Operator/Wavelet/Wavelet.h
  Operator/Wavelet/Wavelet1D.h
  Operator/Wavelet/Wavelet2D.h
  Operator/Wavelet/Wavelet3D.h
  # cuda header files
  ${headers_cuda}
)

################
#  Executable  #
################

if (CUDA_FOUND)
  cuda_add_library(${OUTPUT_NAME} SHARED ${sources})
  CUDA_ADD_CUBLAS_TO_TARGET(${OUTPUT_NAME})
else ()
  add_library(${OUTPUT_NAME} SHARED ${sources})
endif ()
target_link_libraries(${OUTPUT_NAME}
  ${cuda_deps}
  ${boost_deps}
  ${openmp_deps})
set_property(TARGET ${OUTPUT_NAME}
  PROPERTY OUTPUT_NAME ${output_name})
install(TARGETS ${OUTPUT_NAME}
  LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY}
)
