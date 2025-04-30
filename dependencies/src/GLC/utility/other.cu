#include <GLC/utility.cuh>

namespace GLC
{
    __device__ int threadID()
    {
        int blockId = 
        blockIdx.x +
        blockIdx.y * gridDim.x +
        blockIdx.z * gridDim.x * gridDim.y;
        int threadOffset = blockId * blockDim.x * blockDim.y * blockDim.z;
        int threadId = 
            threadIdx.x + 
            threadIdx.y * blockDim.x +
            threadIdx.z * blockDim.x * blockDim.y;
        return threadOffset + threadId;
    }
}