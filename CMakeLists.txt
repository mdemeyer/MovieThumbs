project(MovieThumbs)
cmake_minimum_required(VERSION 2.6.2)

find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include (KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${QT_INCLUDES})

add_subdirectory( src )


