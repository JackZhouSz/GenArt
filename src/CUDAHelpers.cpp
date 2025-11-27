#include "CUDAHelpers.h"

#include "GL/glew.h"
#include "cuda_runtime.h"

#include <cstdio>

void setCUDADevice(int devid)
{
    int deviceCount;

    cudaGetDeviceCount(&deviceCount);

    checkCUDAError("cudaGetDeviceCount");

    ASSERT_R(devid < deviceCount);

    cudaSetDevice(devid);

    checkCUDAError("cudaSetDevice");
}

void checkCUDAError(const char* msg)
{
    cudaError_t Err = cudaGetLastError();
    if (cudaSuccess != Err) {
        fprintf(stderr, "CUDA error: %s: %s.\n", msg, cudaGetErrorString(Err));
        exit(EXIT_FAILURE);
    }
}

void checkCUDABusy(cudaStream_t& cstream, const char* msg)
{
    cudaError_t Err = cudaStreamQuery(cstream);
    if (Err == cudaErrorNotReady)
        std::cerr << msg << " busy\n";
    else if (Err != cudaSuccess)
        checkCUDAError(msg);
    else
        std::cerr << msg << " done\n";
}

void getCUDAMemInfo()
{
    size_t mem_free, mem_total;
    cudaMemGetInfo(&mem_free, &mem_total);

    std::cerr << "CUDA Free Memory = " << mem_free << " CUDA Total Memory = " << mem_total << '\n';
}

void getCUDADeviceInfo()
{
    int i, n, dv, rv;
    const char* suffix = "B";

    printf("\nCUDA Device Info\n");
    cudaError_t a = cudaDriverGetVersion(&dv);
    cudaError_t b = cudaRuntimeGetVersion(&rv);

    printf("Driver version=%d\nRuntime version=%d\n", dv, rv);

    cudaError_t goe = cudaGetDeviceCount(&n);

    printf("There are %d cuda devices\n", n);

    for (i = 0; i < n; i++) {
        printf("\nCUDA device %d:\n", i);
        cudaDeviceProp prop;

        cudaError_t joe = cudaGetDeviceProperties(&prop, i);
        if (prop.totalGlobalMem > 10 * 1024 * 1024) {
            prop.totalGlobalMem /= 1024 * 1024;
            suffix = "MB";
        }

        printf("char         name[256]=\"%s\";\n"
               "cudaUUID_t   uuid=???;\n"
               "char         luid[8]=%s;\n"
               "unsigned int luidDeviceNodeMask=%u;\n"
               "size_t       totalGlobalMem=%lld %s;\n"
               "size_t       sharedMemPerBlock=%lld;\n"
               "int          regsPerBlock=%d;\n"
               "int          warpSize=%d;\n"
               "size_t       memPitch=%lld;\n"
               "int          maxThreadsPerBlock=%d;\n"
               "int          maxThreadsDim[3]=[%d %d %d];\n"
               "int          maxGridSize[3]=[%d %d %d];\n"
               "size_t       totalConstMem=%lld;\n"
               "int          major=%d;\n"
               "int          minor=%d;\n"
               "size_t       textureAlignment=%lld;\n"
               "size_t       texturePitchAlignment=%lld;\n"
               "int          multiProcessorCount=%d;\n"
               "int          integrated=%d;\n"
               "int          canMapHostMemory=%d;\n"
               "int          maxTexture1D=%d;\n"
               "int          maxTexture1DMipmap=%d;\n"
               "int          maxTexture2D[2]=[%d %d];\n"
               "int          maxTexture2DMipmap[2]=[%d %d];\n"
               "int          maxTexture2DLinear[3]=[%d %d %d];\n"
               "int          maxTexture2DGather[2]=[%d %d];\n"
               "int          maxTexture3D[3]=[%d %d %d];\n"
               "int          maxTexture3DAlt[3]=[%d %d %d];\n"
               "int          maxTextureCubemap=%d;\n"
               "int          maxTexture1DLayered[2]=[%d %d];\n"
               "int          maxTexture2DLayered[3]=[%d %d %d];\n"
               "int          maxTextureCubemapLayered[2]=[%d %d];\n"
               "int          maxSurface1D=%d;\n"
               "int          maxSurface2D[2]=[%d %d];\n"
               "int          maxSurface3D[3]=[%d %d %d];\n"
               "int          maxSurface1DLayered[2]=[%d %d];\n"
               "int          maxSurface2DLayered[3]=[%d %d %d];\n"
               "int          maxSurfaceCubemap=%d;\n"
               "int          maxSurfaceCubemapLayered[2]=[%d %d];\n"
               "size_t       surfaceAlignment=%lld;\n"
               "int          concurrentKernels=%d;\n"
               "int          ECCEnabled=%d;\n"
               "int          pciBusID=%d;\n"
               "int          pciDeviceID=%d;\n"
               "int          pciDomainID=%d;\n"
               "int          tccDriver=%d;\n"
               "int          asyncEngineCount=%d;\n"
               "int          unifiedAddressing=%d;\n"
               "int          memoryBusWidth=%d;\n"
               "int          l2CacheSize=%d;\n"
               "int          persistingL2CacheMaxSize=%d;\n"
               "int          maxThreadsPerMultiProcessor=%d;\n"
               "int          streamPrioritiesSupported=%d;\n"
               "int          globalL1CacheSupported=%d;\n"
               "int          localL1CacheSupported=%d;\n"
               "size_t       sharedMemPerMultiprocessor=%lld;\n"
               "int          regsPerMultiprocessor=%d;\n"
               "int          managedMemory=%d;\n"
               "int          isMultiGpuBoard=%d;\n"
               "int          multiGpuBoardGroupID=%d;\n"
               "int          hostNativeAtomicSupported=%d;\n"
               "int          pageableMemoryAccess=%d;\n"
               "int          concurrentManagedAccess=%d;\n"
               "int          computePreemptionSupported=%d;\n"
               "int          canUseHostPointerForRegisteredMem=%d;\n"
               "int          cooperativeLaunch=%d;\n"
               "size_t       sharedMemPerBlockOptin=%lld;\n"
               "int          pageableMemoryAccessUsesHostPageTables=%d;\n"
               "int          directManagedMemAccessFromHost=%d;\n"
               "int          maxBlocksPerMultiProcessor=%d;\n"
               "int          accessPolicyMaxWindowSize=%d;\n"
               "size_t       reservedSharedMemPerBlock=%lld;\n"
               "int          hostRegisterSupported=%d;\n"
               "int          sparseCudaArraySupported=%d;\n"
               "int          hostRegisterReadOnlySupported=%d;\n"
               "int          timelineSemaphoreInteropSupported=%d;\n"
               "int          memoryPoolsSupported=%d;\n"
               "int          gpuDirectRDMASupported=%d;\n"
               "unsigned int gpuDirectRDMAFlushWritesOptions=%u;\n"
               "int          gpuDirectRDMAWritesOrdering=%d;\n"
               "unsigned int memoryPoolSupportedHandleTypes=%u;\n"
               "int          deferredMappingCudaArraySupported=%d;\n"
               "int          ipcEventSupported=%d;\n"
               "int          clusterLaunch=%d;\n"
               "int          unifiedFunctionPointers=%d;\n"
               "int          deviceNumaConfig=%d;\n"
               "int          deviceNumaId=%d;\n"
               "int          mpsEnabled=%d;\n"
               "int          hostNumaId=%d;\n"
               "unsigned int gpuPciDeviceID=%u;\n"
               "unsigned int gpuPciSubsystemID=%u;\n"
               "int          hostNumaMultinodeIpcSupported=%d;\n"
               "int          reserved[56]=xxx;\n",
               prop.name, prop.luid, prop.luidDeviceNodeMask, prop.totalGlobalMem, suffix, prop.sharedMemPerBlock, prop.regsPerBlock, prop.warpSize,
               prop.memPitch, prop.maxThreadsPerBlock, prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2], prop.maxGridSize[0],
               prop.maxGridSize[1], prop.maxGridSize[2], prop.totalConstMem, prop.major, prop.minor, prop.textureAlignment, prop.texturePitchAlignment,
               prop.multiProcessorCount, prop.integrated, prop.canMapHostMemory, prop.maxTexture1D, prop.maxTexture1DMipmap, prop.maxTexture2D[0],
               prop.maxTexture2D[1], prop.maxTexture2DMipmap[0], prop.maxTexture2DMipmap[1], prop.maxTexture2DLinear[0], prop.maxTexture2DLinear[1],
               prop.maxTexture2DLinear[2], prop.maxTexture2DGather[0], prop.maxTexture2DGather[1], prop.maxTexture3D[0], prop.maxTexture3D[1],
               prop.maxTexture3D[2], prop.maxTexture3DAlt[0], prop.maxTexture3DAlt[1], prop.maxTexture3DAlt[2], prop.maxTextureCubemap,
               prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1], prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2],
               prop.maxTextureCubemapLayered[0], prop.maxTextureCubemapLayered[1], prop.maxSurface1D, prop.maxSurface2D[0], prop.maxSurface2D[1],
               prop.maxSurface3D[0], prop.maxSurface3D[1], prop.maxSurface3D[2], prop.maxSurface1DLayered[0], prop.maxSurface1DLayered[1],
               prop.maxSurface2DLayered[0], prop.maxSurface2DLayered[1], prop.maxSurface2DLayered[2], prop.maxSurfaceCubemap, prop.maxSurfaceCubemapLayered[0],
               prop.maxSurfaceCubemapLayered[1], prop.surfaceAlignment, prop.concurrentKernels, prop.ECCEnabled, prop.pciBusID, prop.pciDeviceID,
               prop.pciDomainID, prop.tccDriver, prop.asyncEngineCount, prop.unifiedAddressing, prop.memoryBusWidth, prop.l2CacheSize,
               prop.persistingL2CacheMaxSize, prop.maxThreadsPerMultiProcessor, prop.streamPrioritiesSupported, prop.globalL1CacheSupported,
               prop.localL1CacheSupported, prop.sharedMemPerMultiprocessor, prop.regsPerMultiprocessor, prop.managedMemory, prop.isMultiGpuBoard,
               prop.multiGpuBoardGroupID, prop.hostNativeAtomicSupported, prop.pageableMemoryAccess, prop.concurrentManagedAccess,
               prop.computePreemptionSupported, prop.canUseHostPointerForRegisteredMem, prop.cooperativeLaunch, prop.sharedMemPerBlockOptin,
               prop.pageableMemoryAccessUsesHostPageTables, prop.directManagedMemAccessFromHost, prop.maxBlocksPerMultiProcessor,
               prop.accessPolicyMaxWindowSize, prop.reservedSharedMemPerBlock, prop.hostRegisterSupported, prop.sparseCudaArraySupported,
               prop.hostRegisterReadOnlySupported, prop.timelineSemaphoreInteropSupported, prop.memoryPoolsSupported, prop.gpuDirectRDMASupported,
               prop.gpuDirectRDMAFlushWritesOptions, prop.gpuDirectRDMAWritesOrdering, prop.memoryPoolSupportedHandleTypes,
               prop.deferredMappingCudaArraySupported, prop.ipcEventSupported, prop.clusterLaunch, prop.unifiedFunctionPointers, prop.deviceNumaConfig,
               prop.deviceNumaId, prop.mpsEnabled, prop.hostNumaId, prop.gpuPciDeviceID, prop.gpuPciSubsystemID, prop.hostNumaMultinodeIpcSupported);
    }
}

void finishCUDA()
{
    // Must call cudaDeviceReset before exiting in order for profiling and tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaDeviceReset();
}
