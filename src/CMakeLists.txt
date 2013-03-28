set(MovieThumbs_SRCS movieThumbs.cpp tmdbThumb.cpp)

kde4_add_plugin(MovieThumbs ${MovieThumbs_SRCS})

target_link_libraries(MovieThumbs 
   ${KDE4_KIO_LIBS}
   ${KDE4_SOLID_LIBS}
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}
   ${QT_QTNETWORK_LIBRARY}
   ${QJSON_LIBRARIES}
   ${qjson_LIBRARIES} #Support debian
   )

install(TARGETS MovieThumbs DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES MovieThumbs.desktop DESTINATION ${SERVICES_INSTALL_DIR})


