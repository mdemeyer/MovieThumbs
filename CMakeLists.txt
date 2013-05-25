cmake_minimum_required(VERSION 2.6.2)
project(MovieThumbs)

#Version number
set(MOVIETHUMBS_VERSION_MAJOR 0)
set(MOVIETHUMBS_VERSION_MINOR 2)
set(MOVIETHUMBS_VERSION_RELEASE 0)
set(MOVIETHUMBS_VERSION "${MOVIETHUMBS_VERSION_MAJOR}.${MOVIETHUMBS_VERSION_MINOR}.${MOVIETHUMBS_VERSION_RELEASE}")

find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

macro_optional_find_package(LibTvdb)
set_package_properties(LibTvdb PROPERTIES DESCRIPTION "A small library to fetch information from the thetvdb.com"
                       URL "http://sourceforge.net/projects/libtvdb/"
                       TYPE OPTIONAL
                       PURPOSE "Required to fetch TV series posters"
                      )

include (KDE4Defaults)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${QT_INCLUDES} ${PROJECT_BINARY_DIR})

add_subdirectory( src )


