project(MovieThumbs-Core)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

add_definitions (${QT_DEFINITIONS})

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
    ${QT_INCLUDES}
    ${qjson_INCLUDE_DIR}
    ${QJSON_INCLUDE_DIR}
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

QT4_AUTOMOC(${MOVIETHUMBS_CORE_SRCS})

add_library(moviethumbs SHARED ${MOVIETHUMBS_CORE_SRCS})

target_link_libraries(moviethumbs
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}
   ${QT_QTNETWORK_LIBRARY}
   ${QJSON_LIBRARIES}
   ${qjson_LIBRARIES} #Support debian
   )

install(TARGETS moviethumbs LIBRARY DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${MOVIETHUMBS_INCLUDES} DESTINATION include/libmoviethumbs)
install(FILES blacklist DESTINATION "/usr/share/MovieThumbs/")
