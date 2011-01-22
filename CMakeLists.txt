project (flying-penguin CXX)
cmake_minimum_required (VERSION 2.6)

# == Variables ==

if (WIN32)
    set (RESOURCE_DIR "share/flying-penguin/" CACHE FILEPATH "Directory to look for resources")
	set (CONFIG_DIR_ENV "USERPROFILE" CACHE STRING "Environment variable to use in determining the directory for storing settings")
    set (HIGHSCORE_PATH "highscore" CACHE STRING "Path to save the high score tables")
endif ()

set (RESOURCE_DIR "${CMAKE_INSTALL_PREFIX}/share/flying-penguin/" CACHE FILEPATH "Directory to look for resources")
set (RESOURCE_DIR_REL "../share/flying-penguin/" CACHE FILEPATH "Relative paths for use in tarballs")
set (CONFIG_DIR_ENV "HOME" CACHE STRING "Environment variable to use in determining the directory for storing settings")
set (CONFIG_STATIC ON CACHE BOOL "Whether variables are constant in-game (might increase speed)")
set (HIGHSCORE_PATH "/var/games/flying-penguin" CACHE STRING "Path to save the high score tables")

# == Libraries ==

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/misc/cmake")
if (WIN32)
	set (SFMLDIR "${CMAKE_SOURCE_DIR}/misc/windows/sfml/")
	set (CMAKE_BUILD_TYPE Release)
	add_definitions(-DSFML_DYNAMIC)
endif ()
find_package (SFML REQUIRED COMPONENTS Audio Graphics Window System)
include_directories (${SFML_INCLUDE_DIR} ${PROJECT_BINARY_DIR})

# == Source Files ==

# Use this to generate:
# find src -type f -name "*.[h|c]*" | grep -v config.h.in | sort
set (project_sources
    src/apps.cpp
    src/apps.hpp
    src/background.cpp
    src/background.hpp
    src/ConfigFile/ConfigFile.cpp
    src/ConfigFile/ConfigFile.h
    src/entities.cpp
    src/entities.hpp
    src/main.cpp
    src/main.hpp
    src/npc.cpp
    src/npc.hpp
    src/particles.cpp
    src/particles.hpp
    src/penguin.cpp
    src/penguin.hpp
    src/record.cpp
    src/record.hpp
    src/resources.cpp
    src/resources.hpp
    src/util.hpp
    src/world.cpp
    src/world.hpp)
configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_BINARY_DIR}/config.h")
if (WIN32)
	set (project_sources ${project_sources} misc/windows/flying-penguin.rc)
endif ()
add_executable (flying-penguin ${project_sources})

target_link_libraries (flying-penguin ${SFML_LIBRARY})

# == Package Info ==

if (WIN32)
	install (FILES misc/windows/sfml/lib/sfml-audio.dll DESTINATION bin)
	install (FILES misc/windows/sfml/lib/sfml-graphics.dll DESTINATION bin)
	install (FILES misc/windows/sfml/lib/sfml-system.dll DESTINATION bin)
	install (FILES misc/windows/sfml/lib/sfml-window.dll DESTINATION bin)
endif ()

install (TARGETS flying-penguin DESTINATION bin)
install (FILES misc/linux/flying-penguin.desktop DESTINATION share/applications)
install (FILES misc/linux/flying-penguin.png DESTINATION share/pixmaps)
install (DIRECTORY share/flying-penguin/ DESTINATION share/flying-penguin)
install (FILES COPYING README DESTINATION share/flying-penguin)

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_DESCRIPTION "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_VENDOR "Lee Zher Huei")
set (CPACK_PACKAGE_CONTACT "lee@leezh.net")
set (CPACK_PACKAGE_VERSION "0.2")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "2")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

set (CPACK_NSIS_DISPLAY_NAME "The Flying Penguin")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "The Flying Penguin")
set (CPACK_PACKAGE_EXECUTABLES "flying-penguin;The Flying Penguin")
set (CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\flying-penguin.exe")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\orange-install-nsis.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\orange-uninstall-nsis.ico")
set (CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\banner.bmp")

set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Lee Zher Huei <lee@leezh.net>")
set (CPACK_DEBIAN_PACKAGE_SECTION "games")
set (CPACK_DEBIAN_PRIORITY "optional")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-system1.6, libsfml-window1.6, libsfml-graphics1.6, libsfml-audio1.6, libgl1, libglu1, libstdc++6")

include (CPack)

