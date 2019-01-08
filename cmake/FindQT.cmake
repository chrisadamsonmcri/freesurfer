# Qt Find Module

if(NOT Qt5_DIR)
  # default search path
  if(EXISTS ${FS_PACKAGES_DIR}/qt/5.11/lib/cmake/Qt5)
    set(Qt5_DIR ${FS_PACKAGES_DIR}/qt/5.11/lib/cmake/Qt5)
  elseif(EXISTS ${FS_PACKAGES_DIR}/qt/5.6/lib/cmake/Qt5)
    set(Qt5_DIR ${FS_PACKAGES_DIR}/qt/5.6/lib/cmake/Qt5)
  endif()
endif()

# find Qt components
set(_qt_components Core Widgets)
if(NOT APPLE)
  set(_qt_components ${_qt_components} X11Extras)
endif()

find_package(Qt5 COMPONENTS ${_qt_components})

# trace back root of install
set(Qt5_INSTALL_DIR ${_qt5_install_prefix})

# install the shared libraries to the freesurfer lib directory
if(Qt5_FOUND AND NOT APPLE)
  file(GLOB _qt_libs_to_install "${Qt5_INSTALL_DIR}/lib/lib*.so*")
  if(_qt_libs_to_install)
    install(PROGRAMS ${_qt_libs_to_install} DESTINATION lib/qt/lib)
    # add Qt library directory to rpath
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib/qt/lib:${CMAKE_INSTALL_RPATH}")
    # install the platform plugins as well, and make sure executables know
    # where to find the plugins directory
    if(EXISTS ${Qt5_INSTALL_DIR}/plugins/platforms)
      install(DIRECTORY ${Qt5_INSTALL_DIR}/plugins/platforms DESTINATION lib/qt/plugins)
      install(FILES ${CMAKE_SOURCE_DIR}/qt/qt.conf DESTINATION bin)
    endif()
  endif()
endif()
