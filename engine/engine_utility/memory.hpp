#pragma once

#include <memory>
#include <iomanip>
#include <sstream>

#ifdef __APPLE__
    #include <mach/mach.h>
    #include <sys/resource.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
#endif

// Peak Memory
inline double getPeakMemoryMiB() {

    // Mac
    #ifdef __APPLE__
    struct rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) return static_cast<double>(usage.ru_maxrss) / (1024.0 * 1024.0);

    // Windows
    #elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS counters{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters))) return static_cast<double>(counters.PeakWorkingSetSize) / (1024.0 * 1024.0);

    #endif
    return 0.0;
};

// Physical Memory
inline double getPhysicalMemoryMiB() {

    // Mac
    #ifdef __APPLE__
    task_vm_info_data_t info{};
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;

    if (task_info(mach_task_self(), TASK_VM_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS) return static_cast<double>(info.phys_footprint) / (1024.0 * 1024.0);

    // Windows
    #elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters))) return static_cast<double>(counters.PrivateUsage) / (1024.0 * 1024.0);

    #endif
    return 0.0;
};

// Resident Memory
inline double getCurrentResidentMiB() {

    // Mac
    #ifdef __APPLE__
    mach_task_basic_info_data_t info{};
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS) return static_cast<double>(info.resident_size) / (1024.0 * 1024.0);

    // Windows
    #elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS counters{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters))) return static_cast<double>(counters.WorkingSetSize) / (1024.0 * 1024.0);

    #endif
    return 0.0;
};
