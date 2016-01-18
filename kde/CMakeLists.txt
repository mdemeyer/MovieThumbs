project(MovieThumbs-KDE)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.1.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Network
    )

find_package(KF5 REQUIRED COMPONENTS
    KIO
    KDELibs4Support
    )

find_library(MOVIETHUMBS_LIBRARY moviethumbs)

include_directories(
    ${CMAKE_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Core_INCLUDES}
    ${Qt5Gui_INCLUDES}
    ${Qt5Network_INCLUDES}
    ${INCLUDE_INSTALL_DIR}/libmoviethumbs/
    )

set(MovieThumbs_SRCS moviethumbs.cpp)

add_library(MovieThumbs MODULE ${MovieThumbs_SRCS})

target_link_libraries(MovieThumbs
   KF5::KIOWidgets
   KF5::KDELibs4Support
   ${Qt5Core_LIBRARIES}
   ${Qt5Gui_LIBRARIES}
   ${Qt5Network_LIBRARIES}
   ${MOVIETHUMBS_LIBRARY}
   )

install(TARGETS MovieThumbs DESTINATION ${CMAKE_INSTALL_PLUGINDIR})
install(FILES MovieThumbs.desktop DESTINATION ${CMAKE_INSTALL_KSERVICES5DIR})
