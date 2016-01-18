project(MovieThumbs-Core)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.3.0")
set(CMAKE_AUTOMOC ON)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Network
    )

if(NOT LIB_INSTALL_DIR)
    get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
    if(LIB64)
        set(LIB_INSTALL_DIR lib64)
    else()
        set(LIB_INSTALL_DIR lib)
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Core_INCLUDES}
    ${Qt5Network_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set(MOVIETHUMBS_CORE_SRCS
    fileparser.cpp
    movieclient.cpp
    tmdbmanager.cpp
    downloadmanager.cpp
    )

set(MOVIETHUMBS_INCLUDES
    fileparser.h
    movieclient.h
    tmdbmanager.h
    downloadmanager.h
    )

add_library(moviethumbs SHARED ${MOVIETHUMBS_CORE_SRCS})

target_link_libraries(moviethumbs
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   )

install(TARGETS moviethumbs LIBRARY DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${MOVIETHUMBS_INCLUDES} DESTINATION include/libmoviethumbs)
install(FILES blacklist DESTINATION "/usr/share/MovieThumbs/")
