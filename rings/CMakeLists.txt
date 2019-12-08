cmake_minimum_required(VERSION 3.1.0)

get_filename_component(APPLICATION ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${APPLICATION})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB CURRENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${APPLICATION}
    ${CURRENT_SOURCES}
)

target_link_libraries(${APPLICATION} Qt5::Widgets) 
