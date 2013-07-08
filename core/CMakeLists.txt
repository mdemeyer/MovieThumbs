project(MovieThumbs-Core)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

add_definitions (${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES})

set(MOVIETHUMBS_CORE_SRCS
    fileparser.cpp
    movieclient.cpp
    movieservice.cpp
    posterservice.cpp
    tvservice.cpp
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

install(TARGETS moviethumbs LIBRARY DESTINATION lib)
