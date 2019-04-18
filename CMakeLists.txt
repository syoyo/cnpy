CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(CNPY)

set(CMAKE_CXX_STANDARD 11)

option(CNPY_ENABLE_STATIC "Build static (.a) library" ON)
option(CNPY_ENABLE_ZLIB "Build with zlib support(for .npz)" ON)
option(CNPY_ENABLE_MMAP_EXAMPLE "Build an eample using memory API(Linux only for now)" OFF)

if (CNPY_ENABLE_ZLIB)
  add_definitions(-DCNPY_WITH_ZLIB)
  find_package(ZLIB REQUIRED)
  include_directories(${ZLIB_INCLUDE_DIRS})
endif (CNPY_ENABLE_ZLIB)

add_library(cnpy SHARED "cnpy.cpp")

if (CNPY_ENABLE_ZLIB)
  target_link_libraries(cnpy ${ZLIB_LIBRARIES})
endif (CNPY_ENABLE_ZLIB)

install(TARGETS "cnpy" LIBRARY DESTINATION lib PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

if(CNPY_ENABLE_STATIC)
    add_library(cnpy-static STATIC "cnpy.cpp")
    set_target_properties(cnpy-static PROPERTIES OUTPUT_NAME "cnpy")
    install(TARGETS "cnpy-static" ARCHIVE DESTINATION lib)
endif(CNPY_ENABLE_STATIC)

install(FILES "cnpy.h" DESTINATION include)
install(FILES "mat2npz" "npy2mat" "npz2mat" DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

add_executable(example1 example1.cpp)
target_link_libraries(example1 cnpy)

if (CNPY_ENABLE_MMAP_EXAMPLE)
  add_executable(example_mmap example_mmap.cpp)
  target_link_libraries(example_mmap cnpy)
endif(CNPY_ENABLE_MMAP_EXAMPLE)
