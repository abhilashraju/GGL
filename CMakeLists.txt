cmake_minimum_required(VERSION 3.1)


project (GGL)

# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 17)
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
# Add an executable
set(SOURCES
    examples/console/graphs.cpp
    # examples/console/mst.cpp
    # examples/console/priorityqueue.cpp
    # examples/console/trie.cpp
    # examples/console/union_find.cpp
    
)
add_executable(GGL ${SOURCES})

# # link against the boost libraries
# target_link_libraries( GGL
#     PRIVATE
# )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts" CACHE STRING "Set C++ Compiler Flags" FORCE)
set(CMAKE_CXX_STANDARD 17)
target_compile_features(GGL PUBLIC cxx_auto_type)
target_include_directories(GGL
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)