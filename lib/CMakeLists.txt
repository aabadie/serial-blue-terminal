cmake_minimum_required(VERSION 3.0.2)

project(sbterm-common)

set(${PROJECT_NAME}_RCCS
    ../ressources/sbterm.qrc
)

set(${PROJECT_NAME}_HDRS
    sbTermPortSettings.h
    sbTermSettingsController.h
)

set(${PROJECT_NAME}_SRCS
    sbTermSettingsController.cpp
)

set(${PROJECT_NAME}_MOC_HDRS   
    sbTermMainWindow.h
    sbTermConsole.h
)

set(${PROJECT_NAME}_MOC_SRCS
  sbTermMainWindow.cpp
  sbTermConsole.cpp
)

qt5_add_resources(${PROJECT_NAME}_RCC_SRCS ${${PROJECT_NAME}_RCCS})
qt5_wrap_cpp( ${PROJECT_NAME}_MOC_SRCS ${${PROJECT_NAME}_MOC_HDRS})

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HDRS}
  ${${PROJECT_NAME}_MOC_HDRS}
  ${${PROJECT_NAME}_UI_HDRS}
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
)


# Compilation rules
add_definitions(-DQT_MESSAGELOGCONTEXT)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})


# Include header files for Visual Studio.
source_group("Header Files" REGULAR_EXPRESSION .*\\.h\$)
source_group("Generated Files" FILES ${${PROJECT_NAME}_MOC_SRCS})


# Installation rules
if (WIN32)
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin
		 		  LIBRARY DESTINATION lib
                                  ARCHIVE DESTINATION lib)
  # Also copy dependencies when generating an auto-installer package
  if(BUILD_PACKAGE)
    include(GetPrerequisites)
    get_prerequisites(${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.dll WIN32_PREREQUISITES 1 0 "" "")
    foreach(DEPENDENCY_FILE ${WIN32_PREREQUISITES})
      gp_resolve_item("${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.dll" "${DEPENDENCY_FILE}" "" "" resolved_file)
      install(FILES ${resolved_file} DESTINATION bin)
    endforeach()
  endif()
else ()
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif ()