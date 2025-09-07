add_library(libdbg libdbg.cc)
add_library(minidbg::libdbg ALIAS libdbg)

target_include_directories(libdbg
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

set_target_properties(libdbg
  PROPERTIES OUTPUT_NAME dbg
)

include(GNUInstallDirs)
install(TARGETS libdbg
  EXPORT libdbg-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
)
install(
  EXPORT libdbg-targets
  FILE minidbg-config.cmake
  NAMESPACE minidbg::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/minidbg
)
