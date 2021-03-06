set(CPACK_GENERATOR ${CPACK_GENERATOR};DEB)
set(CPACK_DEB "ON")

set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Serial and RFCOMM Bluetooth communication application")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5 (>=5.4), libqt5bluetooth5 (>=5.4), libqt5serialport5 (>=5.4)")
set(CPACK_DEBIAN_PACKAGE_SECTION "Communication")
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gtkterm")
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "gtkterm")
set(CPACK_DEBIAN_PACKAGE_NAME "sbterm") 
