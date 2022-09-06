cmake_minimum_required(VERSION 3.0.0)
project(logger VERSION 0.1.0)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(logger main.c lib/global.h lib/tail.h lib/view.h)
target_link_libraries(logger ${CURSES_LIBRARY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(logger ncurses)
