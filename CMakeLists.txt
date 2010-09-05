project (flying-penguin C CXX)
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

set (CMAKE_BUILD_TYPE Release)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/misc/cmake")
find_package (SFML REQUIRED)
include_directories (${SFML_INCLUDE_DIR} ${PROJECT_BINARY_DIR})
target_link_libraries (flying-penguin ${SFML_LIBRARY})

set(DEBUG_MSG OFF CACHE BOOL "Whether to include debugging messages")

if (WIN32)
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

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A game where you try to fly your penguin as far as possible")
set (CPACK_PACKAGE_VENDOR "Lee Zher Huei")
set (CPACK_PACKAGE_CONTACT "lee@leezh.net")
set (CPACK_PACKAGE_VERSION "0.1")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
include (CPack)

