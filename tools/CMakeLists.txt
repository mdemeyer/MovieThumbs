project(MovieThumbs-Tools)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.3.0")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
    Network
    )

find_library(MOVIETHUMBS_LIBRARY moviethumbs)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif()

include_directories(${CMAKE_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${Qt5Core_INCLUDES}
    ${Qt5Network_INCLUDES}
    ${INCLUDE_INSTALL_DIR}/libmoviethumbs/)

set(MOVIETHUMBS_TOOLS_SRCS main.cpp tools.cpp)
set(MOVIETHUMBS_TOOLS_HEADERS tools.h)

QT5_WRAP_CPP(MOVIETHUMBS_HEADERS_MOC ${MOVIETHUMBS_TOOLS_HEADERS})

add_executable(moviethumbs ${MOVIETHUMBS_TOOLS_SRCS} ${MOVIETHUMBS_HEADERS_MOC})

target_link_libraries(moviethumbs
   ${Qt5Core_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
   ${Qt5Network_INCLUDES}
   ${MOVIETHUMBS_LIBRARY}
   )

install(TARGETS moviethumbs RUNTIME DESTINATION bin)
install(FILES gnome-moviethumbs.thumbnailer DESTINATION /usr/share/thumbnailers)
