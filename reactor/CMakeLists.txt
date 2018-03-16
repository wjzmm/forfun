cmake_minimum_required (VERSION 2.8)

projcet (reactor)

aux_source_directory(. DIR_SRCS)

add_subdirectory(network)
add_subdirectory(common)
add_subdirectory(utils)

add_executable(server ${DIR_SRCS})


