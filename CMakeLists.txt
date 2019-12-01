cmake_minimum_required(VERSION 3.1.0)

get_filename_component(APPLICATION ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${APPLICATION})

add_subdirectory(attractor)
add_subdirectory(force)
add_subdirectory(pendulum)
add_subdirectory(ship)
