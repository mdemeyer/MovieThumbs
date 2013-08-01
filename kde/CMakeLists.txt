project(MovieThumbs-KDE)

cmake_minimum_required(VERSION 2.8.0)

find_package(KDE4 REQUIRED)
find_library(MOVIETHUMBS_LIBRARY moviethumbs)

include (KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${QT_INCLUDES} ${INCLUDE_INSTALL_DIR}/libmoviethumbs/)

set(MovieThumbs_SRCS moviethumbs.cpp)

kde4_add_plugin(MovieThumbs ${MovieThumbs_SRCS})

target_link_libraries(MovieThumbs
   ${KDE4_KIO_LIBS}
   ${KDE4_SOLID_LIBS}
   ${QT_QTCORE_LIBRARY}
   ${MOVIETHUMBS_LIBRARY}
   )

install(TARGETS MovieThumbs DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES MovieThumbs.desktop DESTINATION ${SERVICES_INSTALL_DIR})
