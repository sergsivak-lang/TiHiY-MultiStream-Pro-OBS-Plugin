cmake_minimum_required(VERSION 3.28)

project(tihiy-multistream-pro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# This CMake file is intended to be used inside/alongside the official OBS Plugin Template.
# For a production build, use OBS Plugin Template and add these source files to it.

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(tihiy-multistream-pro MODULE
    src/plugin-main.cpp
    src/tihiy-multistream-dock.cpp
    src/tihiy-multistream-dock.hpp
)

target_include_directories(tihiy-multistream-pro PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(tihiy-multistream-pro PRIVATE Qt6::Core Qt6::Widgets OBS::libobs OBS::frontend-api)

set_target_properties(tihiy-multistream-pro PROPERTIES
    OUTPUT_NAME "tihiy-multistream-pro"
    PREFIX ""
)

install(TARGETS tihiy-multistream-pro
    RUNTIME DESTINATION "obs-plugins/64bit"
    LIBRARY DESTINATION "obs-plugins/64bit"
)

install(DIRECTORY data/ DESTINATION "data/obs-plugins/tihiy-multistream-pro")
