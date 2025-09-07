cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

project(mini-dbg
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX C)

# Generate a CompilationDatabase (compile_commands.json file) for our build,
# for use by clang_complete, YouCompleteMe, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  # 保留已有配置并追加隐式路径
  list(APPEND CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
  # 对于C语言项目也需同样处理
  list(APPEND CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

include(Dependencies.cmake)
minidbg_setup_dependencies()

include(CTest)

add_subdirectory(src)
add_subdirectory(tools)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
