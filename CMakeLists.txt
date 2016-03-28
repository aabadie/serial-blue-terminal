cmake_minimum_required(VERSION 3.0.2)

project(SerialBlueTerminal VERSION "0.1.0")

option(BUILD_DOCUMENTATION "Add target 'doc' to build documentation.")
option(BUILD_PACKAGE       "Add target 'package' for package generation.")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
if (WIN32)
  set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
else()
  set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
endif()
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)


# CMake module path setup
set(${PROJECT_NAME}_CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_CMAKE_MODULE_PATH}")


# Import external libraries, e.g Qt
find_package( Qt5 REQUIRED COMPONENTS Core Widgets Bluetooth SerialPort)
get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake LOCATION)
get_filename_component(QT5_BINARY_DIR ${QT5_QMAKE_EXECUTABLE} PATH)


# include lib directory
include_directories(
  ${${PROJECT_NAME}_SOURCE_DIR}/lib
)

# include GNU standard installation directories definitions
include(GNUInstallDirs)


# CPack includes
if(BUILD_PACKAGE)
  include(SerialBlueTermCPackConfig)
endif()


# Documentation generation part
if(BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()


# process subfolders
add_subdirectory(lib)
add_subdirectory(app)
