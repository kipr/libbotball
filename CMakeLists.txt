cmake_minimum_required(VERSION 2.8)

project(libbotball)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Aurora REQUIRED)

set(INCLUDE_DIR ${libbotball_SOURCE_DIR}/include)
set(SRC_DIR ${libbotball_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})
include_directories(${AURORA_INCLUDE_DIRS})


file(GLOB SRCS ${SRC_DIR}/*.cpp)

# Cross-compile
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


# C++11
# http://www.guyrutenberg.com/2014/01/05/enabling-c11-c0x-in-cmake/
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

# Library: libbotball
add_library(botball SHARED ${SRCS})

target_link_libraries(botball ${AURORA_LIBRARIES})

install(TARGETS botball DESTINATION lib)
install(DIRECTORY ${INCLUDE_DIR}/botball DESTINATION include)

