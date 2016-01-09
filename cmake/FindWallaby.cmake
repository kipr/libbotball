find_path(WALLABY_INCLUDE_DIR wallaby/wallaby.h)
find_library(WALLABY_LIBRARY NAMES wallaby libwallaby)
if(WIN32)
  find_program(WALLABY_DLL wallaby.dll)
endif()

set(WALLABY_LIBRARIES ${WALLABY_LIBRARY})
set(WALLABY_INCLUDE_DIRS ${WALLABY_INCLUDE_DIR})

if(WIN32)
  set(WALLABY_DLLS ${WALLABY_DLL})
endif()

include(FindPackageHandleStandardArgs)

if(WIN32)
  find_package_handle_standard_args(Wallaby DEFAULT_MSG WALLABY_LIBRARY WALLABY_INCLUDE_DIR WALLABY_DLL)
  mark_as_advanced(WALLABY_INCLUDE_DIR WALLABY_LIBRARY WALLABY_DLL)
else()
  find_package_handle_standard_args(Wallaby DEFAULT_MSG WALLABY_LIBRARY WALLABY_INCLUDE_DIR)
  mark_as_advanced(WALLABY_INCLUDE_DIR WALLABY_LIBRARY)
endif()
