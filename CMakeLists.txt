cmake_minimum_required(VERSION 2.8)

project (vtksegmentation)

find_package (VTK REQUIRED)
include (${VTK_USE_FILE})

add_executable (vtksegmentation vtksegmentation.cxx)

target_link_libraries (vtksegmentation ${VTK_LIBRARIES})
