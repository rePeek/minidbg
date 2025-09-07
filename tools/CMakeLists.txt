add_executable(mini-dbg minidbg.cc)
target_link_libraries(mini-dbg
  PRIVATE minidbg::libdbg
          PkgConfig::libedit)

include(GNUInstallDirs)
install(TARGETS mini-dbg
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
