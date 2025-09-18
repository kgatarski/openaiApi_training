cmake_minimum_required(VERSION 3.16)

project(cpp_recall LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(cpp_recall
  main.cpp
  general.cpp
  shared_ptr_test.cpp
  unique_ptr_test.cpp
  weak_ptr_test.cpp
)
target_link_libraries(cpp_recall Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS cpp_recall
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(tests.cmake)
