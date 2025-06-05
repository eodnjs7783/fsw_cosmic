cmake_minimum_required(VERSION 3.0)
project(hvd_lib C)

# The shared OSAL and cFE include directories should always be used
# Note that this intentionally does NOT include PSP-specific includes, just the generic
include_directories(${CFECORE_SOURCE_DIR}/src/inc)
include_directories(${CFEPSP_SOURCE_DIR}/fsw/inc)

include_directories(${cfs_lib_MISSION_DIR}/fsw/inc)
include_directories(${ifc_lib_MISSION_DIR}/fsw/inc)
#include_directories(device/inc)
include_directories(fsw/inc)

include_directories(../../submodules/libgscsp/lib/libgscsp/include)
include_directories(../../submodules/libgscsp/lib/libgscsp/lib/libcsp/include)
include_directories(../../submodules/libgscsp/lib/libparam_client/include)
include_directories(../../submodules/libgscsp/lib/libutil/include)
include_directories(../../submodules/libgscsp/build/lib/libgscsp/lib/libcsp/include)
include_directories(../../submodules/libgscsp/lib/libp60_client/include)
include_directories(../eps/device/inc)
include_directories(../eps/device/payr)

file(GLOB_RECURSE HVD_LIB_SRC fsw/src/*.c)

# Create the app module
add_cfe_app(hvd_lib ${HVD_LIB_SRC})
