#include <GLC/utility.cuh>

namespace GLC
{
    __host__ __device__ float radians(float degree)
    {
        #ifdef __CUDA_ARCH__
            return degree * CUDART_PI_F / 180.0f;
        #else
            return degree * std::numbers::pi / 180.0f;
        #endif
    }
}