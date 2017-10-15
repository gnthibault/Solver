#ifndef CHECK_UTILS_CU_H
#define CHECK_UTILS_CU_H

//For logging utility
#include <stdio.h>

//Cuda Runtime API
#include <cuda_runtime_api.h>

//Local


//Output a CUDA error string corresponding to the given error code
inline void __checkCudaErrors(
    cudaError err, const char *file, const int line ) {
  if(err != cudaSuccess) {
    std::cout<<file<<"("<<line<<") : CUDA Runtime API error "
      <<(int)err<<" : "<<cudaGetErrorString(err)<<std::endl;
  }
};
#define checkCudaErrors(err)	__checkCudaErrors (err, __FILE__, __LINE__)

#ifdef USE_NVTX
  #include "nvToolsExt.h"
  const uint32_t colors[] = {
    0x0000ff00,
    0x000000ff,
    0x00ffff00,
    0x00ff00ff,
    0x0000ffff,
    0x00ff0000,
    0x00ffffff };
  const int num_colors = sizeof(colors)/sizeof(uint32_t);

  #define PUSH_NVCTX(name,cid) { \
    int color_id = cid; \
    color_id = color_id%num_colors;\
    nvtxEventAttributes_t eventAttrib = {0}; \
    eventAttrib.version = NVTX_VERSION; \
    eventAttrib.size = NVTX_EVENT_ATTRIB_STRUCT_SIZE; \
    eventAttrib.colorType = NVTX_COLOR_ARGB; \
    eventAttrib.color = colors[color_id]; \
    eventAttrib.messageType = NVTX_MESSAGE_TYPE_ASCII; \
    eventAttrib.message.ascii = name; \
    nvtxRangePushEx(&eventAttrib); \
  }
#define POP_NVCTX nvtxRangePop();
#else
  #define PUSH_NVCTX(name,cid)
  #define POP_NVCTX
#endif

#endif //CHECK_UTILS_CU_H
