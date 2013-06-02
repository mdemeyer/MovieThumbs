macro_bool_to_01(LibTvdb_FOUND HAVE_TVDB)
#Header file to read version number settings
configure_file(
    MovieThumbsConfig.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MovieThumbsConfig.h
    )

set(MovieThumbs_SRCS fileparser.cpp movieThumbs.cpp tmdbThumb.cpp)

if(LibTvdb_FOUND)
   set(MovieThumbs_SRCS ${MovieThumbs_SRCS} tvdbFetcher.cpp)
   set(EXTRA_LIBS ${EXTRA_LIBS} ${LIBTVDB_LIBRARIES})
   set(HAVE_TVDB 1)
endif(LibTvdb_FOUND)

kde4_add_plugin(MovieThumbs ${MovieThumbs_SRCS})

target_link_libraries(MovieThumbs 
   ${KDE4_KIO_LIBS}
   ${KDE4_SOLID_LIBS}
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}
   ${QT_QTNETWORK_LIBRARY}
   ${QJSON_LIBRARIES}
   ${qjson_LIBRARIES} #Support debian
   ${EXTRA_LIBS}
   )

install(TARGETS MovieThumbs DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES MovieThumbs.desktop DESTINATION ${SERVICES_INSTALL_DIR})
