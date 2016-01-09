find_path(AURORA_INCLUDE_DIR aurora/graphics.h)
find_library(AURORA_LIBRARY NAMES aurora libaurora)
if(WIN32)
  find_program(AURORA_DLL aurora.dll)
endif()

set(AURORA_LIBRARIES ${AURORA_LIBRARY})
set(AURORA_INCLUDE_DIRS ${AURORA_INCLUDE_DIR})

if(WIN32)
  set(AURORA_DLLS ${AURORA_DLL})
endif()

include(FindPackageHandleStandardArgs)

if(WIN32)
  find_package_handle_standard_args(Aurora DEFAULT_MSG AURORA_LIBRARY AURORA_INCLUDE_DIR AURORA_DLL)
  mark_as_advanced(AURORA_INCLUDE_DIR AURORA_LIBRARY AURORA_DLL)
else()
  find_package_handle_standard_args(Aurora DEFAULT_MSG AURORA_LIBRARY AURORA_INCLUDE_DIR)
  mark_as_advanced(AURORA_INCLUDE_DIR AURORA_LIBRARY)
endif()
