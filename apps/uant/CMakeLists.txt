cmake_minimum_required(VERSION 3.0)
project(uant C)

# The shared OSAL and cFE include directories should always be used
# Note that this intentionally does NOT include PSP-specific includes, just the generic
include_directories(${CFECORE_SOURCE_DIR}/src/inc)
include_directories(${CFEPSP_SOURCE_DIR}/fsw/inc)

include_directories(${cfs_lib_MISSION_DIR}/fsw/inc)
include_directories(${ifc_lib_MISSION_DIR}/fsw/inc)
include_directories(${hyvrid_lib_MISSION_DIR}/fsw/inc)
include_directories(device/inc)
include_directories(fsw/inc)

file(GLOB_RECURSE UANT_DEV_SRC device/*.c)
file(GLOB_RECURSE UANT_APP_SRC fsw/*.c)

# Create the app module
add_cfe_app(uant ${UANT_DEV_SRC} ${UANT_APP_SRC})