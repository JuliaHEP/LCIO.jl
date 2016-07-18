project(lciowrap)

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_MACOSX_RPATH 1)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-DJULIA_ENABLE_THREADING)

if(WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=pentium4")
endif()

find_package(CxxWrap)

set(LCIO_Dir "${LCIO}")
find_package(LCIO)

include_directories(${LCIO_INCLUDE_DIRS})

add_library(lciowrap SHARED lciowrap.cc )
target_link_libraries(lciowrap CxxWrap::cxx_wrap ${LCIO_LCIO_LIBRARY} ${LCIO_SIO_LIBRARY})

install(TARGETS
  lciowrap
LIBRARY DESTINATION lib${LIBDIR_SUFFIX}
ARCHIVE DESTINATION lib${LIBDIR_SUFFIX}
RUNTIME DESTINATION lib${LIBDIR_SUFFIX}
INCLUDES DESTINATION include)
