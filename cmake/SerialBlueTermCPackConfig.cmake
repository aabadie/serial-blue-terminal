#CPack config file
include(InstallRequiredSystemLibraries)

# Common variables
set(CPACK_GENERATOR "")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(ARCH "x86_64")
else()
    set(ARCH "x86")
endif()

set(CPACK_PACKAGE_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "sbterm-${ARCH}-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "sbterm-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}-src")

set(CPACK_SOURCE_GENERATOR "TGZ;TBZ2;ZIP")
set(CPACK_PACKAGE_CONTACT "alexandre.abadie@inria.fr")

set(CPACK_RESOURCE_FILE_README "${${PROJECT_NAME}_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${${PROJECT_NAME}_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${${PROJECT_NAME}_SOURCE_DIR}/ABOUT")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A serial and rfcomm communication application based on Qt.")
set(CPACK_RESOURCE_FILE_WELCOME "${${PROJECT_NAME}_SOURCE_DIR}/cmake/CPack.SerialBlueTermWelcome.txt")

if(APPLE)
    set(CPACK_GENERATOR PACKAGEMAKER)
elseif(UNIX)
  include(CPackDebConfig)
  include(CPackRpmConfig)
else()
  include(CPackNsisConfig)
endif()

include(CPack)

