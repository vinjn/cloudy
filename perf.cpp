#if defined(_WIN32)
#include "windows-desktop-x64/nvperf_host_impl.h"
#elif defined(__linux__)
#include "linux-desktop-x64/nvperf_host_impl.h"
#endif

extern "C"
{
#include "perf.h"
}
#include "NvPerfReportGenerator.h"
#include "NvPerfPeriodicSamplerGpu.h"
#include "NvPerfOpenGL.h"

//nv::perf::profiler::ReportGeneratorOpenGL m_nvperf;
double m_nvperfWarmupTime = 0.5; // Wait 0.5s to allow the clock to stabalize before begining to profile
NVPW_Device_ClockStatus m_clockStatus = NVPW_DEVICE_CLOCK_STATUS_UNKNOWN; // Used to restore clock state when exiting

using namespace nv::perf;
using namespace nv::perf::sampler;

size_t nsightDeviceIndex = -1;

int setup()
{
    const bool initializeNvPerfResult = InitializeNvPerf();
    if (initializeNvPerfResult)
    {
        const bool openGLLoadDriverResult = OpenGLLoadDriver(); // device periodic sampler requires at least one driver to be loaded
        if (initializeNvPerfResult)
        {
            nsightDeviceIndex = GetCompatibleGpuDeviceIndex();
            if (nsightDeviceIndex == size_t(~0))
            {
                printf("Current device is unsupported, test is skipped.\n");
                return 1;
            }

            const size_t MaxNumUndecodedSamples = 1024;
            const size_t RecordBufferSize = 256 * 1024 * 1024; // 256 MB
        }
    }
}

size_t GetCompatibleGpuDeviceIndex()
{
    NVPW_GetDeviceCount_Params getDeviceCountParams = { NVPW_GetDeviceCount_Params_STRUCT_SIZE };
    NVPA_Status nvpaStatus = NVPW_GetDeviceCount(&getDeviceCountParams);
    if (nvpaStatus)
    {
        NV_PERF_LOG_ERR(50, "Failed NVPW_GetDeviceCount: %u\n", nvpaStatus);
        return size_t(~0);
    }

    for (size_t deviceIndex = 0; deviceIndex < getDeviceCountParams.numDevices; ++deviceIndex)
    {
        if (GpuPeriodicSamplerIsGpuSupported(deviceIndex))
        {
            return deviceIndex;
        }
    }
    return size_t(~0);
}
