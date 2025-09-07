add_executable(tests tests.cpp)

target_link_libraries(
  tests
  PRIVATE minidbg::libdbg
          Catch2::Catch2WithMain)

add_test(NAME Tests COMMAND tests)
