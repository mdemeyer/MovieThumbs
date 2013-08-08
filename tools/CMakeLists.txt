project(MovieThumbs-Tools)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)
find_library(MOVIETHUMBS_LIBRARY moviethumbs)

add_definitions (${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${INCLUDE_INSTALL_DIR}/libmoviethumbs/)

set(MOVIETHUMBS_TOOLS_SRCS main.cpp tools.cpp)
set(MOVIETHUMBS_TOOLS_HEADERS tools.h)

QT4_WRAP_CPP(MOVIETHUMBS_HEADERS_MOC ${MOVIETHUMBS_TOOLS_HEADERS})

add_executable(moviethumbs ${MOVIETHUMBS_TOOLS_SRCS} ${MOVIETHUMBS_HEADERS_MOC})

target_link_libraries(moviethumbs
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}
   ${MOVIETHUMBS_LIBRARY}
   )

install(TARGETS moviethumbs RUNTIME DESTINATION bin)
install(FILES gnome-moviethumbs.thumbnailer DESTINATION /usr/share/thumbnailers)
