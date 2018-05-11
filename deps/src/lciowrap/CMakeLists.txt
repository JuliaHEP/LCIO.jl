project(lciowrapper)

cmake_minimum_required(VERSION 2.8.12)
# this helps Libdl.dlopen find the right paths
set(CMAKE_MACOSX_RPATH 0)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
add_definitions(-DJULIA_ENABLE_THREADING)

find_package(LCIO REQUIRED)

# JlCxx_DIR needs to be passed in with -D
find_package(JlCxx REQUIRED )

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${LCIO_LIBRARY_DIRS};${Julia_LIBRARY_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${INTERFACE_INCLUDE_DIRECTORIES})
include_directories(${LCIO_INCLUDE_DIRS})
# include_directories(${Julia_INCLUDE_DIRS})

add_library(lciowrap SHARED lciowrap.cc )
target_link_libraries(lciowrap ${LCIO_LCIO_LIBRARY} ${LCIO_SIO_LIBRARY} JlCxx::cxxwrap_julia)

install(TARGETS
  lciowrap
LIBRARY DESTINATION lib${LIBDIR_SUFFIX}
ARCHIVE DESTINATION lib${LIBDIR_SUFFIX}
RUNTIME DESTINATION lib${LIBDIR_SUFFIX}
INCLUDES DESTINATION include)
