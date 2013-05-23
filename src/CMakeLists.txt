set(MovieThumbs_SRCS fileparser.cpp movieThumbs.cpp tmdbThumb.cpp tvdbFetcher.cpp)

kde4_add_plugin(MovieThumbs ${MovieThumbs_SRCS})

macro_optional_find_package(LibTvdb)
set_package_properties(LibTvdb PROPERTIES DESCRIPTION "A small library to fetch information from the thetvdb.com"
                       URL "http://sourceforge.net/projects/libtvdb/"
                       TYPE OPTIONAL
                       PURPOSE "Required to fetch TV series posters"
                      )

target_link_libraries(MovieThumbs 
   ${KDE4_KIO_LIBS}
   ${KDE4_SOLID_LIBS}
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}
   ${QT_QTNETWORK_LIBRARY}
   ${QJSON_LIBRARIES}
   ${qjson_LIBRARIES} #Support debian
   ${LIBTVDB_LIBRARIES}
   )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(TARGETS MovieThumbs DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES MovieThumbs.desktop DESTINATION ${SERVICES_INSTALL_DIR})


