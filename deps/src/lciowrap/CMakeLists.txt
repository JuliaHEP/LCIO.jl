project(lciowrapper)

cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MACOSX_RPATH 0)
list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
add_definitions(-DJULIA_ENABLE_THREADING)

set(LCIO_DIR "${LCIO_INSTALLDIR}")
find_package(LCIO REQUIRED)

# JLCxx_DIR needs to be passed in with -D
find_package(JLCxx REQUIRED )

# this helps Libdl.dlopen find the right paths
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${LCIO_LIBRARY_DIRS}")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${INTERFACE_INCLUDE_DIRECTORIES})
include_directories(${LCIO_INCLUDE_DIRS})
include_directories(${Julia_INCLUDE_DIRS})

add_library(lciowrap SHARED lciowrap.cc )
target_link_libraries(lciowrap ${LCIO_LCIO_LIBRARY} ${LCIO_SIO_LIBRARY} JlCxx::jlcxx)

install(TARGETS
  lciowrap
LIBRARY DESTINATION lib${LIBDIR_SUFFIX}
ARCHIVE DESTINATION lib${LIBDIR_SUFFIX}
RUNTIME DESTINATION lib${LIBDIR_SUFFIX}
INCLUDES DESTINATION include)
