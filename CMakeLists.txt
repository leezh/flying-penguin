project (flying-penguin CXX)
cmake_minimum_required (VERSION 2.6)

set (project_sources
	src/clouds.cpp
	src/loops.cpp
	src/loops.hpp
	src/main.cpp
	src/main.hpp
	src/particles.cpp
	src/penguin.cpp
	src/resources.cpp
	src/resources.hpp
	src/tween.hpp)
configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_BINARY_DIR}/config.h")
add_executable (flying-penguin ${project_sources})

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/misc/cmake")
if (WIN32)
	set (SFMLDIR "${CMAKE_SOURCE_DIR}/misc/windows/sfml/")
	set (CMAKE_BUILD_TYPE Release)
	add_definitions(-DSFML_DYNAMIC)
endif ()
find_package (SFML REQUIRED COMPONENTS System Window Graphics)
include_directories (${SFML_INCLUDE_DIR} ${PROJECT_BINARY_DIR})
target_link_libraries (flying-penguin ${SFML_LIBRARY})

if (WIN32)
	set(RESOURCE_DIR "" CACHE FILEPATH "Directory to look for resources")
	
	install (TARGETS flying-penguin DESTINATION flying-penguin)
	install (DIRECTORY res/ DESTINATION flying-penguin)
	install (FILES AUTHORS COPYING README DESTINATION flying-penguin)
else ()
	set(RESOURCE_DIR "${CMAKE_INSTALL_PREFIX}/share/flying-penguin/" CACHE FILEPATH "Directory to look for resources")
	
	install (TARGETS flying-penguin DESTINATION bin)
	install (FILES misc/linux/flying-penguin.desktop DESTINATION share/applications)
	install (FILES misc/linux/flying-penguin.png DESTINATION share/pixmaps)
	install (DIRECTORY res/ DESTINATION share/flying-penguin)
	install (FILES AUTHORS COPYING README DESTINATION share/flying-penguin)
endif ()

set (CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_DESCRIPTION "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_VENDOR "Lee Zher Huei")
set (CPACK_PACKAGE_CONTACT "lee@leezh.net")
set (CPACK_PACKAGE_VERSION "0.2")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "2")

set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "The Flying Penguin")
set (CPACK_PACKAGE_EXECUTABLES "flying-penguin;The Flying Penguin")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_NSIS_DISPLAY_NAME "The Flying Penguin")

set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Lee Zher Huei <lee@leezh.net>")
set (CPACK_DEBIAN_PACKAGE_SECTION "games")
set (CPACK_DEBIAN_PRIORITY "optional")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-system1.6, libsfml-window1.6, libsfml-graphics1.6, libsfml-audio1.6, libgl1, libglu1, libstdc++6")

include (CPack)

