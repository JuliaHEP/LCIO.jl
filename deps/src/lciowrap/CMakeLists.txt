project(lciowrapper)

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MACOSX_RPATH 0)

list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
add_definitions(-DJULIA_ENABLE_THREADING)

if(WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=pentium4")
endif()

find_package(CxxWrap)
set(LCIO_DIR "${LCIO}")
find_package(LCIO)

# this helps Libdl.dlopen find the right paths
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${CxxWrap_DIR}/../;${LCIO_LIBRARY_DIRS}/lib")

include_directories(${LCIO_INCLUDE_DIRS})

add_library(lciowrap SHARED lciowrap.cc )
target_link_libraries(lciowrap CxxWrap::cxx_wrap ${LCIO_LCIO_LIBRARY} ${LCIO_SIO_LIBRARY})

install(TARGETS
  lciowrap
LIBRARY DESTINATION lib${LIBDIR_SUFFIX}
ARCHIVE DESTINATION lib${LIBDIR_SUFFIX}
RUNTIME DESTINATION lib${LIBDIR_SUFFIX}
INCLUDES DESTINATION include)
