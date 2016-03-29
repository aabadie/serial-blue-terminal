cmake_minimum_required(VERSION 3.0.2)

project(sbterm)

set(${PROJECT_NAME}_RCCS
    ../ressources/sbterm.qrc
)

qt5_add_resources(${PROJECT_NAME}_RCC_SRCS ${${PROJECT_NAME}_RCCS})

set(${PROJECT_NAME}_HDRS
)

set(${PROJECT_NAME}_SRCS
    main.cpp
)

set(${PROJECT_NAME}_MOC_HDRS   
)

set(${PROJECT_NAME}_MOC_SRCS
)

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HDRS}
  ${${PROJECT_NAME}_MOC_HDRS}
)

set(${PROJECT_NAME}_SOURCES
  ${${PROJECT_NAME}_SRCS}
  ${${PROJECT_NAME}_MOC_SRCS}
  ${${PROJECT_NAME}_RCC_SRCS}
)

set(${PROJECT_NAME}_LIBRARIES
  Qt5::Core
  Qt5::Widgets
  Qt5::Bluetooth
  Qt5::SerialPort
  sbterm-common
)

include_directories(${PROJECT_SOURCE_DIR})

# Compilation rules
add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})


# Include header files for Visual Studio.
source_group("Header Files" REGULAR_EXPRESSION .*\\.h\$)
source_group("Generated Files" FILES ${${PROJECT_NAME}_MOC_SRCS})


# Installation rules
if(WIN32)
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
  # Also copy dependencies when generating an auto-installer package
  if(BUILD_PACKAGE)
    include(GetPrerequisites)
    get_prerequisites(${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.dll WIN32_PREREQUISITES 1 0 "" "")
    foreach(DEPENDENCY_FILE ${WIN32_PREREQUISITES})
      gp_resolve_item("${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.dll" "${DEPENDENCY_FILE}" "" "" resolved_file)
      install(FILES ${resolved_file} DESTINATION bin)
    endforeach()
  endif()
else()
  install(TARGETS ${PROJECT_NAME}
      RUNTIME
      DESTINATION bin
      COMPONENT binary)
endif()
